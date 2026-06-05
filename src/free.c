#include <malloc.h>

static t_zone *find_zone_for_ptr(void *ptr)
{
	t_zone	*zone;

	if (!ptr)
		return NULL;
	for (zone = g_zones; zone; zone = zone->next) {
		if (zone->start <= ptr && zone->end >= ptr)
			return zone;
	}
	return NULL;
}

static int is_valid_block(t_block *block, t_zone *zone)
{
	t_block *block_list;

	if (!block || !zone)
		return 0;
	for (block_list = zone->blocks; block_list; block_list = block_list->next) {
		if (block_list == block)
			return 1;
	}
	return 0;
}

void free(void *ptr)
{
	t_zone	*zone;
	t_block	*block;

	if (!ptr)
		return;

	pthread_mutex_lock(&g_malloc_lock);
	{
		zone = find_zone_for_ptr(ptr);
		if (!zone) {
			pthread_mutex_unlock(&g_malloc_lock);
			return;
		}
		block = (t_block *)(ptr - HEADER_SIZE);
		if (!is_valid_block(block, zone)) {
			pthread_mutex_unlock(&g_malloc_lock);
			return;
		}
		if (block->is_free) {
			pthread_mutex_unlock(&g_malloc_lock);
			return;
		}
		block->is_free = 1;
		zone->free_blocks++;
		zone->remaining_space += block->data_size;
	}
	pthread_mutex_unlock(&g_malloc_lock);
}