#include <malloc.h>
#include <colors.h>

__attribute__((constructor))
static void init_malloc(void)
{
	write(STDOUT_FILENO, MAGENTA"[42 MALLOC] Library loaded!\n"NC, 40);
}

t_zone *g_zones = NULL;
pthread_mutex_t g_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

static size_t	aling_size(size_t size)
{
	const size_t	alignment = ALIGNMENT;
	if (size > SIZE_MAX - alignment)
		return size;
	return ((size + alignment - 1) & -alignment);
	// return ((size + alignment - 1) & ~(alignment - 1));
}

static int	get_zone_type(size_t size)
{
	if (IS_TINY(size))
		return TINY;
	else if (IS_SMALL(size))
		return SMALL;
	return LARGE;
}

static void zone_init(t_zone *new_zone, u_int type, size_t zone_size)
{
	new_zone->start = (void *)new_zone;
	new_zone->end = (void *)new_zone + ZONE_HEADER_SIZE;
	new_zone->last_addr = (void *)new_zone + zone_size;
	new_zone->type = type;
	new_zone->free_blocks = 0;
	new_zone->remaining_space = zone_size - ZONE_HEADER_SIZE;
	new_zone->blocks = NULL;
	new_zone->prev = NULL;
	new_zone->next = g_zones;
	if (g_zones)
		g_zones->prev = new_zone;
	g_zones = new_zone;
}

static t_zone *create_large_zone(size_t size)
{
	t_zone	*new_zone;
	size_t	zone_size;

	if (size > SIZE_MAX - HEADER_SIZE - ZONE_HEADER_SIZE)
		return (NULL);

	zone_size = size + HEADER_SIZE + ZONE_HEADER_SIZE;
	new_zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, 
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (new_zone == MAP_FAILED)
		return NULL;
	zone_init(new_zone, LARGE, zone_size);

	return new_zone;
}

static t_zone *create_zone(u_int type)
{
	t_zone *new_zone;
	size_t	zone_size;

	if (type == TINY)
		zone_size = TINY_ZONE_SIZE;
	else if (type == SMALL)
		zone_size = SMALL_ZONE_SIZE;	
	else
		return NULL;
	new_zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, 
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (new_zone == MAP_FAILED)
		return NULL;
	zone_init(new_zone, type, zone_size);

	return new_zone;
}

static t_zone *find_zone(u_int type, size_t size)
{
	t_zone	*zone;
	t_zone	*new_zone;

	if (type == LARGE)
		return create_large_zone(size);

	for (zone = g_zones; zone != NULL; zone = zone->next) {
		if (type == zone->type && zone->remaining_space >= size + HEADER_SIZE)
			return zone;
	}

	new_zone = create_zone(type);

	return new_zone;
}

static t_block *find_free_block(t_zone *zone, size_t size)
{
	t_block	*block;

	if (!zone->free_blocks)
		return NULL;
	for (block = zone->blocks; block != NULL; block = block->next) {
		if (block->is_free && block->data_size >= size)
			return block;
	}
	return NULL;
}

static int defragment_zone(t_zone *zone, size_t size)
{
	t_block *block;
	t_block *new_block;
	void	*old_pos;
	void	*new_pos;
	size_t	total_allocated;

	if (!zone->blocks)
		return 0;

	total_allocated = ((size_t)zone->last_addr - (size_t)zone->start) - zone->remaining_space;
	if (total_allocated + size + HEADER_SIZE > 
		(size_t)zone->last_addr - (size_t)zone->start)
		return 0;

	new_pos = (void *)zone->start + ZONE_HEADER_SIZE;
	for (block = zone->blocks; block != NULL; block = block->next) {
		if (!block->is_free) {
			old_pos = (void *)block;
			if (old_pos != new_pos) {
				new_block = (t_block *)new_pos;
				ft_memmove(new_block, block, block->data_size + HEADER_SIZE);

				if (new_block->prev)
					new_block->prev->next = new_block;
				else
					zone->blocks = new_block;
				if (new_block->next)
					new_block->next->prev = new_block;
				block = new_block;
			}
			new_pos = (void *)new_pos + block->data_size + HEADER_SIZE;
		}
	}

	zone->end = new_pos;
	zone->remaining_space = (size_t)zone->last_addr - (size_t)zone->end;
	zone->free_blocks = 0;

	return 1;
}

static t_block *create_block(t_zone *zone, size_t size)
{
	t_block *block;
	t_block *new_block;

	new_block = (t_block *)zone->end;
	new_block->data_size = size;
	new_block->is_free = 0;
	new_block->prev = NULL;
	new_block->next = NULL;

	if (!zone->blocks)
		zone->blocks = new_block;
	else {
		for (block = zone->blocks; block && block->next; block = block->next)
		{}
		block->next = new_block;
		new_block->prev = block;
	}

	zone->end = (void *)zone->end + size + HEADER_SIZE;

	return new_block;
}

static t_block *allocate_block(t_zone *zone, size_t size)
{
	t_block	*block;

	block = find_free_block(zone, size);
	if (block) {
		block->is_free = 0;
		zone->free_blocks--;
		// block->data_size = size;
		// zone->remaining_space -= size;
		zone->remaining_space -= block->data_size;
		return block;
	}

	if ((size_t)zone->end + size + HEADER_SIZE > (size_t)zone->last_addr) {
		if (!defragment_zone(zone, size))
			return NULL;
	}

	return create_block(zone, size);
}

void *malloc(size_t size)
{
	int		type;
	t_zone	*zone;
	t_block	*block;
	void	*ptr;

	if (!size || size > MAX_ALLOC_SIZE)
		return NULL;

	size = aling_size(size);

	pthread_mutex_lock(&g_malloc_lock);
	{
		type = get_zone_type(size);

		zone = find_zone(type, size);
		if (!zone) {
			pthread_mutex_unlock(&g_malloc_lock);
			return NULL;
		}
		
		block = allocate_block(zone, size);
		if (!block) {
			pthread_mutex_unlock(&g_malloc_lock);
			return NULL;
		}

		ptr = (void *)block + HEADER_SIZE;
	}
	pthread_mutex_unlock(&g_malloc_lock);

	return ptr;
}