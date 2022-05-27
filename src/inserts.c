// Copyright 2022 Maria Sfiraiala & Flavius Mazilu (314CA)

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


int add_last(void **arr, int *len, data_structure *data)
{
	// determining arr's total length
	int total_len = get_size(*arr, *len);

	// we first transfer the header and after adding its length we'all also add
	// the data

	memcpy(*arr + total_len, data->header, sizeof(head));
	total_len += sizeof(head);
	memcpy(*arr + total_len, data->data, data->header->len);

	// incrementing the number of packages
	++(*len);

	return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
	// we define 2 corner cases: one for when the index is greater than the
	// number of data packages => we'll insert the data at the last position
	// available
	if (index >= *len)
		return add_last(arr, len, data);
	// if the index is smaller than 0 => error
	else if (index < 0)
		return -1;

	// getting the total number of bytes and the number of bytes until the
	// index
	int total_len = get_size(*arr, *len);
	int index_len = get_size(*arr, index);

	// we'll make space for a swap zone
	int swap_len = total_len - index_len;
	char *swap = malloc(swap_len * sizeof(*swap));
	DIE(!swap, "malloc() failed\n");

	// the first part of the array goes into swap
	memcpy(swap, *arr + index_len, swap_len);

	// inserting the values
	memcpy(*arr + index_len, data->header, sizeof(head));
	index_len += sizeof(head);
	memcpy(*arr + index_len, data->data, data->header->len);
	index_len += data->header->len;

	// copying the swap area at the back of the array
	memcpy(*arr + index_len, swap, swap_len);

	++(*len);
	free(swap);

	return 0;
}

void add_to_array(void **arr, char *person1, char *person2, void *bill1,
				 void *bill2, int type)
{
	// getting the size of the each bill according to header's type
	int size_bill1, size_bill2;
	int size_int8 = sizeof(int8_t);
	int size_int16 = sizeof(int16_t);
	int size_int32 = sizeof(int32_t);
	
	if (type == 1) {
		size_bill1 = size_int8;
		size_bill2 = size_int8;
	}
	if (type == 2) {
		size_bill1 = size_int16;
		size_bill2 = size_int32;
	}
	if (type == 3) {
		size_bill1 = size_int32;
		size_bill2 = size_int32;
	}

	// making space for the new data in the polymorphic array
	(*arr) = malloc(strlen(person1) + strlen(person2) + size_bill1 + size_bill2
				   + 2);

	// transfering the data into the bytes array
	memcpy(*arr, person1, strlen(person1) + 1);
	memcpy(*arr + strlen(person1) + 1, bill1, size_bill1);
	memcpy(*arr + strlen(person1) + size_bill1 + 1, bill2, size_bill2);
	memcpy(*arr + strlen(person1) + size_bill1 + size_bill2 + 1, person2,
		  strlen(person2) + 1);
}

