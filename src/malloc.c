#include <malloc.h>

t_zone *g_zones = NULL;
pthread_mutex_t g_malloc_lock = PTHREAD_MUTEX_INITIALIZER;


void *malloc(size_t size)
{
	void	*ptr;

	ptr = NULL;

	if (!size)
		return NULL;
	
	return ptr;
}