
/* This is a Hello class implementation file
 */
#include <stdio.h>

#include "hello.h"
#include "implement/hello.h"

#include <libs/exception.h>

/** @class Hello
 *  @brief Hello class - brief description.
 * 	@see hello.h
 */

/** @file hello.h 
 *  @brief Hello class - brief description.
 * Hello is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( Hello, Base );

/* Class virtual function prototypes
 */

static
void
_hello_virtual( Hello self )
{
	return;
}


/* Class initializing
 */

static
void
Hello_initialize( Class this )
{
	HelloVtable vtab = & HelloVtableInstance;
	
	vtab->hello_virtual	= 	_hello_virtual;

	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
Hello_finalize( Class this )
{
	/* Release global resources! */
}
#endif


/* Constructor
 */

static
void
Hello_constructor( Hello self, const void * params )
{
	assert( ooc_isInitialized( Hello ) );
	
	chain_constructor( Hello, self, NULL ); 

}

/* Destructor
 */

static
void
Hello_destructor( Hello self, HelloVtable vtab )
{
}

/* Copy constuctor
 */

static
int
Hello_copy( Hello self, const Hello from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


Hello
hello_new( void )
{
	ooc_init_class( Hello );
		
	return (Hello) ooc_new( Hello, NULL );
}


void
hello_methods( Hello self)
{
	printf("Hello World!\n");
}
