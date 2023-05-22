#include "to_parts.h"
#include "literal_table.h"
#include "symbole_table.h"
#include "database.h"
#include "base_table.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define stop_if(condition, action , ...) {      \
            if (condition)                      \
            {                                   \
                fprintf(stderr, __VA_ARGS__ );  \
                fprintf(stderr, "\n");          \
                action;                         \
            }                                   \
    }

int main(int argc, char const *argv[])
{
    stop_if (argc == 1, return 1, "Usage : %s <filename>", argv[0]);

    FILE* fp = fopen (argv[1], "r");
    open_db("key_words.db");

    char buffer[256] = "";
    int address = 0;

    while ( fgets (buffer, 256, fp) )
    {
        if ( buffer[0] == '\n' )
            continue;

        ptr_line_string line_str = to_line_string ( strdup (buffer), " \t\n");

        if ( !is_key_word ("key_words", "name", line_str->elements[0]) &&
             !is_key_word ("psudo_word", "name", line_str->elements[0]) )
        {
            // START, DS, DC, EQU
                // add to symbol table
            if ( line_str->parts > 1 )
            {
                // START and EQU
                if ( strcmp ( line_str->elements[1], "START") == 0 ||
                     strcmp ( line_str->elements[1], "EQU") == 0 )
                {
                    if ( isdigit (line_str->elements[2][0]) )
                        add_to_symbole ( line_str->elements[0], atoi (line_str->elements[2]), 1, line_str->elements[1][0] == 'E' ? 'A' : 'R');
                    else if ( '*' == line_str->elements[2][0])
                        add_to_symbole ( line_str->elements[0], address, 1, line_str->elements[1][0] == 'E' ? 'A' : 'R');
                }
                // DS and DS
                else if ( strcmp (line_str->elements[1], "DS") == 0 ||
                          strcmp (line_str->elements[1], "DC") == 0)
                {
                    ptr_line_string temp_line_str = to_line_string (strdup (line_str->elements[2]), "F");

                    int length = 1;
                    if ( isdigit (line_str->elements[2][0]) )
                        length = atoi(temp_line_str->elements[0]);
                    
                    add_to_symbole (line_str->elements[0], address, 4, 'R');

                    ptr_line_string temp_line_str_2 = to_line_string(strdup(temp_line_str->elements[temp_line_str->parts - 1]), " ,");
                    address += length * (temp_line_str_2->parts > 0 ? temp_line_str_2->parts : 1) * 4;

                    free_string (temp_line_str_2);
                    free_string (temp_line_str);
                }
                // =F or =A
                    // add to literal
                else if ( is_key_word ( "key_words", "name", line_str->elements[1]) )
                {
                    ptr_line_string temp_line_str = to_line_string (strdup(buffer), " ,\t\n");
                    if (temp_line_str->parts == 4)
                    {
                        if ( '=' == temp_line_str->elements[3][0] )
                            add_to_literal(temp_line_str->elements[3], -1, 4, 'R');
                        else
                            add_to_symbole(line_str->elements[0], -1, 4, 'R');
                    }

                    address += 4;
                    free_string(temp_line_str);
                }
            }
        }
        else if ( is_key_word ( "key_words", "name", line_str->elements[0]) )
        {
            ptr_line_string temp_line_str = to_line_string(strdup(buffer), " ,\t\n");

            if ( temp_line_str->parts == 3 )
            {
                if ( '=' == temp_line_str->elements[2][0] )
                    add_to_literal(temp_line_str->elements[2], -1, 4, 'R');
            }
            address += 4;
            free_string(temp_line_str);
        }
        else if ( is_key_word ("psudo_word", "name", line_str->elements[0]) )
        {
            if ( strcmp (line_str->elements[0], "USING") == 0 )
            {
                ptr_line_string temp_line_str = to_line_string ( strdup (buffer), " ,\t\n");

                if ( isdigit (temp_line_str->elements[1][0]) )
                {
                    if ( isdigit (temp_line_str->elements[2][0]) )
                        add_to_base ( .reg = atoi (temp_line_str->elements[2]), .value = atoi (temp_line_str->elements[1]) );
                    else
                        add_to_base ( .reg_name = strdup (temp_line_str->elements[2]), .value = atoi (temp_line_str->elements[1]) );
                }
                else if ( temp_line_str->elements[1][0] == '*' )
                {
                    if ( isdigit (temp_line_str->elements[2][0]) )
                        add_to_base ( .reg = atoi (temp_line_str->elements[2]), .value = address);
                    else
                        add_to_base ( .reg_name = strdup (temp_line_str->elements[2]), .value = address );
                }
                else
                {
                    if ( isdigit ( temp_line_str->elements[2][0] ) )
                        add_to_base ( .reg = atoi (temp_line_str->elements[2]), .value_name = strdup (temp_line_str->elements[1]) );
                    else
                        add_to_base ( .reg_name = strdup (temp_line_str->elements[2]), .value_name = strdup (temp_line_str->elements[1]) );
                }

                free_string (temp_line_str);
            }
            else if ( strcmp (line_str->elements[0], "LTORG") == 0 )
            {
                realloc_address (&address);
                address += 4;
            }
        }
         
        free_string ( line_str );
        memset(buffer, 0, 256);
    }

    rewind (fp);

    while ( fgets ( buffer, 256, fp) )
    {
        if ( buffer[0] == '\n' )
            continue;

        ptr_line_string line_str = to_line_string ( strdup (buffer), " ,\t\n");

        if ( line_str->parts > 0)
        {
            if ( strcmp ( line_str->elements[0], "USING" ) == 0 )
            {
                if ( !isdigit (line_str->elements[1][0]) )
                    change_value ( line_str->elements[1] , value_of_symbole (line_str->elements[1]) );
                if ( !isdigit (line_str->elements[2][0]) )
                    change_register ( line_str->elements[2] , value_of_symbole (line_str->elements[2]) );
            }
            else if ( !is_key_word ("key_words", "name", line_str->elements[0]) )
            {
                if ( value_of_symbole ( line_str->elements[line_str->parts - 1]) != -1)
                    change_value_of ( line_str->elements[0], value_of_symbole(line_str->elements[line_str->parts - 1]));
                else if ( line_str->elements[line_str->parts - 1][0] != '=' )
                {
                    ptr_line_string temp_line_str = to_line_string ( strdup (line_str->elements[line_str->parts - 1]), " ()");
                    if (temp_line_str->parts == 2)
                        change_value_of ( line_str->elements[0], value_of_symbole(temp_line_str->elements[0])
                                                                + 4*value_of_symbole(temp_line_str->elements[1]));
                    free_string (temp_line_str);
                }
            }
        }

        free_string (line_str);
        memset(buffer, 0, 256);
    }

    printf ("%s\n", "symbol table");
    display_symbole ();
    printf ("\n%s\n", "literal table");
    display_literal ();
    printf ("\n%s\n", "base table");
    display_base ();

    free_symbole ();
    free_literal ();
    free_register();

    close_db();
    fclose(fp);
    return 0;
}
