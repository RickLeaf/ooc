
#include "ooc.h"
#include "implement/list.h"
#include "exception.h"

#include "signal.h"

/** @class Signal
 *  @brief Signal class - an ooc signal / slot mechanism.
 * 	@see signal.h
 */

/** @file signal.h 
 *  @brief Signal class - an ooc signal / slot mechanism.
 * A signal / slot mechanism implemented for the ooc object manager.
  */ 

/* This is a class implementation file
 */
 
DeclareClass( SignalItem, Base );

/* Virtual function definitions
 */

Virtuals( SignalItem, Base )

EndOfVirtuals;

/* Class members
 */
 
ClassMembers( SignalItem, Base )

	void *			source;
	void *			target;
	SignalHandler	handler;
	
EndOfClassMembers;


/* Allocating the class description table and the vtable
 */

AllocateClass( SignalItem, Base );

/* Class virtual function prototypes
 */

/* Class initializing
 */

static
void
SignalItem_initialize( Class this )
{

}

/* Class finalizing
 */

static
void
SignalItem_finalize( Class this )
{
}


/* Constructor
 */

static
void
SignalItem_constructor( SignalItem self, const void * params )
{
	assert( ooc_isInitialized( SignalItem ) );
	
	chain_constructor( SignalItem, self, NULL );

}

/* Destructor
 */

static
void
SignalItem_destructor( SignalItem self )
{
}

/* Copy constuctor
 */

static
int
SignalItem_copy( SignalItem self, const SignalItem from )
{
	return FALSE;
}

static
int
signalitem_compare( SignalItem self, const SignalItem other )
{
	return	( 	( self->source == other->source )
			&&	( self->target == other->target )
			&&	( self->handler == other->handler ) );
}

/* Signal class
 */
 
ClassMembers( Signal, List )

EndOfClassMembers;

AllocateClass( Signal, List );

/* Class virtual function prototypes
 */

/* Signal Queue
 */

struct SignalQueued {
	Signal			emitted_signal;
	void *			parameter;				/* managed by SignalQueued! */
	void_fn_voidp	param_destroyer;
	};
	
static void signal_queued_free( struct SignalQueued * sq );
 
static	List	signal_queue	= 	NULL; 	/* List of SignalQueued pointers */

/* Signal register
 */
 
static 	List 	signal_register =	NULL; 	/* List of Signal * pointers */

/* Class initializing
 */

static
void
Signal_initialize( Class this )
{
	ooc_init_class( SignalItem );
	ooc_init_class( List );
	
	signal_queue 	= list_new( ( list_item_destroyer ) signal_queued_free );
	signal_register = list_new( ( list_item_destroyer ) ooc_delete_and_null );
}

/* Class finalizing
 */

static
void
Signal_finalize( Class this )
{
	ooc_delete_and_null( (Object *) & signal_queue );
	ooc_delete_and_null( (Object *) & signal_register );
}

/* Constructor
 */

static
void
Signal_constructor( Signal self, const void * params )
{
	assert( ooc_isInitialized( Signal ) );
	
	chain_constructor( Signal, self, ooc_delete );  /* Parent is a List of SignalItems */
}

/* Destructor
 */

static
void
Signal_destructor( Signal self )
{
}

/* Copy constuctor
 */

static
int
Signal_copy( Signal self, const Signal from )
{
	return FALSE;
}

static
void
signal_queued_free( struct SignalQueued * sq )
{
	if( sq->param_destroyer )
		sq->param_destroyer( sq->parameter );
		
	ooc_free( sq );
}

/**	Emits a signal asynchronuosly. 
 * Asynchronuos signal emittion means, that the signal_emit() function returns immadiately, and the signal is buffered in the signal queue.
 * Signal is executed when the main eventloop reaches the signal emittion section.
 * @param signal 			The signal that must be emitted.
 * @param parameter 		The parameter to be passed to the connected signal handler. The signaling system takes over the ownership of this 
 * 							memory block or Object if param_destroy_fn != NULL, so you must not destroy it!
 * @param param_destroy_fn	The destroy function for freeing up the memory occupied by the parameter. The signalimg system automatically destroys
 * 							the parameter when all connected signal handler has been executed.
 * 							For constant parameters you should pass NULL (no demolition is required at the end).
 * @see signal_emit_sync()
 */

