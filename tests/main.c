#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <colors.h>

static void print_info(void)
{
	printf(GOLD_TEXT"\n");
	printf("====================== Size of variables ======================\n");
	printf(NC"\n");
	printf(BLUE"PAGESIZE:"TEAL_256" %lu\n", PAGESIZE);
	printf(BLUE"MAX_ALLOC_SIZE:"TEAL_256" %lu\n", MAX_ALLOC_SIZE);
	printf(BLUE"TINY_ZONE_SIZE:"TEAL_256" %lu\n", TINY_ZONE_SIZE);
	printf(BLUE"SMALL_ZONE_SIZE:"TEAL_256" %lu\n", SMALL_ZONE_SIZE);
	printf(BLUE"HEADER_SIZE:"TEAL_256" %lu\n", HEADER_SIZE);
	printf(BLUE"ZONE_HEADER_SIZE:"TEAL_256" %lu\n", ZONE_HEADER_SIZE);
	printf(BLUE"ALIGNMENT:"TEAL_256" %d\n", ALIGNMENT);
	printf(BLUE"TINY_MIN:"TEAL_256" %d\n", TINY_MIN);
	printf(BLUE"TINY_MAX:"TEAL_256" %lu\n", TINY_MAX);
	printf(BLUE"TINY_DATA_MIN:"TEAL_256" %d\n", TINY_DATA_MIN);
	printf(BLUE"TINY_DATA_MAX:"TEAL_256" %d\n", TINY_DATA_MAX);
	printf(BLUE"SMALL_MIN:"TEAL_256" %d\n", SMALL_MIN);
	printf(BLUE"SMALL_MAX:"TEAL_256" %lu\n", SMALL_MAX);
	printf(BLUE"SMALL_DATA_MIN:"TEAL_256" %d\n", SMALL_DATA_MIN);
	printf(BLUE"SMALL_DATA_MAX:"TEAL_256" %d\n", SMALL_DATA_MAX);
	printf(BLUE"LARGE_MIN:"TEAL_256" %d\n", LARGE_MIN);
	printf(BLUE"LARGE_DATA_MIN:"TEAL_256" %d\n", LARGE_DATA_MIN);
	printf("\n");
}

static void test_malloc(void)
{
	printf(GOLD_TEXT"\n");
	printf("====================== Testing malloc() ======================\n");
	printf(NC"\n");
	
	void *p1 = malloc(10);
	void *p2 = malloc(100);
	void *p3 = malloc(1000);
	void *p4 = malloc(2000);
	void *p5 = malloc(5000);
	void *p6 = malloc(10000);

	if (p1 && p2 && p3 && p4 && p5 && p6)
		printf(GREEN"All allocations successful!\n"NC);
	else
		printf(RED"Error! One or more allocations failed!\n"NC);
	printf("\n");
	
	printf(BLUE"p1:"TEAL_256" %p "BLUE"- decimal value:"TEAL_256" %lu\n", p1, (size_t)p1);
	printf(BLUE"p2:"TEAL_256" %p "BLUE"- decimal value:"TEAL_256" %lu\n", p2, (size_t)p2);
	printf(BLUE"p3:"TEAL_256" %p "BLUE"- decimal value:"TEAL_256" %lu\n", p3, (size_t)p3);
	printf(BLUE"p4:"TEAL_256" %p "BLUE"- decimal value:"TEAL_256" %lu\n", p4, (size_t)p4);
	printf(BLUE"p5:"TEAL_256" %p "BLUE"- decimal value:"TEAL_256" %lu\n", p5, (size_t)p5);
	printf(BLUE"p6:"TEAL_256" %p "BLUE"- decimal value:"TEAL_256" %lu\n", p6, (size_t)p6);
	printf("\n");
	printf(BLUE"Check alignment (p1 mod 16):"TEAL_256" %lu\n", (size_t)p1 % ALIGNMENT);
	printf(BLUE"Check alignment (p2 mod 16):"TEAL_256" %lu\n", (size_t)p2 % ALIGNMENT);
	printf(BLUE"Check alignment (p3 mod 16):"TEAL_256" %lu\n", (size_t)p3 % ALIGNMENT);
	printf(BLUE"Check alignment (p4 mod 16):"TEAL_256" %lu\n", (size_t)p4 % ALIGNMENT);
	printf(BLUE"Check alignment (p5 mod 16):"TEAL_256" %lu\n", (size_t)p5 % ALIGNMENT);
	printf(BLUE"Check alignment (p6 mod 16):"TEAL_256" %lu\n", (size_t)p6 % ALIGNMENT);
	printf("\n");
	printf(BLUE"Distance between p1 and p2:"TEAL_256" %lu\n", (p2 > p1) ? p2 - p1 : p1 - p2);
	printf(BLUE"Distance between p2 and p3:"TEAL_256" %lu\n", (p3 > p2) ? p3 - p2 : p2 - p3);
	printf(BLUE"Distance between p3 and p4:"TEAL_256" %lu\n", (p4 > p3) ? p4 - p3 : p3 - p4);
	printf(BLUE"Distance between p4 and p5:"TEAL_256" %lu\n", (p5 > p4) ? p5 - p4 : p4 - p5);
	printf(BLUE"Distance between p5 and p6:"TEAL_256" %lu\n", (p6 > p5) ? p6 - p5 : p5 - p6);
	printf(NC"\n");
	free(p1); free(p2); free(p3); free(p4); free(p5); free(p6);
	{
		printf(BLUE"Test: malloc(0)\n"NC);
		void *p7 = malloc(0);
		if (!p7)
			printf(GREEN"malloc(0) == NULL\n"NC);
		else
			printf(RED"malloc(0) returned %p (expected NULL)\n"NC, p7);
		printf("\n");
		free(p7);
	}
	{
		printf(BLUE"Test: malloc(MAX_ALLOC_SIZE)\n"NC);
		void *p8 = malloc(MAX_ALLOC_SIZE);
		if (!p8)
			printf(GREEN"malloc(MAX_ALLOC_SIZE) == NULL\n"NC);
		else
			printf(RED"malloc(MAX_ALLOC_SIZE) returned %p (expected NULL)\n"NC, p8);
		printf("\n");
		free(p8);
	}
	{
		printf(BLUE"Test: malloc(MAX_ALLOC_SIZE - overhead)\n"NC);
		size_t near_max = MAX_ALLOC_SIZE - ((HEADER_SIZE + ZONE_HEADER_SIZE) << 1);
		void *p9 = malloc(near_max);
		if (!p9)
			printf(GREEN"malloc(%lu) == NULL\n"NC, near_max);
		else
			printf(RED"malloc(%lu) returned %p (expected NULL)\n"NC, near_max, p9);
		printf(NC"\n");
		free(p9);
	}
}

