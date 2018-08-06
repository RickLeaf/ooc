
#ifndef HELLO_H
#define HELLO_H

#include <libs/ooc.h>

DeclareClass( Hello, Base );

Hello hello_new( void );

void hello_methods( Hello self );

/* Virtual function definitions
 */

Virtuals( Hello, Base )

	void	(* hello_virtual)  ( Hello );

EndOfVirtuals;

#endif  /* HELLO_H */