void
signal_emit( Signal signal, void * parameter, void_fn_voidp param_destroy_fn )
{
	struct SignalQueued sq;
		
	if( signal ) {
		assert( ooc_isInstanceOf( signal, Signal ) );

		sq.emitted_signal 	= signal;
		sq.parameter	  	= parameter;
		sq.param_destroyer	= param_destroy_fn;
		
		list_append( signal_queue, ooc_memdup( & sq, sizeof( struct SignalQueued ) ) );
		}
	else if( param_destroy_fn )
		param_destroy_fn( parameter );
}

static
int
signal_queue_process_item( struct SignalQueued * sq )
{
	signal_emit_sync( sq->emitted_signal, sq->parameter, NULL );
	
	return TRUE; /* delete this item from the queue */
}

/**	Processes the asynchronously emitted signals.
 * Must be called from the eventloop. Processes all buffered async signals.
 * @warning	The async signals emitted by the called handlers will be executed as well before return. 
 */
  
void
signal_process_signals( void )
{
	assert( ooc_isInstanceOf( signal_queue, List ) );
	
	list_foreach_delete_if( signal_queue, (list_item_checker) signal_queue_process_item, NULL );
}

static
int
signal_register_comparator( Signal * self, Signal * other )
{
	return ( self == other );	
}

static
void
signal_register_signal( Signal * signal_p )
{
	assert( signal_p );
	assert( ooc_isInstanceOf( * signal_p, Signal ) );
	assert( ooc_isInstanceOf( signal_register, List ) );
	
	if( ! list_find_item( list_first( signal_register ), (list_item_checker) signal_register_comparator, signal_p ) ) {
		list_append( signal_register, signal_p );
		};
}

static
int
signalitem_is_targeting( SignalItem self, Object target )
{
	assert( ooc_isInstanceOf( self, SignalItem ) );
	
	return ( self->target == target );
}

static
void
signal_register_disconnect_target( Signal * signal_p, Object target )
{
	assert( signal_p );
	assert( ooc_isInstanceOf( * signal_p, Signal ) );
	
	list_foreach_delete_if( (List) * signal_p, (list_item_checker) signalitem_is_targeting, target );
}

static
int
signal_is_hold_by( Signal * signal_p, Object holder )
{
	assert( signal_p );
	assert( ooc_isInstanceOf( * signal_p, Signal ) );
	
	SignalItem si = list_get_item( list_first( (List)* signal_p ) );
	
	return ( si->source == holder );
}

static
int
signal_queued_is_hold_by( struct SignalQueued * sq, Object holder )
{
	return signal_is_hold_by( & sq->emitted_signal, holder );
}

/** Object destroy notifier for the signaling system.
 * Every Object that can be signal source or a signal target, must call this notifier in their destructor.
 * This notifier disconnects all signals that are related to the Object under destruction.
 * @param	object	The Object that is being deleted.
 */
 
void
signal_destroy_notify( Object object )
{
	assert( ooc_isInstanceOf( signal_register, List ) );
	assert( ooc_isInstanceOf( signal_queue, List ) );
	
	/* remove all signals from queue that are held by this object */
	list_foreach_delete_if( signal_queue, (list_item_checker) signal_queued_is_hold_by, object );
	
	/* disconnects all signals, that targets this object */
	list_foreach( signal_register, (list_item_executor) signal_register_disconnect_target, object );
	
	/* delete all signals from signal_register, that are held by this object */
	list_foreach_delete_if( signal_register, (list_item_checker) signal_is_hold_by, object );
}



/*	=====================================================
	Class member functions
 */
 
 /**
  * @typedef typedef void ( * SignalHandler )( void * target, void * source, void * param )
  * @brief	Signal handler prototype.
  * 		The signal handler is a function that has three parameters.
  * 		In Object oriented manner it is a member function of the target object.
  * @param	target		The Object that has been assigned to the handler.
  * @param 	source		The source Object that has activated the signal.
  * @param	param		The parameter the has been passed at the time of signal emittion.
  * @see signal_connect(), signal_emit(), signal_emit_sync()
  * @note	The signal handler can throw an exception, but this exception will not be propagated. The signal emittion routines will return
  * 		as if everything went normal. 
  */

