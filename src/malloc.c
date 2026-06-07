#include <malloc.h>
#include <utils.h>
#include <colors.h>

__attribute__((constructor))
static void	init_malloc(void)
{
	write(STDOUT_FILENO, MAGENTA"[42 MALLOC] Library loaded!\n"NC, 40);
}

t_zone *g_zones = NULL;
pthread_mutex_t g_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

void	*malloc(size_t size)
{
	u_int	type;
	t_zone	*zone;
	t_block	*block;
	void	*ptr;

	if (!size || size > MAX_ALLOC_SIZE)
		return NULL;

	size = align_size(size);

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

		ptr = block_payload(block);
	}
	pthread_mutex_unlock(&g_malloc_lock);

	return ptr;
}