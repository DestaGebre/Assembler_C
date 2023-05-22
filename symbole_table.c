#define _GNU_SOURCE
#include "symbole_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct __symbole
{
    char* symbole_name;
    int value;
    int length;
    char re_alloc;
} symbole, *ptr_symbole;

typedef struct __symbole_table
{
    symbole** rows;
    int length;
} symbole_table, *ptr_symbole_table;

static ptr_symbole_table table = NULL;

static int len ()
{
    if (table)
        return table->length;
    return 0;
}

void add_to_symbole (char const* name, int value, int length, char re_alloc)
{
    if ( !table )
    {
        table = malloc (sizeof(symbole_table));
        memset (table, 0, sizeof(symbole_table));
    }

    table->rows = realloc (table->rows, sizeof(ptr_symbole_table)*++(table->length));
    table->rows[table->length - 1] = malloc (sizeof(symbole));
    memset ( table->rows[table->length - 1], 0, sizeof(symbole) );

    // *table->rows[table->length - 1] = (symbole){ .symbole_name = strdup(name), .address = address, .length = length, .value = value };
    table->rows[table->length - 1]->symbole_name = strdup (name);
    table->rows[table->length - 1]->value = value;
    table->rows[table->length - 1]->length = length;
    table->rows[table->length - 1]->re_alloc = re_alloc;
}

int value_of_symbole (char const* name)
{
    if ( table )
    {
        for (int i = 0; i < len (); ++i)
            if ( strcmp (table->rows[i]->symbole_name, name) == 0 )
                return table->rows[i]->value;
    }

    return -1;
}

void change_value_of (char const* name, int value)
{
    if ( table )
    {
        for (int i = 0; i < len (); ++i)
            if ( strcmp (table->rows[i]->symbole_name, name) == 0 )
                table->rows[i]->value = value;
    }	
} 

void display_symbole ()
{
    if ( table )
    {
        printf ("%-10s\t%10s\t%10s\t%10s\n", "name", "value", "length", "re_alloc");
        for (int i = 0; i < table->length; ++i)
        {
            printf("%-10s\t%10d\t%10d\t%10c\n", table->rows[i]->symbole_name, table->rows[i]->value,
                                     table->rows[i]->length, table->rows[i]->re_alloc);
        }
    }
}

void free_symbole ()
{
    if ( table )
    {
        for (int i = 0; i < table->length; ++i)
        {
            free (table->rows[i]->symbole_name);
            free (table->rows[i]);
        }
        free (table->rows);
        free (table);
    }
}


#ifdef __test_symbole

int main (int argc, char const *argv[])
{
    add_to_symbole ("tom", 4, 5, 'R');
    add_to_symbole ("tad", 4, 10, 'R');

    printf (" value of tom : %d\n", value_of("tom"));
    printf (" value of tad : %d\n", value_of("tad"));

    display_symbole ();

    free_symbole ();
    return 0;
}

#endif
