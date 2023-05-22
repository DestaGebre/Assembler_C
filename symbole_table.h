#pragma once

void add_to_symbole (char const* name, int value, int length, char re_alloc);

void change_value_of (char const* name, int value);
int value_of_symbole (char const* name);
void display_symbole ();

void free_symbole ();
