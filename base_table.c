#include "base_table.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __table
{
	ptr_base* rows;
	int length;
	_Bool droped;
} base_table, *ptr_base_table;

static ptr_base_table table = NULL;

static int len ()
{
	if (table)
		return table->length;
	return 0;
}


void _add_to_base ( base row )
{
	if (!table)
	{
		table = malloc (sizeof(base_table));
		memset (table, 0, sizeof(base_table));
	}

	table->rows = realloc (table->rows, (sizeof(ptr_base))*++(table->length) );
	table->rows[len () - 1] = malloc(sizeof(base));
	memset(table->rows[len () - 1], 0, sizeof(base));

	*table->rows[len () - 1] = row;
}

void display_base ()
{
	if (table)
	{
		printf("%-20s\t%20s\n", "base register", "register value");
		for (int i = 0; i < len(); ++i)
		{
			if (table->rows[i]->reg_name && strlen (table->rows[i]->reg_name) > 0)
				printf("%-20s\t", table->rows[i]->reg_name );
			else
				printf("%-20d\t", table->rows[i]->reg );

			if (table->rows[i]->value_name && strlen (table->rows[i]->value_name) > 0)
				printf("%20s\n", table->rows[i]->value_name );
			else
				printf("%20d\n", table->rows[i]->value );
		}

		if (table->droped)
			printf("-----%s------\n", "Register Droped");
	}
}

void change_register ( char const* name, int reg)
{
	if (table)
	{
		for (int i = 0; i < len (); ++i)
		{
			if ( table->rows[i]->reg_name && ( strcmp (table->rows[i]->reg_name, name) == 0 ) )
			{
				free (table->rows[i]->reg_name);
				table->rows[i]->reg_name = NULL;
				table->rows[i]->reg = reg;
			}
		}
	}
}

void change_value ( char const* name, int value )
{
	if (table)
	{
		for (int i = 0; i < len (); ++i)
		{
			if ( table->rows[i]->value_name && strcmp (table->rows[i]->value_name, name) == 0 )
			{
				free ( table->rows[i]->value_name );
				table->rows[i]->value_name = NULL;
				table->rows[i]->value = value;
			}
		}
	}
}

void droped ()
{
	if (table)
		table->droped = 1;
}

void free_register ()
{
	if (table)
	{
		for (int i = 0; i < len(); ++i)
		{
			!table->rows[i]->reg_name ? : free (table->rows[i]->reg_name);
			!table->rows[i]->value_name ? : free (table->rows[i]->value_name);
			free ( table->rows[i] );
		}

		free ( table->rows );
		free ( table );
	}
}

#ifdef __test_base

int main(int argc, char const *argv[])
{
	add_to_base ( .reg = 5, .value = 6);
	add_to_base ( .reg_name = strdup ("tom"), .value = 6);

	display_base ();

	change_register ( "tom", 12);
	display_base ();

	free_register ();
	return 0;
}

#endif