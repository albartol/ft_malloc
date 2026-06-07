#include <malloc.h>
#include <utils.h>

void	free(void *ptr)
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
		block = block_from_payload(ptr);
		if (!is_valid_block(block, zone)) {
			pthread_mutex_unlock(&g_malloc_lock);
			return;
		}
		if (zone->type == LARGE) {
			free_large_zone(zone);
			pthread_mutex_unlock(&g_malloc_lock);
			return;
		}
		convert_block_to_empty(zone, block);
	}
	pthread_mutex_unlock(&g_malloc_lock);
}