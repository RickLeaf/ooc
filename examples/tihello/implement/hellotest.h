
#ifndef IMPL_HELLOTEST_H
#define IMPL_HELLOTEST_H

#ifndef HELLOTEST_H
#error Do #include "hellotest.h" right before doing #include "implement/hellotest.h"
#endif


//!!! TODO !!! Remove this group if the class has B_a_s_e parent!
#ifndef NO_RELATIVE_INCLUDE
#include "hello.h"
#else
#include "implement/hello.h"
#endif


ClassMembers( HelloTest, Hello )

	int		data;

EndOfClassMembers;


#endif /* IMPL_HELLOTEST_H */
