// Copyright 2022 Maria Sfiraiala & Flavius Mazilu (314CA)

#ifndef _INSERTS_H_
#define _INSERTS_H_

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

#endif
