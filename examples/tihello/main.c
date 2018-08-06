#include <stdio.h>

#include <libs/ooc.h>
#include <libs/exception.h>

#include "hello.h"
#include "hellotest.h"

int
main( int argc, char * argv[] )
{
    Hello hello;
    HelloTest hellotest;
    
	ooc_init_class( Exception );
	
	hello = hello_new();
	hello_methods(hello);

    hellotest = hellotest_new();
    
    hellotest_methods(hellotest);
    
    hello_methods((Hello)hellotest);

	return 0;
}
