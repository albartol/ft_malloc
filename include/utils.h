#ifndef UTILS_H
# define UTILS_H

# include <malloc.h>

// malloc utils
size_t	align_size(size_t size);
u_int	get_zone_type(size_t size);
void	update_empty_stats(t_zone *zone);
void	replace_block(t_zone *zone, t_block *old, t_block *new, u_int type);
void	remove_block(t_zone *zone, t_block *old, u_int type);
t_zone	*find_zone(u_int type, size_t size);
t_block	*allocate_block(t_zone *zone, size_t size);
static inline void	*block_payload(t_block *block)
{
	return (char *)block + HEADER_SIZE;
}
static inline void	*block_end(t_block *block)
{
	return (char *)block + HEADER_SIZE + block->data_size;
}

// free utils
u_int	is_valid_block(t_block *block, t_zone *zone);
t_zone	*find_zone_for_ptr(void *ptr);
void	free_large_zone(t_zone *zone);
void	merge_empty_spaces(t_zone *zone);
void	convert_block_to_empty(t_zone *zone, t_block *block);
static inline t_block	*block_from_payload(void *block)
{
	return (t_block *)((char *)block - HEADER_SIZE);
}

// realloc utils
void	split_block(t_zone *zone, t_block *block, size_t new_size);
void	*try_to_extend_block(t_zone *zone, t_block *block, size_t size);

// show_alloc_mem utils
static inline const char	*get_type_str(u_int type)
{
	return type == TINY ? "TINY" : type == SMALL ? "SMALL" : "LARGE";
}

#endif // UTILS_H