static void test_free(void)
{
	printf(GOLD_TEXT"\n");
	printf("====================== Testing free() ======================\n");
	printf(NC"\n");
	
	{
		printf(BLUE"Test 1: Basic free and block reuse\n"NC);
		void *ptr1 = malloc(50);
		void *ptr2 = malloc(100);
	
		printf(BLUE"ptr1:"TEAL_256" %p\n", ptr1);
		printf(BLUE"ptr2:"TEAL_256" %p\n"NC, ptr2);
	
		free(ptr1);
		printf(GREEN"Freed ptr1\n"NC);
	
		void *ptr3 = malloc(30);
		printf(BLUE"ptr3:"TEAL_256" %p "BLUE"(should reuse ptr1's block)\n"NC, ptr3);
		
		if (ptr3 == ptr1)
			printf(GREEN"ptr3 correctly reused ptr1's block\n"NC);
		else
			printf(YELLOW"ptr3 did not reuse ptr1's block (might be normal if defrag occurred)\n"NC);
		printf("\n");
	
		free(ptr2);
		free(ptr3);
		printf(GREEN"Freed ptr2 and ptr3\n"NC);
		printf(NC"\n");
	}
	{
		printf(BLUE"Test 2: free(NULL)\n"NC);
		free(NULL);
		printf(GREEN"free(NULL) succeeded (no crash)\n"NC);
		printf(NC"\n");
	}
	{
		printf(BLUE"Test 3: Multiple allocations and frees\n"NC);
		void *ptr1 = malloc(20);
		void *ptr2 = malloc(40);
		void *ptr3 = malloc(60);
		
		printf(GREEN"Allocated 3 blocks\n"NC);
		
		free(ptr2);
		printf(GREEN"Freed middle block (ptr2)\n"NC);
		
		void *ptr4 = malloc(35);
		if (ptr4 == ptr2)
			printf(GREEN"New allocation reused freed block\n"NC);
		else
			printf(YELLOW"New allocation didn't reuse freed block\n"NC);
		
		free(ptr1);
		free(ptr3);
		free(ptr4);
		printf(GREEN"Freed all remaining blocks\n"NC);
		printf(NC"\n");
	}
}

static void test_realloc(void)
{
	printf(GOLD_TEXT"\n");
	printf("====================== Testing realloc() ======================\n");
	printf(NC"\n");
	printf(YELLOW"(realloc implementation pending)\n"NC);
	printf(NC"\n");
}

int main(void)
{
	printf(GOLD_TEXT"\n");
	printf(" ============================================================ \n");
	printf("||                                                          ||\n");
	printf("||                       MALLOC TESTS                       ||\n");
	printf("||                                                          ||\n");
	printf(" ============================================================ \n");
	printf(NC"\n");

	print_info();
	test_malloc();
	test_free();
	test_realloc();

	return 0;
}