#pragma once

#ifndef __cplusplus
	#ifndef bool
		#define bool _Bool
		#define true 1
		#define false 0
	#endif
#endif

void open_db (char const* name);

bool is_key_word (char const* table, char const* column, char const* name);

void close_db ();