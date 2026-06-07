#include <malloc.h>
#include <utils.h>

void	convert_block_to_empty(t_zone *zone, t_block *block)
{
	t_block *empty_iter;

	remove_block(zone, block, BLOCK);
	zone->blocks_num--;
	zone->remaining_space += (block->data_size + HEADER_SIZE);

	if (!zone->empty_spaces || block < zone->empty_spaces) {
		block->next = zone->empty_spaces;
		block->prev = NULL;
		if (zone->empty_spaces)
			zone->empty_spaces->prev = block;
		zone->empty_spaces = block;
	}
	else {
		empty_iter = zone->empty_spaces;
		while (empty_iter->next && empty_iter->next < block)
			empty_iter = empty_iter->next;
		
		block->next = empty_iter->next;
		block->prev = empty_iter;
		if (empty_iter->next)
			empty_iter->next->prev = block;
		empty_iter->next = block;
	}

	merge_empty_spaces(zone);
}