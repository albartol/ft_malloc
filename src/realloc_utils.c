#include <malloc.h>
#include <utils.h>

void	split_block(t_zone *zone, t_block *block, size_t new_size)
{
	t_block	*new_empty;
	t_block	*empty_iter;
	size_t	leftover;

	leftover = block->data_size - new_size;
	if (leftover < MIN_SPLIT_SIZE)
		return;

	block->data_size = new_size;
	block->end = block_end(block);
	
	zone->remaining_space += leftover;
	new_empty = (t_block *)block->end;
	
	new_empty->data_size = leftover - HEADER_SIZE;
	new_empty->end = block_end(new_empty);

	if (!zone->empty_spaces || new_empty < zone->empty_spaces) {
		new_empty->next = zone->empty_spaces;
		new_empty->prev = NULL;
		if (zone->empty_spaces)
			zone->empty_spaces->prev = new_empty;
		zone->empty_spaces = new_empty;
	}
	else {
		empty_iter = zone->empty_spaces;
		while (empty_iter->next && empty_iter->next < new_empty)
			empty_iter = empty_iter->next;
		
		new_empty->next = empty_iter->next;
		new_empty->prev = empty_iter;
		if (empty_iter->next)
			empty_iter->next->prev = new_empty;
		empty_iter->next = new_empty;
	}
	merge_empty_spaces(zone);
}