int insert(char *buffer, void **arr, int *len, int *bytes)
{
	unsigned char type_of_header;

	char *string1 = (char *)malloc(STRING_MAX_LEN * sizeof(char));
	DIE(!string1, "malloc() failed\n");
	char *string2 = (char *)malloc(STRING_MAX_LEN * sizeof(char));
	DIE(!string2, "malloc() failed\n");

	char *value1 = (char *)malloc(STRING_MAX_LEN * sizeof(char));
	DIE(!value1, "malloc() failed\n");
	char *value2 = (char *)malloc(STRING_MAX_LEN * sizeof(char));
	DIE(!value2, "malloc() failed\n");

	// from the buffer we read the type of header, the 2 names and the 2 ints
	// (as strings as well)
	sscanf(buffer, "%hhu %s %s %s %s", &type_of_header, string1, value1,
		  value2, string2);

	// allocating just enough space for the names
	char *person1 = strdup(string1), *person2 = strdup(string2);
	free(string1);
	free(string2);

	// reserving memory for the structures
	head *h = malloc(sizeof(*h));
	DIE(!h, "malloc() failed\n");
	data_structure *data_info = malloc(sizeof(*data_info));
	DIE(!data_info, "malloc() failed\n");

	// for each type of int we'll transform the strings using atoi, then we'll
	// cast the result to the desired length
	if (type_of_header == 1) {
		int8_t bill1 = atoi(value1), bill2 = atoi(value2);
	
		init_header(h, type_of_header, 2 * sizeof(bill1) + strlen(person1)
				   + strlen(person2) + 2);

		add_to_array(&data_info->data, person1, person2, &bill1, &bill2, 1);
	}


	else if (type_of_header == 2) {
		int16_t bill1 = atoi(value1);
		int32_t bill2 = atoi(value2);
	
		init_header(h, type_of_header, sizeof(bill1) + sizeof(bill2) +
				   strlen(person1) + strlen(person2) + 2);

		add_to_array(&data_info->data, person1, person2, &bill1, &bill2, 2);
	}

	else {
		int32_t bill1 = atoi(value1), bill2 = atoi(value2);
	
		init_header(h, type_of_header, 2 * sizeof(bill1) + strlen(person1)
				   + strlen(person2) + 2);

		add_to_array(&data_info->data, person1, person2, &bill1, &bill2, 3);
	}

	data_info->header = h;

	// the total number of bytes contained by the array rises
	*bytes += sizeof(head) + data_info->header->len;

	// reallocating the array according to the newly found dimension
	void *tmp = realloc(*arr, *bytes);
	if (!tmp && *bytes) {
		fprintf(stderr, "realloc() failed\n");
		return -1;
	}
	*arr = tmp;

	add_last(arr, len, data_info);

	free(person1);
	free(person2);

	free(value1);
	free(value2);

	free(h);
	free(data_info->data);
	free(data_info);

	return 0;
}

int insert_at(char *buffer, void **arr, int *len, int *bytes)
{
	unsigned char type_of_header;
	int index;

	char *string1 = (char *)malloc(STRING_MAX_LEN * sizeof(char));
	DIE(!string1, "malloc() failed\n");
	char *string2 = (char *)malloc(STRING_MAX_LEN * sizeof(char));
	DIE(!string2, "malloc() failed\n");

	char *value1 = (char *)malloc(STRING_MAX_LEN * sizeof(char));
	DIE(!value1, "malloc() failed\n");
	char *value2 = (char *)malloc(STRING_MAX_LEN * sizeof(char));
	DIE(!value2, "malloc() failed\n");

	sscanf(buffer, "%d %hhu %s %s %s %s", &index, &type_of_header, string1,
		  value1, value2, string2);

	char *person1 = strdup(string1), *person2 = strdup(string2);
	free(string1);
	free(string2);

	head *h = malloc(sizeof(*h));
	DIE(!h, "malloc() failed\n");
	data_structure *data_info = malloc(sizeof(*data_info));
	DIE(!data_info, "malloc() failed\n");

	if (type_of_header == 1) {
		int8_t bill1 = atoi(value1), bill2 = atoi(value2);
	
		init_header(h, type_of_header, 2 * sizeof(bill1) + strlen(person1) +
				   strlen(person2) + 2);

		add_to_array(&data_info->data, person1, person2, &bill1, &bill2, 1);
	}


	else if (type_of_header == 2) {
		int16_t bill1 = atoi(value1);
		int32_t bill2 = atoi(value2);
	
		init_header(h, type_of_header, sizeof(bill1) + sizeof(bill2) +
				   strlen(person1) + strlen(person2) + 2);

		add_to_array(&data_info->data, person1, person2, &bill1, &bill2, 2);
	}

	else {
		int32_t bill1 = atoi(value1), bill2 = atoi(value2);
	
		init_header(h, type_of_header, 2 * sizeof(bill1) + strlen(person1) +
				   strlen(person2) + 2);

		add_to_array(&data_info->data, person1, person2, &bill1, &bill2, 3);
	}

	data_info->header = h;

	*bytes += sizeof(head) + data_info->header->len;

	void *tmp = realloc(*arr, *bytes);
	if (!tmp && *bytes) {
		fprintf(stderr, "realloc() failed\n");
		return -1;
	}
	*arr = tmp;

	add_at(arr, len, data_info, index);

	free(person1);
	free(person2);

	free(value1);
	free(value2);

	free(h);
	free(data_info->data);
	free(data_info);

	return 0;
}
