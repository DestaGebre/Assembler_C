/*
 * this module is to_parts.
 * what it do is it takes a string a separate it into parts
 * using given delimiter(separator) like space,tab, words or other as we want.
*/
#pragma once

typedef struct __string
{
	char** elements;
	char* str;
	int parts;
} line_string, *ptr_line_string;

ptr_line_string to_line_string (char* str, char const* delimeter);

int length (ptr_line_string line_str);

/**
 * panics when the pos is out of range
 */
char* at (ptr_line_string line_str, int pos);

void free_string (ptr_line_string line_str);
