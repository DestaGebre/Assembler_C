// definition of to_part 

#include "to_parts.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ptr_line_string to_line_string (char* str, char const* delimeter)
{
	char* scratch = NULL;
	ptr_line_string line_str = malloc (sizeof (line_string));
	memset(line_str, 0, sizeof(line_string));

	line_str->str = str;

	char* txt = strtok_r (line_str->str, delimeter, &scratch);
	
	if (!txt ) return NULL;

	while (txt)
	{
		line_str->elements = realloc (line_str->elements, sizeof(char*)*(++(line_str->parts)));
		line_str->elements[line_str->parts - 1] = txt;
		txt = strtok_r(NULL, delimeter, &scratch);
	}

	return line_str;
}

char* at (ptr_line_string line_str, int pos)
{
	if (pos >= line_str->parts || pos < 0)
	{
		fprintf(stderr, "%s max = %d specified = %d\n", "position out of range", line_str->parts, pos);
		exit (1);
	}

	return line_str->elements[pos];
}

int length (ptr_line_string line_str)
{
	if (line_str)
		return line_str->parts;
	return 0;
}

void free_string (ptr_line_string line_str)
{
	if (line_str)
	{
		free (line_str->elements);
		free (line_str->str);
		free (line_str);
	}
}


#ifdef __test_to_part

int main(int argc, char const *argv[])
{
	// ptr_line_string line_str = to_line_string ("dont judge the day by the harvest you reap"
											   // " but rather by the seed you plant.", " ");
	FILE* fp = fopen("to_parts.c", "r");

	char buffer[256];
	
	ptr_line_string line_str;

	while (fgets(buffer, 256, fp))
	{
		line_str = to_line_string ( strdup (buffer), " \t");
		printf("%s\n", line_str->elements[0]);
		free_string(line_str);
	}

	// free_string(line_str);
	// for (int i = 0; i < len (line_str) - 1; ++i)
		// printf("%s\t%s\n", at (line_str, i), at (line_str, i + 1));
	fclose(fp);
	return 0;
}

#endif
