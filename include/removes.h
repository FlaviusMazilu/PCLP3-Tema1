// Copyright 2022 Maria Sfiraiala & Flavius Mazilu (314CA)

#ifndef _REMOVES_H_
#define _REMOVES_H_

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
* @brief -> driver function for "delete_at", parses the necessary parameters
* @param -> buffer = the string from which we extract needed parameters
* @param -> arr = the array of bytes, stores multiple types of data
* @param -> len = the total number of data packages
* @param -> bytes = the total number of bytes stored in arr
* @return -> 0 for success, anything else for failure
*/
int delete(char *buffer, void **arr, int *len, int *bytes);

#endif
