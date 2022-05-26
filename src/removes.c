#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

#include "../include/structs.h"
#include "../include/utils.h"
#include "../include/inserts.h"
#include "../include/removes.h"

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