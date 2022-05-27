// Copyright 2022 Maria Sfiraiala & Flavius Mazilu (314CA)

#ifndef _UTILS_H_
#define _UTILS_H_

/*
* @brief -> determines the length of the array for index packages of data
* @param -> arr = the array for which we get the size
* @param -> index = the package index up until we determine the size
* @return -> the number of bytes up until the index-th data package
*/
int get_size(void *arr, int index);

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
* @brief -> driver function for "find", parses the necessary parameters
* @param -> buffer = the string from which we extract needed parameters
* @param -> arr = the array of bytes, stores multiple types of data
* @param -> len = the total number of data packages
* @return -> 0 for success, anything else for failure
*/
int find_index(char *buffer, void *arr, int *len);

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


#define DIE(assertion, call_description)                                       \
    do {                                                                       \
        if (assertion) {                                                       \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
            perror(call_description);                                          \
            exit(errno);                                                       \
        }                                                                      \
    } while (0)

#endif