
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "literal_table.h"

typedef struct __literal
{
	char* literal_name;
	int address;
	int length;
	char re_alloc;
} literal, *ptr_literal;

typedef struct __literal_table
{
	literal** rows;
	int length;
} literal_table, *ptr_literal_table;

static ptr_literal_table table = NULL;

void add_to_literal (char const* name, int address, int length, char re_alloc)
{
	if (!table)
	{
		table = malloc (sizeof (literal_table));
		memset (table, 0, sizeof (literal_table));
	}

	table->rows = realloc (table->rows, sizeof (literal*)*(++(table->length)) );
	table->rows[table->length - 1] = malloc (sizeof(literal));
	memset (table->rows[table->length - 1], 0, sizeof (literal_table));

	*table->rows[table->length - 1] = (literal){ .literal_name = strdup (name), .address = address,
									   .length = length, .re_alloc = re_alloc };
}

void realloc_address (int* address)
{
	if (table)
	{
		for (int i = 0; i < table->length; ++i)
		{
			if (table->rows[i]->address == -1)
			{
				table->rows[i]->address = *address;
				*address += 4;
			}
		}
	}
}


void display_literal ()
{
	if (table)
	{
		printf ("%-10s\t%10s\t%10s\t%10s\n", "name", "value", "length", "realloc");
		for (int i = 0; i < table->length; ++i)
			printf ("%-10s\t%10d\t%10d\t%10c\n", table->rows[i]->literal_name, table->rows[i]->address
									 , table->rows[i]->length, table->rows[i]->re_alloc);
	}
}

bool is_there (char const* literal_name)
{
	if (table)
		for (int i = 0; i < table->length; ++i)
			if ( strcmp (table->rows[i]->literal_name, literal_name) == 0 )
				return true;

	return false;
}

int value_of_literal (char const* literal_name)
{
	if (table)
	{
		for (int i = 0; i < table->length; ++i)
			if ( strcmp (table->rows[i]->literal_name, literal_name) == 0 )
				return table->rows[i]->address;
	}

	return -1;
}

void free_literal ()
{
	if (table)
	{
		for (int i = 0; i < table->length; ++i)
		{
			free ( table->rows[i]->literal_name );
			free ( table->rows[i] );
		}
		free ( table->rows );
		free ( table );
	}
}


#ifdef __test_literal__

int main(int argc, char const *argv[])
{
	add_to_literal ("tom", 5, 4, 'R');
	add_to_literal ("tad", 5, 4, 'R');
	add_to_literal ("abe", 5, 4, 'R');
	add_to_literal ("temi", 5, 4, 'R');
	add_to_literal ("kinde", 5, 4, 'R');

	printf("value of tom : %d\n", value_of_literal ("tom"));

	display_literal ();

	free_literal ();
	return 0;
}

#endif 