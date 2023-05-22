#include "database.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static sqlite3* connection;

void open_db (char const* name)
{
    if ( sqlite3_open (name, &connection) != SQLITE_OK )
    {
        fprintf(stderr, "%s\n", sqlite3_errmsg(connection));
        exit(1);
    }
}

bool is_key_word (char const* table, char const* column, char const* name)
{
    bool result = false;
    if (connection)
    {
        sqlite3_stmt* query = NULL;

        char* tmp;
        asprintf (&tmp, "SELECT * FROM %s WHERE %s LIKE '%s'", table, column, name);
        // printf("--CMD : %s\n", tmp);

        if (sqlite3_prepare_v2 (connection, tmp, -1, &query, NULL) != SQLITE_OK )
        {
            fprintf(stderr, "%s : %s\n", "Error", sqlite3_errmsg(connection));
            sqlite3_finalize (query);
            free(tmp);
            return false;
        }

        while ( SQLITE_ROW == sqlite3_step (query) )
        {
            result = true;
            break;
        }

        sqlite3_finalize (query);
        free(tmp);
    }

    return result;
}

void close_db ()
{
    sqlite3_close (connection);
}

#ifdef __test_database

int main(int argc, char const *argv[])
{
    open_db("key_words.db");

    if (is_key_word("psudo_word", "name", "LTORG"))
        printf("%s\n", "USING is keyword");

    if (is_key_word("key_words", "name", "QWE"))
        printf("%s\n", "QWE is keyword");

    close_db();
    return 0;
}

#endif
