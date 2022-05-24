// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include "structs.h"

#define STRING_MAX_LEN 256

#define DIE(assertion, call_description)                                       \
    do {                                                                       \
        if (assertion) {                                                       \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
            perror(call_description);                                          \
            exit(errno);                                                       \
        }                                                                      \
    } while (0)

/*
* @brief -> determines the length of the array for index packages of data
* @param -> arr = the array for which we get the size
* @param -> index = the package index up until we determine the size
* @return -> the number of bytes up until the index-th data package
*/
int get_size(void *arr, int index);

/*
* @brief -> adds a data package starting with the last position of the array
* @param -> arr = the array which contains the data
* @param -> len = the total number of packages
* @param -> data = the package we'd like to have inserted
* @return -> 0 for succes, anything else for failure
*/
int add_last(void **arr, int *len, data_structure *data);

/*
* @brief -> adds a data package starting with the "index" position of the array
* @param -> arr = the array which contains the data
* @param -> len = the total number of packages
* @param -> data = the package we'd like to have inserted
* @param -> index = the package position starting with which we'll insert the
*			new data
* @return -> 0 for succes, anything else for failure
*/
int add_at(void **arr, int *len, data_structure *data, int index);

/*
* @brief -> finds the index-th data package from an array of bytes and prints
*			it
* @param -> data_block = the array of bytes on which we perform the search
* @param -> len = the total number of data packages contained by the array
* @param -> index = the position of the package we want to find
* return -> none 
*/
void find(void *data_block, int len, int index);

/*
* @brief -> deletes a data package starting with the "index" position of the
*			array
* @param -> arr = the array which contains the data
* @param -> len = the total number of packages
* @param -> data = the package we'd like to have deleted
* @param -> index = the package position starting with which we'll delete the
*			data
* @return -> 0 for succes, anything else for failure
*/
int delete_at(void **arr, int *len, int index);

/*
* @brief -> prints all the data contained by the array
* @param -> arr = the array we'll print
* @param -> the number of data packages contained by arr
* @return -> none
*/
void print(void *arr, int len);

/*
* @brief -> initializes the header with the given values
* @param -> h = the header we'll initialize
* @param -> t = the type of the header, used for its first field
* @param -> l = the total length of the data that we'll insert later on, used
*			for header's second field
* @return -> none
*/
void init_header(head *h, unsigned char t, unsigned int l);

/*
* @ brief -> adds 2 strings and 2 "ints" of different sizes to the same array,
*			 in a compact manner
* @ param -> arr = the polymorphic array
* @ param -> person1 = the first string; it represents a name
* @ param -> person2 = the second string; it represents a name
* @ param -> bill1 = the first "int", it represents a bill
* @ param -> bill2 = the second "int", it representa a bill
* @ return -> none
*/
void add_to_array(void **arr, char *person1, char *person2, void *bill1, void
				 *bill2, int type);

/*
* @brief -> driver function for "add_last", parses the necessary parameters
* @param -> buffer = the string from which we extract needed parameters
* @param -> arr = the array of bytes, stores multiple types of data
* @param -> len = the total number of data packages
* @param -> bytes = the total number of bytes stored in arr
* @return -> 0 for success, anything else for failure
*/
int insert(char *buffer, void **arr, int *len, int *bytes);

/*
* @brief -> driver function for "add_at", parses the necessary parameters
* @param -> buffer = the string from which we extract needed parameters
* @param -> arr = the array of bytes, stores multiple types of data
* @param -> len = the total number of data packages
* @param -> bytes = the total number of bytes stored in arr
* @return -> 0 for success, anything else for failure
*/
int insert_at(char *buffer, void **arr, int *len, int *bytes);

/*
* @brief -> driver function for "find", parses the necessary parameters
* @param -> buffer = the string from which we extract needed parameters
* @param -> arr = the array of bytes, stores multiple types of data
* @param -> len = the total number of data packages
* @return -> 0 for success, anything else for failure
*/
int find_index(char *buffer, void *arr, int *len);

/*
* @brief -> driver function for "delete_at", parses the necessary parameters
* @param -> buffer = the string from which we extract needed parameters
* @param -> arr = the array of bytes, stores multiple types of data
* @param -> len = the total number of data packages
* @param -> bytes = the total number of bytes stored in arr
* @return -> 0 for success, anything else for failure
*/
int delete(char *buffer, void **arr, int *len, int *bytes);

/*
* @brief -> determines the operation from an entire line of input
* @param -> buffer = line read from stdin
* @param -> op = stores the magic word for the operation
* @return -> none
*/
void get_op(char *buffer, char *op);

/*
* @brief -> executes every operation
* @param -> buffer = the rest of the input, without the op magic word
* @param -> op = operation magic word
* @param -> arr = the array of bytes
* @param -> len = the total number of packages
* @param -> bytes = the total number of bytes stored in arr
* @return -> 0 for success, anything else for failure
*/
int apply_op(char *buffer, char *op, void **arr, int *len, int *bytes);

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

int delete_at(void **arr, int *len, int index)
{
	if (index == *len - 1)
		return 0;
	int len_to_index = get_size(*arr, index);
	int len_to_next = get_size(*arr, index + 1);
	int total_len = get_size(*arr, *len);

	// ignoring the part that we want to be deleted when doing memmove so it's
	// not included in the array anymore
	memmove(*arr + len_to_index, *arr + len_to_next, total_len - len_to_next);

	return 0;
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

int delete(char *buffer, void **arr, int *len, int *bytes)
{
	int index;
	sscanf(buffer, "%d", &index);

	int len_to_index = get_size(*arr, index);
	head *to_delete = (head *)(*arr + len_to_index);

	// recalculating the number of bytes considering the fact that soon we'll 
	// delete a package of data
	*bytes -= (to_delete->len + sizeof(*to_delete));

	int ret = delete_at(arr, len, index);
	if (!ret) {
		void *tmp = realloc(*arr, *bytes);
		if (!tmp && *bytes) {
			fprintf(stderr, "realloc() failed\n");
			return -1;
		}
		*arr = tmp;
		--(*len);
	}

	return ret;
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

int main(void)
{
	void *arr = NULL; // the polymorphic array
	int len = 0, bytes = 0; // the number of data packages, the number of bytes

	char *buffer = (char *)malloc(STRING_MAX_LEN * sizeof(*buffer));
	DIE(!buffer, "malloc() failed\n");

	char *op = (char *)malloc(STRING_MAX_LEN * sizeof(*op));
	DIE(!op, "malloc() failed\n");

	int mem_ups;
	// we execute operations as long as we aren't specifically told to stop
	// using the exit command
	do {
		// reading from stdin, line by line
		fgets(buffer, STRING_MAX_LEN, stdin);
		// if the line is empty => garbage
		if (!strcmp(buffer, "\n"))
			strcpy(buffer, "garbage");

		// determining the operation magic word
		get_op(buffer, op);
	
		mem_ups = apply_op(buffer, op, &arr, &len, &bytes);
		// in case we find problems allocating space, we use mem_ups as a guard
		if (mem_ups == -1)
			exit(-1);
	} while (strcmp(op, "exit"));

	free(buffer);
	free(op);
	free(arr);

	return 0;
}
