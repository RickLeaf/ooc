
/* This is a Sample class implementation file
 */

#include "implement/sample.h"

#include <exception.h>

/** @class Sample
 *  @brief Sample class - brief description.
 * 	@see sample.h
 */

/** @file sample.h 
 *  @brief Sample class - brief description.
 * Sample is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( Sample, Base );

/* Class virtual function prototypes
 */

static
void
_sample_virtual( Sample self )
{
	return;
}


/* Class initializing
 */

static
void
Sample_initialize( Class this )
{
	SampleVtable vtab = (SampleVtable) this->vtable;
	
	vtab->sample_virtual	= 	_sample_virtual;

	ooc_init_class( !!! Classes that are USED by Sample !!! ); Remove if you initialize it other place!
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

static
void
Sample_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
Sample_constructor( Sample self, const void * params )
{
	assert( ooc_isInitialized( Sample ) );
	
	chain_constructor( Sample, self, NULL ); !!! TODO !!! change the third parameter as required! 

}

/* Destructor
 */

static
void
Sample_destructor( Sample self )
{
}

/* Copy constuctor
 */

static
int
Sample_copy( Sample self, const Sample from )
{
	/* makes the default object copying (bit-by-bit) */
	return FALSE;
	
	/* Copies data by hand */
	self->data = from->data;
	...
	return TRUE
	
	/* Prevent object duplication */
	ooc_throw( exception_new( err_can_not_be_duplicated ) );
	return TRUE;
}

/*	=====================================================
	Class member functions
 */


Sample
sample_new( void )
{
	ooc_init_class( Sample ); !!! TODO !!! You may want to remove the initialization from here ...
		
	return (Sample) ooc_new( Sample, NULL );
}


void
sample_methods( Sample self, int )
{
	
}