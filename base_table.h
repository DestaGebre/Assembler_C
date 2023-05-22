#pragma once

typedef struct __base
{	
	struct
	{
		int reg;
		char* reg_name;
	};
	struct
	{
		int value;
		char* value_name;
	};
} base, *ptr_base;

#define add_to_base(...) _add_to_base( (base){ __VA_ARGS__ } )

void _add_to_base ( base row );

void display_base ();

void change_register ( char const* name, int reg);
void change_value ( char const* name, int value );

void drop ();

void free_register ();
