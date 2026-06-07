#include <malloc.h>

size_t	align_size(size_t size)
{
	const size_t	alignment = ALIGNMENT;
	if (size > SIZE_MAX - alignment)
		return size;
	return ((size + alignment - 1) & -alignment);
	// return ((size + alignment - 1) & ~(alignment - 1));
}

u_int	get_zone_type(size_t size)
{
	if (IS_TINY(size))
		return TINY;
	else if (IS_SMALL(size))
		return SMALL;
	return LARGE;
}

void	update_empty_stats(t_zone *zone)
{
	if (!zone || !zone->empty_spaces)
		return;
	zone->max_free_space_size = zone->empty_spaces->data_size;
	for (t_block *empty_iter = zone->empty_spaces; empty_iter != NULL; 
		empty_iter = empty_iter->next) {
		if (empty_iter->data_size > zone->max_free_space_size)
			zone->max_free_space_size = empty_iter->data_size;
	}
}

void	replace_block(t_zone *zone, t_block *old, t_block *new, u_int type)
{
	if (!zone || !old || !new)
		return;

	new->prev = old->prev;
	new->next = old->next;

	if (old->prev)
		old->prev->next = new;
	else if (type == EMPTY)
		zone->empty_spaces = new;
	else
		zone->blocks = new;

	if (old->next)
		old->next->prev = new;
}

void	remove_block(t_zone *zone, t_block *old, u_int type)
{
	if (!zone || !old)
		return;

	if (old->prev)
		old->prev->next = old->next;
	else if (type == EMPTY)
		zone->empty_spaces = old->next;
	else
		zone->blocks = old->next;

	if (old->next)
		old->next->prev = old->prev;
}