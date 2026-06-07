#include <malloc.h>
#include <utils.h>
#include <libft.h>
#include <colors.h>

static size_t	count_items(t_zone *zone)
{
	t_block	*block;
	t_block	*empty;
	size_t	count = 0;

	for (block = zone->blocks; block; block = block->next)
		count++;
	for (empty = zone->empty_spaces; empty; empty = empty->next)
		count++;

	return count;
}

static size_t	print_zone(t_zone *zone)
{
	t_block *block;
	t_block *empty;
	size_t	item_count;
	size_t	total_allocated = 0;

	item_count = count_items(zone);
	if (!item_count)
		return total_allocated;

	printf(BLUE"ZONE:"CYAN" %s :"TEAL_256" %p - %p "CYAN": %zu bytes\n"NC, 
		get_type_str(zone->type), zone->start, zone->end, 
		zone->total_space + ZONE_HEADER_SIZE);

	block = zone->blocks;
	empty = zone->empty_spaces;
	for (size_t i = 0; i < item_count; i++) {
		if (block && block < empty) {
			printf(CYAN"\tBLOCK:"TEAL_256" %p - %p "CYAN": %zu bytes\n"NC, block, 
				block->end, block->data_size + HEADER_SIZE);
			total_allocated += block->data_size;
			block = block->next;
		} else if (empty) {
			printf(CYAN"\tEMPTY:"TEAL_256" %p - %p "CYAN": %zu bytes\n"NC, empty, 
				empty->end, empty->data_size + HEADER_SIZE);
			empty = empty->next;
		}
	}

	return total_allocated;
}

void    show_alloc_mem_ex(void)
{
	size_t	total_allocated = 0;
	u_int	total_blocks = 0;
	u_int	total_empty = 0;

	if (!g_zones) {
		printf(YELLOW"No zones allocated\n"NC);
		return;
	}
	pthread_mutex_lock(&g_malloc_lock);
	{
		printf("\n");
		printf("====================== EXTENDED MEMORY INFO ======================\n");
		printf("\n");

		for (t_zone *zone = g_zones; zone; zone = zone->next) {
			total_allocated += print_zone(zone);
			printf(NC"\n");
		}
		printf("Total : %zu bytes\n", total_allocated);
		
		printf("\n");
		printf("====================== DETAILED STATISTICS ======================\n");
		printf("\n");

		for (t_zone *zone = g_zones; zone; zone = zone->next) {
			printf("Zone: %s\n", 
				zone->type == TINY ? "TINY" : 
				zone->type == SMALL ? "SMALL" : "LARGE");
			printf("\tStart: %p\n", zone->start);
			printf("\tEnd: %p\n", zone->end);
			printf("\tTotal Space: %zu bytes\n", zone->total_space);
			printf("\tRemaining Space: %zu bytes\n", zone->remaining_space);
			printf("\tMax Free Space: %zu bytes\n", zone->max_free_space_size);
			printf("\tAllocated Blocks: %u\n", zone->blocks_num);

			int empty_count = 0;
			for (t_block *block = zone->empty_spaces; block; block = block->next)
				empty_count++;
			printf("\tEmpty Spaces: %d\n", empty_count);
			printf("\n");

			total_blocks += zone->blocks_num;
			total_empty += empty_count;
		}

		printf("====================== GLOBAL STATISTICS ======================\n");
		printf("Total Allocated Blocks: %d\n", total_blocks);
		printf("Total Empty Spaces: %d\n", total_empty);
		printf("\n");
	}
	pthread_mutex_unlock(&g_malloc_lock);
}