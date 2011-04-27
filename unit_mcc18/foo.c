
/* This is a Foo class implementation file
 */

#include "foo.h"
#include "implement/foo.h"

#include "../libs/exception.h"

/** @class Foo
 *  @brief Foo class - brief description.
 * 	@see foo.h
 */

/** @file foo.h 
 *  @brief Foo class - brief description.
 * Foo is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( Foo, Base );

/* Class virtual function prototypes
 */

static
void
_foo_virtual( Foo self )
{
	return;
}


/* Class initializing
 */

static
void
Foo_initialize( Class this )
{
	FooVtable vtab = (FooVtable) this->vtable;
	
	vtab->foo_virtual	= 	_foo_virtual;

	// ooc_init_class( !!! Classes that are USED by Foo !!! ); Remove if you initialize it other place!
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

static
void
Foo_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
Foo_constructor( Foo self, const void * params )
{
	assert( ooc_isInitialized( Foo ) );
	
	chain_constructor( Foo, self, NULL );

}

/* Destructor
 */

static
void
Foo_destructor( Foo self, FooVtable vtab )
{
#ifndef OOC_NO_DYNAMIC_MEM
	ooc_free_and_null( (void**) & self->text );
#else
	self->text = NULL;
#endif
}

/* Copy constuctor
 */

static
int
Foo_copy( Foo self, const Foo from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


#ifndef OOC_NO_DYNAMIC_MEM

Foo
foo_new( void )
{
	ooc_init_class( Foo );
		
	return (Foo) ooc_new( Foo, NULL );
}

Foo
foo_new_with_data( int data )
{
	Foo foo = foo_new();
	
	foo_set_data( foo, data );
	
	return foo;
}

Foo
foo_new_with_text( char * text )
{
	Foo foo;
	
	ooc_manage( text, (ooc_destroyer) ooc_free );
	
	foo = foo_new();
	
	foo_add_text( foo, ooc_pass( text ) );
	
	return foo;
}

Foo
foo_new_with_const_text( const char * text )
{
	Foo foo = foo_new();
	
	{
	ooc_manage_object( foo );
	foo_add_text( foo, ooc_strdup( text ) );
	return ooc_pass( foo );
	}
}

#else

Foo
foo_use( void * mem )
{
	ooc_init_class( Foo );
		
	ooc_use( mem, Foo, NULL );

	return mem; 
}

Foo
foo_use_with_data( void * mem, int data )
{
	foo_use( mem );
	
	foo_set_data( (Foo) mem, data );
	
	return (Foo) mem;
}

Foo
foo_use_with_text( void * mem, char * text )
{
	foo_use( mem );
	
	foo_add_text( (Foo) mem, text );
}

#endif


void
foo_set_data( Foo self, int data )
{
	self->data = data;
}

int
foo_get_data( Foo self )
{
	return self->data;
}

void
foo_add_text( Foo self, char * text )
{
#ifndef OOC_NO_DYNAMIC_MEM
	ooc_free( self->text );
#endif
	self->text = text;	
}

const char *
foo_get_text( Foo self )
{
	return self->text;
}
