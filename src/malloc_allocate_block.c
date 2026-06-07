#include <malloc.h>
#include <utils.h>
#include <libft.h>

static t_block	*find_empty_space(t_zone *zone, size_t size)
{
	t_block	*empty;

	if (!zone->empty_spaces)
		return NULL;
	for (empty = zone->empty_spaces; empty != NULL; empty = empty->next) {
		if (empty->data_size >= size)
			return empty;
	}
	return NULL;
}

static void	add_block_to_list(t_zone *zone, t_block *new_block)
{
	t_block *block_iter;

	if (!zone->blocks || new_block < zone->blocks) {
		new_block->prev = NULL;
		new_block->next = zone->blocks;
		if (zone->blocks)
			zone->blocks->prev = new_block;
		zone->blocks = new_block;
		return;
	}
	block_iter = zone->blocks;
	while (block_iter->next && block_iter->next < new_block)
		block_iter = block_iter->next;
	
	new_block->next = block_iter->next;
	new_block->prev = block_iter;
	if (block_iter->next)
		block_iter->next->prev = new_block;
	block_iter->next = new_block;
}

static void	update_empty_spaces(t_zone *zone, t_block old_empty, t_block *new_block)
{
	t_block *new_empty;

	if (old_empty.data_size) {
		new_empty = (t_block *)new_block->end;
		new_empty->data_size = old_empty.data_size;
		new_empty->end = old_empty.end;
		replace_block(zone, &old_empty, new_empty, EMPTY);
	}
	else
		remove_block(zone, &old_empty, EMPTY);
}

static t_block	*allocate_from_empty(t_zone *zone, t_block *empty, size_t size)
{
	t_block old_empty;
	t_block *new_block;
	size_t	leftover;

	leftover = empty->data_size - size;
	if (leftover >= MIN_SPLIT_SIZE)
		old_empty.data_size = leftover - HEADER_SIZE;
	else
		old_empty.data_size = 0;
	old_empty.end = empty->end;
	old_empty.prev = empty->prev;
	old_empty.next = empty->next;

	new_block = empty;
	new_block->data_size = size;
	if (!old_empty.data_size)
		new_block->data_size += leftover;
	new_block->end = block_end(new_block);
	new_block->prev = NULL;
	new_block->next = NULL;

	update_empty_spaces(zone, old_empty, new_block);

	add_block_to_list(zone, new_block);
	zone->blocks_num++;
	zone->remaining_space -= (new_block->data_size + HEADER_SIZE);

	update_empty_stats(zone);

	return new_block;
}

t_block	*allocate_block(t_zone *zone, size_t size)
{
	t_block	*empty;

	empty = find_empty_space(zone, size);
	if (!empty)
		return NULL;

	return allocate_from_empty(zone, empty, size);
}