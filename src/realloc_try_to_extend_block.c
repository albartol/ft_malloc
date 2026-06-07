#include <malloc.h>
#include <utils.h>
#include <libft.h>

static t_block *find_empty_after(t_zone *zone, t_block *block)
{
	if (!zone->empty_spaces)
		return NULL;
	for (t_block *empty_iter = zone->empty_spaces; 
		empty_iter && (void *)empty_iter <= block->end; 
		empty_iter = empty_iter->next) {
		if (block->end == (void *)empty_iter)
			return empty_iter;
	}
	return NULL;
}

static t_block *find_empty_before(t_zone *zone, t_block *block)
{
	if (!zone->empty_spaces)
		return NULL;
	for (t_block *empty_iter = zone->empty_spaces; 
		empty_iter && empty_iter->end <= (void *)block; 
		empty_iter = empty_iter->next) {
		if (block == (void *)empty_iter->end)
			return empty_iter;
	}
	return NULL;
}

void	*extend_block(t_zone *zone, t_block *block, size_t size)
{
	t_block	*empty;
	size_t	available_space;
	
	empty = find_empty_after(zone, block);
	if (!empty)
		return NULL;

	available_space = block->data_size + HEADER_SIZE + empty->data_size;
	if (available_space < size)
		return NULL;

	remove_block(zone, empty, EMPTY);
	zone->remaining_space -= HEADER_SIZE + empty->data_size;

	block->data_size = available_space;
	block->end = block_end(block);
	
	if (block->data_size - size >= MIN_SPLIT_SIZE)
		split_block(zone, block, size);
	else
		update_empty_stats(zone);

	return block_payload(block);
}

void	*extend_block_back(t_zone *zone, t_block *block, size_t size)
{
	t_block	*empty;
	t_block *new_block;
	void	*old_payload;
	void	*new_payload;
	size_t	available_space;
	
	empty = find_empty_before(zone, block);
	if (!empty)
		return NULL;

	available_space = empty->data_size + HEADER_SIZE + block->data_size;
	if (available_space < size)
		return NULL;

	remove_block(zone, empty, EMPTY);
	zone->remaining_space -= HEADER_SIZE + empty->data_size;
	
	new_block = empty;
	new_block->data_size = available_space;
	new_block->end = block->end;

	replace_block(zone, block, new_block, BLOCK);
	
	old_payload = block_payload(block);
	new_payload = block_payload(new_block);
	ft_memmove(new_payload, old_payload, block->data_size);

	if (new_block->data_size - size >= MIN_SPLIT_SIZE)
		split_block(zone, new_block, size);
	else
		update_empty_stats(zone);

	return block_payload(new_block);
}

void	*try_to_extend_block(t_zone *zone, t_block *block, size_t size)
{
	void	*new_ptr;

	new_ptr = extend_block(zone, block, size);
	if (new_ptr)
		return new_ptr;

	return extend_block_back(zone, block, size);
}