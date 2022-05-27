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
