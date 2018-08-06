/*
 * port/c6000.h
 * 
 * Copyright (c) 2011, old_fashioned. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */


#ifndef C6000_H_
#define C6000_H_

/* Inlcude standard headers */

#include <stdlib.h>
#include <assert.h>

#ifdef COMPILING_OOC_C
#include <stddef.h>
#include <string.h>
#endif /* COMPILING_OOC_C */

#ifndef OOC_NO_THREADS
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <xdc/cfg/global.h>
#endif



/* Longjump functions */
#include <setjmp.h>
 
#define JMP_BUF		jmp_buf
#define SETJMP		setjmp
#define LONGJMP		longjmp


/* implementing static inline */
#define STIN static __inline

#ifndef OOC_NO_THREADS
/* implementing Thread Local Storage */

#define TLS static

typedef Semaphore_Handle        ooc_Mutex;
#define ooc_mutex_init( x )     (x = Semaphore_create(1, NULL, NULL))
#define ooc_mutex_release( x )  Semaphore_delete(&x)
#define ooc_lock( x )	         Semaphore_pend(x, BIOS_WAIT_FOREVER)
#define ooc_unlock( x )         Semaphore_post(x)
#define ooc_try_lock( x )		 if (Semaphore_getCount(x) == 0) {       \
									Semaphore_pend(x, BIOS_WAIT_FOREVER);\
								 }

#else /* threadless implementation */

#define TLS static

#define ooc_Mutex				void
#define ooc_mutex_init( x )
#define ooc_mutex_release( x )
#define ooc_lock( x )
#define ooc_unlock( x )
#define ooc_try_lock( x )		TRUE

#endif
/* Compilation control
 */

#define OOC_HAS_UNIX_SIGNALS

#ifdef COMPILING_OOC_C

/*	Helper: pointer read-out while nulling
 */

#define OOC_IMPLEMENT_PTR_READ_AND_NULL						\
	void * tmp = * ptr_ptr;									\
	* ptr_ptr = NULL;										\
	return tmp;

#endif /* COMPILING_OOC_C */

#endif /*ANYC_H_*/
