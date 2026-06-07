#include <malloc.h>
#include <utils.h>
#include <libft.h>
#include <colors.h>

static size_t	print_zone(t_zone *zone)
{
	size_t	total_allocated = 0;

	printf(BLUE"%s :"TEAL_256" %p\n"NC, get_type_str(zone->type), zone->start);

	for (t_block *block = zone->blocks; block; block = block->next) {
		printf(TEAL_256"\t%p - %p : %zu bytes\n"NC, block, 
			block->end, block->data_size + HEADER_SIZE);
		total_allocated += block->data_size;
	}

	return total_allocated;
}

void    show_alloc_mem(void)
{
	size_t	total_allocated = 0;

	if (!g_zones) {
		printf(YELLOW"No zones allocated\n"NC);
		return;
	}
	pthread_mutex_lock(&g_malloc_lock);
	{
		for (t_zone *zone = g_zones; zone; zone = zone->next) {
			total_allocated += print_zone(zone);
			printf(NC"\n");
		}
		printf(BLUE"Total :"TEAL_256" %zu "BLUE"bytes\n", total_allocated);
	}
	pthread_mutex_unlock(&g_malloc_lock);
}