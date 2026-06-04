#ifndef MALLOC_H
# define MALLOC_H

# include <libft.h>
# include <unistd.h>
# include <stdint.h>
# include <sys/mman.h>
# include <sys/resource.h>

// # if defined(__APPLE__)
// #  define PAGESIZE (getpagesize())
// # elif defined(__linux__)
// #  define PAGESIZE (sysconf(_SC_PAGESIZE))
// # endif

static inline size_t	get_pagesize(void)
{
	static size_t pagesize = 0;

	if (pagesize)
		return pagesize;

	# if defined(__APPLE__)
	pagesize = getpagesize();
	# elif defined(__linux__)
	pagesize = sysconf(_SC_PAGESIZE);
	# endif

	return pagesize;
}

// Max size of a single allocation
static inline size_t get_max_alloc_size(void)
{
	static size_t max_size = 0;
	struct rlimit limit;
	
	if (max_size)
		return max_size;

	if (getrlimit(RLIMIT_AS, &limit) == -1)
		max_size = SIZE_MAX;
	else
		max_size = limit.rlim_cur;

	return max_size;
}

# define PAGESIZE get_pagesize()
# define MAX_ALLOC_SIZE get_max_alloc_size()

// Zone sizes in page multiples (PAGESIZE: 4KB)
// 128 tiny allocs 192 bytes each (24KB)
# define TINY_ZONE_SIZE (PAGESIZE * 6)
// 128 small allocs 3072 bytes each (384KB)
# define SMALL_ZONE_SIZE (TINY_ZONE_SIZE * 16)

// Structure for tracking allocations
typedef struct s_block	t_block;
// Structure for tracking allocation zones
typedef struct s_zone	t_zone;

// Allocation size boundaries
// TINY: 32 bytes header + 1-160 bytes data (33-192 bytes total)
// SMALL: 32 bytes header + 161-3040 bytes data (193-3072 bytes total)
// LARGE: 32 bytes header + 3041-n bytes data (3073-n bytes total)

// To ensure a proper alignment the size of the headers must be 
// multiple of ALIGNMENT.
# define HEADER_SIZE (sizeof(t_block))
# define ZONE_HEADER_SIZE (sizeof(t_zone))
// HEADER_SIZE % ALIGNMENT == 0
// 32 % 16 == 0
// 64 % 16 == 0
# define ALIGNMENT 16

// TINY:
// Data size is between 1 and 160 bytes
# define TINY_DATA_MIN 1
// # define TINY_DATA_MAX 160
# define TINY_DATA_MAX (ALIGNMENT * 10)
// This zone blocks have between 33 and 192 bytes
# define TINY_MIN 33
// # define TINY_MAX 192
# define TINY_MAX (TINY_DATA_MAX + HEADER_SIZE)

// SMALL:
// Data size is between 161 and 3040 bytes
# define SMALL_DATA_MIN 161
// # define SMALL_DATA_MAX 3040
# define SMALL_DATA_MAX (TINY_DATA_MAX * 19)
// This zone blocks have between 193 and 3072 bytes
# define SMALL_MIN 193
// # define SMALL_MAX 3072
# define SMALL_MAX (SMALL_DATA_MAX + HEADER_SIZE)

// LARGE:
// Anything over 3040 bytes is LARGE (allocated directly)
// This zone is not tracked by the freelist
# define LARGE_MIN 3073
# define LARGE_DATA_MIN 3041

# define IS_TINY(size) ((size) >= TINY_DATA_MIN && (size) <= TINY_DATA_MAX)
# define IS_SMALL(size) ((size) >= SMALL_DATA_MIN && (size) <= SMALL_DATA_MAX)
# define IS_LARGE(size) ((size) >= LARGE_DATA_MIN)

enum e_type {
	TINY = 0,
	SMALL = 1,
	LARGE = 2,
};

struct s_block
{
	size_t	data_size;
	u_int	is_free;
	u_int	type;
	t_block	*prev;
	t_block	*next;
};

struct s_zone
{
	void	*start;
	void	*end;
	void	*last_addr;
	u_int	type;
	u_int	free_blocks;
	size_t	remaining_space;
	t_block	*blocks;
	t_zone	*prev;
	t_zone	*next;
};

// Global allocator state
extern t_zone *g_zones;

// Deallocates the memory allocation pointed to by ptr.
// If ptr is a NULL pointer, no operation is performed.
void	free(void *ptr);

// Allocates size bytes for a memory block and returns a pointer to it.
// If size is 0, it returns NULL.
void	*malloc(size_t size);

// Reallocates size bytes for a memory block pointed to by ptr.
// If ptr is NULL, it behaves like a call to malloc(size).
// If size is 0 and ptr isn't NULL, it behaves like a call to free(ptr).
// If there is not enough space in the zone to satisfy the request, 
// it allocates a new block (possibly in a new zone) and copies data.
void	*realloc(void *ptr, size_t size);

// Prints a visualization of the state of the allocated memory zones.
// Example of output:
	// TINY : 0xA0000
	// 0xA0020 - 0xA004A : 42 bytes
	// 0xA006A - 0xA00BE : 84 bytes
	// SMALL : 0xAD000
	// 0xAD020 - 0xADEAD : 3725 bytes
	// LARGE : 0xB0000
	// 0xB0020 - 0xBBEEF : 48847 bytes
	// Total : 52698 bytes
void	show_alloc_mem();

//BONUS:

// Thread safety
# include <pthread.h>
// Mutex for the global lock.
extern pthread_mutex_t g_malloc_lock;

// show_alloc_mem but with extra details (history, hexadecimal dump).
void	show_alloc_mem_ex();

#endif // MALLOC_H
