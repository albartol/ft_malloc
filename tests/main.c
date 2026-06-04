#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	printf("\n");
	printf("PAGESIZE: %lu\n", PAGESIZE);
	printf("MAX_ALLOC_SIZE: %lu\n", MAX_ALLOC_SIZE);
	printf("TINY_ZONE_SIZE: %lu\n", TINY_ZONE_SIZE);
	printf("SMALL_ZONE_SIZE: %lu\n", SMALL_ZONE_SIZE);
	printf("HEADER_SIZE: %lu\n", HEADER_SIZE);
	printf("ZONE_HEADER_SIZE: %lu\n", ZONE_HEADER_SIZE);
	printf("ALIGNMENT: %d\n", ALIGNMENT);
	printf("TINY_MIN: %d\n", TINY_MIN);
	printf("TINY_MAX: %lu\n", TINY_MAX);
	printf("TINY_DATA_MIN: %d\n", TINY_DATA_MIN);
	printf("TINY_DATA_MAX: %d\n", TINY_DATA_MAX);
	printf("SMALL_MIN: %d\n", SMALL_MIN);
	printf("SMALL_MAX: %lu\n", SMALL_MAX);
	printf("SMALL_DATA_MIN: %d\n", SMALL_DATA_MIN);
	printf("SMALL_DATA_MAX: %d\n", SMALL_DATA_MAX);
	printf("LARGE_MIN: %d\n", LARGE_MIN);
	printf("LARGE_DATA_MIN: %d\n", LARGE_DATA_MIN);
	printf("\n");

	void *p1 = malloc(10);
	void *p2 = malloc(100);
	void *p3 = malloc(1000);
	void *p4 = malloc(2000);
	void *p5 = malloc(5000);
	void *p6 = malloc(10000);

	if (p1 && p2 && p3 && p4 && p5 && p6)
		printf("All allocations successful!\n");
	printf("\n");
	
	printf("p1: %p - decimal value: %lu\n", p1, (size_t)p1);
	printf("p2: %p - decimal value: %lu\n", p2, (size_t)p2);
	printf("p3: %p - decimal value: %lu\n", p3, (size_t)p3);
	printf("p4: %p - decimal value: %lu\n", p4, (size_t)p4);
	printf("p5: %p - decimal value: %lu\n", p5, (size_t)p5);
	printf("p6: %p - decimal value: %lu\n", p6, (size_t)p6);
	printf("\n");
	printf("Check alignment (p1 mod 16): %lu\n", (size_t)p1 % ALIGNMENT);
	printf("Check alignment (p2 mod 16): %lu\n", (size_t)p2 % ALIGNMENT);
	printf("Check alignment (p3 mod 16): %lu\n", (size_t)p3 % ALIGNMENT);
	printf("Check alignment (p4 mod 16): %lu\n", (size_t)p4 % ALIGNMENT);
	printf("Check alignment (p5 mod 16): %lu\n", (size_t)p5 % ALIGNMENT);
	printf("Check alignment (p6 mod 16): %lu\n", (size_t)p6 % ALIGNMENT);
	printf("\n");
	printf("Distance between p1 and p2: %lu\n", (p2 > p1) ? p2 - p1 : p1 - p2);
	printf("Distance between p2 and p3: %lu\n", (p3 > p2) ? p3 - p2 : p2 - p3);
	printf("Distance between p3 and p4: %lu\n", (p4 > p3) ? p4 - p3 : p3 - p4);
	printf("Distance between p4 and p5: %lu\n", (p5 > p4) ? p5 - p4 : p4 - p5);
	printf("Distance between p5 and p6: %lu\n", (p6 > p5) ? p6 - p5 : p5 - p6);
	printf("\n");
	printf("Size of p1: %lu\n", sizeof(p1));
	printf("Size of p2: %lu\n", sizeof(p2));
	printf("Size of p3: %lu\n", sizeof(p3));
	printf("Size of p4: %lu\n", sizeof(p4));
	printf("Size of p5: %lu\n", sizeof(p5));
	printf("Size of p6: %lu\n", sizeof(p6));
	printf("\n");

	void *p7 = malloc(0);
	if (!p7)
		printf("malloc(0) == NULL\nError: %s\n", strerror(errno));
	printf("\n");
	
	void *p8 = malloc(MAX_ALLOC_SIZE);
	if (!p8)
		printf("malloc(%lu) == NULL\nError: %s\n", MAX_ALLOC_SIZE, strerror(errno));
	printf("\n");
	
	void *p9 = malloc(MAX_ALLOC_SIZE - ((HEADER_SIZE + ZONE_HEADER_SIZE) << 1));
	if (!p9)
		printf("malloc(%lu) == NULL\nError: %s\n", 
	MAX_ALLOC_SIZE - ((HEADER_SIZE + ZONE_HEADER_SIZE) << 1), strerror(errno));
	printf("\n");

	return 0;
}