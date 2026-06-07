#include <malloc.h>

static void	zone_init(t_zone *new_zone, u_int type, size_t zone_size)
{
	new_zone->start = (void *)new_zone;
	new_zone->end = (char *)new_zone + zone_size;
	new_zone->type = type;
	new_zone->blocks_num = 0;
	new_zone->total_space = zone_size - ZONE_HEADER_SIZE;
	new_zone->remaining_space = new_zone->total_space;
	new_zone->max_free_space_size = new_zone->total_space;
	new_zone->blocks = NULL;

	new_zone->empty_spaces = (t_block *)((char *)new_zone->start + ZONE_HEADER_SIZE);
	new_zone->empty_spaces->data_size = new_zone->total_space - HEADER_SIZE;
	new_zone->empty_spaces->end = new_zone->end;
	new_zone->empty_spaces->prev = NULL;
	new_zone->empty_spaces->next = NULL;
	
	new_zone->prev = NULL;
	new_zone->next = g_zones;
	if (g_zones)
		g_zones->prev = new_zone;
	g_zones = new_zone;
}

static t_zone	*create_large_zone(size_t size)
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

static t_zone	*create_zone(u_int type)
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

t_zone	*find_zone(u_int type, size_t size)
{
	t_zone	*zone;
	t_zone	*new_zone;

	if (type == LARGE)
		return create_large_zone(size);

	for (zone = g_zones; zone != NULL; zone = zone->next) {
		if (type == zone->type && zone->max_free_space_size >= size + HEADER_SIZE)
				return zone;
	}

	new_zone = create_zone(type);

	return new_zone;
}