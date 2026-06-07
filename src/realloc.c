#include <malloc.h>
#include <utils.h>
#include <libft.h>

static void	*realloc_malloc(size_t size)
{
	u_int	type;
	t_zone	*zone;
	t_block	*block;
	void	*ptr;

	if (!size || size > MAX_ALLOC_SIZE)
		return NULL;

	size = align_size(size);

	type = get_zone_type(size);

	zone = find_zone(type, size);
	if (!zone)
		return NULL;
	block = allocate_block(zone, size);
	if (!block)
		return NULL;

	ptr = block_payload(block);

	return ptr;
}

static void	realloc_free(void *ptr)
{
	t_zone	*zone;
	t_block	*block;

	if (!ptr)
		return;

	zone = find_zone_for_ptr(ptr);
	if (!zone)
		return;
	block = block_from_payload(ptr);
	if (!is_valid_block(block, zone))
		return;
	if (zone->type == LARGE) {
		free_large_zone(zone);
		return;
	}
	convert_block_to_empty(zone, block);
}

void *realloc(void *ptr, size_t size)
{
	t_zone	*zone;
	t_block	*block;
	void	*new_ptr;
	size_t	old_size;

	if (!ptr)
		return (malloc(size));
	if (!size) {
		free(ptr);
		return NULL;
	}

	size = align_size(size);

	pthread_mutex_lock(&g_malloc_lock);
	{
		zone = find_zone_for_ptr(ptr);
		if (!zone) {
			pthread_mutex_unlock(&g_malloc_lock);
			return NULL;
		}
		block = block_from_payload(ptr);
		if (!is_valid_block(block, zone)) {
			pthread_mutex_unlock(&g_malloc_lock);
			return NULL;
		}
		old_size = block->data_size;
		if (old_size >= size) {
			if (old_size - size >= MIN_SPLIT_SIZE)
				split_block(zone, block, size);
			pthread_mutex_unlock(&g_malloc_lock);
			return ptr;
		}
		if (get_zone_type(size) == zone->type) {
			new_ptr = try_to_extend_block(zone, block, size);
			if (new_ptr) {
				pthread_mutex_unlock(&g_malloc_lock);
				return new_ptr;
			}
		}
		new_ptr = realloc_malloc(size);
		if (!new_ptr) {
			pthread_mutex_unlock(&g_malloc_lock);
			return NULL;
		}
		ft_memmove(new_ptr, ptr, old_size);
		realloc_free(ptr);
	}
	pthread_mutex_unlock(&g_malloc_lock);

	return new_ptr;
}