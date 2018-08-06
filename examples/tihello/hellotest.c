
/* This is a HelloTest class implementation file
 */
#include <stdio.h>
#include "hellotest.h"
#include "implement/hellotest.h"

#include <libs/exception.h>

/** @class HelloTest
 *  @brief HelloTest class - brief description.
 * 	@see hellotest.h
 */

/** @file hellotest.h 
 *  @brief HelloTest class - brief description.
 * HelloTest is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( HelloTest, Hello );

/* Class virtual function prototypes
 */

static
void
_hellotest_virtual( HelloTest self )
{
	return;
}


/* Class initializing
 */

static
void
HelloTest_initialize( Class this )
{
	HelloTestVtable vtab = & HelloTestVtableInstance;
	
	vtab->hellotest_virtual	= 	_hellotest_virtual;

	//ooc_init_class( !!! Classes that are USED by HelloTest !!! ); Remove if you initialize it other place!
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
HelloTest_finalize( Class this )
{
	/* Release global resources! */
}
#endif


/* Constructor
 */

static
void
HelloTest_constructor( HelloTest self, const void * params )
{
	assert( ooc_isInitialized( HelloTest ) );
	
	chain_constructor( HelloTest, self, NULL ); 

}

/* Destructor
 */

static
void
HelloTest_destructor( HelloTest self, HelloTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
HelloTest_copy( HelloTest self, const HelloTest from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


HelloTest
hellotest_new( void )
{
	ooc_init_class( HelloTest ); 
		
	return (HelloTest) ooc_new( HelloTest, NULL );
}


void
hellotest_methods( HelloTest self)
{
	printf("Hello World Test!\n");
}
