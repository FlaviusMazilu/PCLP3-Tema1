#ifndef _STRUCTS_H_
#define _STRUCTS_H

typedef struct __attribute__ ((__packed__)) head {
	unsigned char type;
	unsigned int len;
} head;

typedef struct data_structure {
	head *header;
	void *data;
} data_structure;

#endif