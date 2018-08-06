
#ifndef HELLOTEST_H
#define HELLOTEST_H

#include <libs/ooc.h>

#include "Hello.h"

DeclareClass( HelloTest, Hello );

HelloTest hellotest_new( void );

void hellotest_methods( HelloTest self );

/* Virtual function definitions
 */

Virtuals( HelloTest, Hello )

	void	(* hellotest_virtual)  ( HelloTest );

EndOfVirtuals;

#endif  /* HELLOTEST_H */