/** Connects a signal to a signal handler.
 * The connected signal handler will be executed when the signal is emitted.
 * @param	source		The source Object of the signal (the Object that holds the Signal as a member).
 * @param	signal_p	The address of the Signal (must be in the source Object!).
 * @param	target		The target Object (the object that is passed as the first parameter of the signal handler).
 * @param	handler		The signal handler.
 * @see	signal_disconnect()
 * @note	Multiple handlers can be connected to a signal, they are called in the order of connections in the case of signal emittion.
 * @note	For user's convenience there is no new operator for Signals, need to create new signals, just connect them. Never call ooc_new( Signal, NULL )!;
 */
 
void
signal_connect( void * source, Signal * signal_p, void * target, SignalHandler handler )
{
	struct SignalItemObject	si;
	
	if( * signal_p == NULL )
		* signal_p = (Signal) ooc_new( Signal, NULL );
	
	si.source	= source;
	si.target	= target;
	si.handler	= handler;
	
	list_append( (List) *signal_p, ooc_duplicate( (Object) & si ) );
	
	signal_register_signal( signal_p );
}

/** Disconnects a signal handler from a signal.
 * The connected signal handler will be disconnected.
 * @param	source		The source Object of the signal.
 * @param	self		The Signal (must be in the source Object!).
 * @param	target		The target Object.
 * @param	handler		The signal handler.
 * @see	signal_connect()
 * @note	If the same signal handler was connected multiple times to the same source and target
 * 			objects (meaningless but possible) only the first one will be disconnected!
 * @warning	Already buffered async emitted signals will be executed even if they have been disconnected!
 */
 
void
signal_disconnect( void * source, Signal self, void * target, SignalHandler handler )
{
	struct SignalItemObject si;
	ListIterator found;

	if( self ) {
		
		assert( ooc_isInstanceOf( self, Signal ) );
		
		si.source	= source;
		si.target	= target;
		si.handler	= handler;
		
		found = list_find_item( list_first( (List) self ), (list_item_checker) signalitem_compare, & si );
		
		if( found )
			list_delete_item( (List) self, found );
		}
}


static
void
signalitem_emit_sync( SignalItem self, void * param )
{
	assert( ooc_isInstanceOf( self, SignalItem ) );
	
	try
		self->handler( self->target, self->source, param );
	catch_any;
	end_try;
}

/**	Emits a signal synchronously. 
 * Synchronous signal emittion means, that the signal handlers are called immediately, and the signal_emit_sync() function returns only,
 * when all connected signal handlers has been executed.
 * @param signal 			The signal that must be emitted.
 * @param parameter 		The parameter to be passed to the connected signal handler. The signaling system takes over the ownership of this 
 * 							memory block or Object if param_destroy_fn != NULL, so you must not destroy it!
 * @param param_destroy_fn	The destroy function for freeing up the memory occupied by the parameter. The signalimg system automatically destroys
 * 							the parameter when all connected signal handler has been executed.
 * 							For constant parameters you should pass NULL (no demolition is required at the end).
 * 							If you would like to take care of the lifecycle of the parameter object instaed of the signaling system
 * 							then you can pass NULL as well.
 * @see signal_emit()
 */

void
signal_emit_sync( Signal signal, void * parameter, void_fn_voidp param_destroy_fn )
{
	if( signal ) {
		
		assert( ooc_isInstanceOf( signal, Signal ) );
		
		list_foreach( (List) signal, (list_item_executor) signalitem_emit_sync, parameter );
		}
	
	if( param_destroy_fn )
		param_destroy_fn( parameter );
}

/** @file signal.h 
@details
The use of the Signal class is optimized for the user's concenience, therefore it is a bit different then other ooc classes.
There are some rules that must be considered using Signals.
The signaling system must be initilaized at the start and released at the end of your application.
@code
int main( int argc, char argv[] )
{
	ooc_init_class( Signal );

	...  Your staff here 

	ooc_finalize_all();
	return 0;
}
@endcode

@todo		Desrcibing the use of the signaling system

@warning	Signal handling is not thread safe!

 */ 

