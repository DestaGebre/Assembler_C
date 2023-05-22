#pragma once

#ifndef __cplusplus
	#define bool _Bool
	#ifndef true
		#define true 1
		#define false 0
	#endif 
#endif


void add_to_literal (char const* name, int address, int length, char re_alloc);

void display_literal ();

bool is_there (char const* literal_name);
void realloc_address (int* address);
int value_of_literal (char const* literal_name);

void free_literal ();