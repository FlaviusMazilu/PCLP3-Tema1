#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

#include "../include/structs.h"
#include "../include/utils.h"
#include "../include/inserts.h"
#include "../include/removes.h"

#define STRING_MAX_LEN 256


int get_size(void *arr, int index)
{
	// startig with a pointer that indicates arr's first position
	void *tmp = arr;
	int len = 0; // the total length, we'll return it later
	
	int size_uchar = sizeof(unsigned char);
	int size_uint = sizeof(unsigned int);
	int size_head = sizeof(head);

	register int i = 0;
	for (i = 0; i < index; ++i) {
		tmp += size_uchar;
		len += *(unsigned int *)tmp + size_head;
		tmp += size_uint + *(unsigned int *)tmp;
	}

	return len;
}



void find(void *data_block, int len, int index) 
{
	// getting the number of bytes until the data package positioned at index
	int len_to_index = get_size(data_block, index);

	// tmp points at the start of the data block we're interested in
	void *tmp = data_block + len_to_index;

	// getting the type of the header and printing it
	unsigned char type = *(unsigned char *)(tmp);
	printf("Tipul %hhu\n", type);

	// we skip the header now, we're interested in the stored data
	tmp += sizeof(head);
	
	// printing the first name
	printf("%s pentru ", (char *)tmp);
	tmp += strlen((char *)tmp) + 1;

	// printing the second name and the bills according to each type of header
	if (type == 1) {
		int8_t bill1 = *(int8_t *)tmp;
		tmp += sizeof(bill1);

		int8_t bill2 = *(int8_t *)tmp;
		tmp += sizeof(bill2);

		printf("%s\n", (char *)tmp);
		printf("%"PRId8"\n%"PRId8"\n", bill1, bill2);
	}

	if (type == 2) {
		int16_t bill1 = *(int16_t *)tmp;
		tmp += sizeof(bill1);

		int32_t bill2 = *(int32_t *)tmp;
		tmp += sizeof(bill2);

		printf("%s\n", (char *)tmp);
		printf("%"PRId16"\n%"PRId32"\n", bill1, bill2);
	}

	if (type == 3) {
		int32_t bill1 = *(int32_t *)tmp;
		tmp += sizeof(bill1);

		int32_t bill2 = *(int32_t *)tmp;
		tmp += sizeof(bill2);

		printf("%s\n", (char *)tmp);
		printf("%"PRId32"\n%"PRId32"\n", bill1, bill2);
	}

	tmp += strlen((char *)tmp) + 1;

	printf("\n");
}



void print(void *arr, int len)
{
	void *tmp = arr;
	int size_head = sizeof(head);
	register int i = 0;
	for (i = 0; i < len; ++i) {
		unsigned char type = *(unsigned char *)(tmp);
		printf("Tipul %hhu\n", type);

		tmp += size_head;
	
		printf("%s pentru ", (char *)tmp);
		tmp += strlen((char *)tmp) + 1;
	
		if (type == 1) {
			int8_t bill1 = *(int8_t *)tmp;
			tmp += sizeof(bill1);

			int8_t bill2 = *(int8_t *)tmp;
			tmp += sizeof(bill2);

			printf("%s\n", (char *)tmp);
			printf("%"PRId8"\n%"PRId8"\n", bill1, bill2);
		}

		if (type == 2) {
			int16_t bill1 = *(int16_t *)tmp;
			tmp += sizeof(bill1);

			int32_t bill2 = *(int32_t *)tmp;
			tmp += sizeof(bill2);

			printf("%s\n", (char *)tmp);
			printf("%"PRId16"\n%"PRId32"\n", bill1, bill2);
		}

		if (type == 3) {
			int32_t bill1 = *(int32_t *)tmp;
			tmp += sizeof(bill1);

			int32_t bill2 = *(int32_t *)tmp;
			tmp += sizeof(bill2);

			printf("%s\n", (char *)tmp);
			printf("%"PRId32"\n%"PRId32"\n", bill1, bill2);
		}

		tmp += strlen((char *)tmp) + 1;

		printf("\n");
	}

}

void init_header(head *h, unsigned char t, unsigned int l)
{
	h->type = t;
	h->len = l;
}

int find_index(char *buffer, void *arr, int *len)
{
	int index;
	sscanf(buffer, "%d", &index);

	// if the index is out of bounds, we'll return -1
	if (index < 0 || index >= *len)
		return -1;

	find(arr, *len, index);

	return 0;
}

void get_op(char *buffer, char *op)
{
	char *tmp = malloc(STRING_MAX_LEN * sizeof(*tmp));
	DIE(!tmp, "malloc() failed\n");

	strcpy(tmp, buffer);
	strcpy(op, strtok(tmp, "\n "));

	strcpy(tmp, buffer + strlen(op) + 1);
	strcpy(buffer, tmp);

	free(tmp);
}

int apply_op(char *buffer, char *op, void **arr, int *len, int *bytes)
{
	if (!strcmp(op, "insert"))
		return insert(buffer, arr, len, bytes);
	if (!strcmp(op, "print"))
		print(*arr, *len);
	if (!strcmp(op, "insert_at"))
		return insert_at(buffer, arr, len, bytes);
	if (!strcmp(op, "find"))
		return find_index(buffer, *arr, len);
	if (!strcmp(op, "delete_at"))
		return delete(buffer, arr, len, bytes);
	return 0;
}
