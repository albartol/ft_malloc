#include <malloc.h>
#include <utils.h>

t_zone	*find_zone_for_ptr(void *ptr)
{
	t_zone	*zone;

	if (!ptr)
		return NULL;
	for (zone = g_zones; zone; zone = zone->next) {
		if ((char *)zone->start <= (char *)ptr && (char *)zone->end > (char *)ptr)
			return zone;
	}
	return NULL;
}

u_int	is_valid_block(t_block *block, t_zone *zone)
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

void	free_large_zone(t_zone *zone)
{
	if (zone->prev)
		zone->prev->next = zone->next;
	else
		g_zones = zone->next;
	if (zone->next)
		zone->next->prev = zone->prev;

	munmap(zone, zone->total_space + ZONE_HEADER_SIZE);
}

void	merge_empty_spaces(t_zone *zone)
{
	t_block	*empty;
	t_block	*next_empty;
	size_t	gap;

	empty = zone->empty_spaces;
	while (empty && empty->next) {
		next_empty = empty->next;
		gap = (char *)next_empty - (char *)empty->end;
		if (gap < MIN_SPLIT_SIZE) {
			empty->data_size += gap + next_empty->data_size + HEADER_SIZE;
			empty->end = next_empty->end;
			empty->next = next_empty->next;
			if (next_empty->next)
				next_empty->next->prev = empty;
		}
		else
			empty = next_empty;
	}
	update_empty_stats(zone);
}