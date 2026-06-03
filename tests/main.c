#include <stdio.h>
#include <malloc.h>

int main(void)
{
    printf("PAGESIZE: %ld\n", get_pagesize());
    printf("TINY_ZONE_SIZE: %ld\n", TINY_ZONE_SIZE);
    printf("SMALL_ZONE_SIZE: %ld\n", SMALL_ZONE_SIZE);
    printf("HEADER_SIZE: %ld\n", HEADER_SIZE);
    printf("TINY_MIN: %d\n", TINY_MIN);
    printf("TINY_MAX: %d\n", TINY_MAX);
    printf("TINY_DATA_MIN: %d\n", TINY_DATA_MIN);
    printf("TINY_DATA_MAX: %d\n", TINY_DATA_MAX);
    printf("SMALL_MIN: %d\n", SMALL_MIN);
    printf("SMALL_MAX: %d\n", SMALL_MAX);
    printf("SMALL_DATA_MIN: %d\n", SMALL_DATA_MIN);
    printf("SMALL_DATA_MAX: %d\n", SMALL_DATA_MAX);
    printf("LARGE_MIN: %d\n", LARGE_MIN);
    printf("LARGE_DATA_MIN: %d\n", LARGE_DATA_MIN);
    return 0;
}