#ifndef VECTOR_H_
#define VECTOR_H_

#include "ooc.h"

/** Vector class declaration
 */

DeclareClass( Vector, Base );

/** Index for the Vector.
 * The VectorIndex can be used as an identifier of an item in the Vector.
 * The VectorIndex starts at 0.
 */
 
typedef size_t VectorIndex;

/** Vector Item Destroyer.
 * Destroyer for the items stored in the Vector.
 * @see vector_new(),  
 */
 
typedef void	(* vector_item_destroyer ) ( void * item );

/** Vector Item Executor.
 * @see  vector_foreach().
 */
 
typedef void	(* vector_item_executor ) ( void * item, void * param );

/** Vector Item Checker.
 * @see vector_foreach_until_true(), vector_find_item(), vector_find_item_reverse().
 */
 
typedef int		(* vector_item_checker ) ( void * item, void * param );


/* Vector member functions
 */
 
/** Vector constructor.
 * Creates a new non-typed vector.
 * @param	chunk_size	The chunk size of the Vector. This is used as the initial size of the 
 * 						vector, and as a size increment for each reallocation.
 * @param	destroyer	The destructor for the items stored in the Vector (usually ooc_free() or
 * 						ooc_delete()). You can pass @c NULL if you store static items in the Vector,
 * 						or you take care of the item's deletion other way.
 * @return	The newly created Vector.
 */
 
Vector			vector_new( VectorIndex chunk_size, vector_item_destroyer destroyer );

/** @def vector_new_type( chunk_size, pClass, manage )
 * @brief Vector constructor.
 * Creates a new typed vector.
 * @param	chunk_size	The chunk size of the Vector. This is used as the initial size of the 
 * 						vector, and as a size increment for each reallocation.
 * @param	pClass		The type of the items in the Vector. (Name of the class.)
 * @param	manage		@c TRUE is the Vector must manage the stored items (delete when destroying
 * 						the Vector), @c FALSE if you manage those other way.
 * @return	The newly created Vector.
 * @note	This is a convenient macro for _vector_new_type().
 */
 
#define			vector_new_type( chunk_size, pClass, manage ) _vector_new_type( chunk_size, & pClass ## Class, manage )

/** Vector constructor.
 * Creates a new typed vector.
 * @param	chunk_size	The chunk size of the Vector. This is used as the initial size of the 
 * 						vector, and as a size increment for each reallocation.
 * @param	type		The type of the items in the Vector. (Class Table pointer.)
 * @param	manage		@c TRUE is the Vector must manage the stored items (delete when destroying
 * 						the Vector), @c FALSE if you manage those other way.
 * @return	The newly created Vector.
 * @see 	vector_new_type().
 */
 
Vector			_vector_new_type( VectorIndex chunk_size, Class type, int manage );

/** Vector constructor.
 * Creates a new non-typed Vector from a data table.
 * All items in the Vector are treated static, they are not freed when destroying Vector.
 * @param	table		The source table.
 * @param	record_size	The record size in the table.
 * @param	table_size	The number of records in the table.
 * @return	The newly created Vector.
 */ 

Vector			vector_new_from_table( void * table, size_t record_size, VectorIndex table_size );

/** Put an item at the end of the Vector.
 * @param	vector	The vector.
 * @param	item	The item to be put.
 * @note	For typed Vectors type check is performed.
 * 			In case of type failure a @c Exception(err_bad_cast)) exception is thrown.
 */
 
VectorIndex		vector_push_back( Vector vector, void * item );

/** Inserts an item into the vector.
 * Inserts an item into the Vector at the index. The rest of the Vector is shifted.
 * @param	vector	The vector.
 * @param	index	The point to insert the item. The new item will be accessed at index.
 * @param	item	The item to insert.
 * @note	Because of the memory move, this operation can be time consuming!
 * 			For typed Vectors type check is performed.
 * 			In case of type failure a @c Exception(err_bad_cast)) exception is thrown.
 */
 
VectorIndex		vector_insert( Vector vector, VectorIndex index, void * item );

/** Deletes an item from the vector.
 * Deletes an item from the Vector at the index. The rest of the Vector is shifted.
 * @param	vector	The vector.
 * @param	index	The point where to delete the item from the Vector. 
 *					For managed vectors or if destroyer supplied, the item is destroyed as well.
 * @note	Because of the memory move, this operation can be time consuming!
 */
 
void			vector_delete_item( Vector vector, VectorIndex index );

/** Numbers stored in the Vector.
 * @param	vector	The vector.
 * @return	The number of items stored in the Vector. 
 * 			(This is the number of items you really put into the vector, and not the size of the allocation chunks!)
 */
 
VectorIndex		vector_items( Vector vector );

/** Overwrites an item.
 * Overwrites an item at index.
 * @param	vector	The vector.
 * @param	index	The position in the vector.
 * @param	item	The item to put in the vector.
 * @note	The previous item at this position is destroyed, if destroyer was supplied, or
 * 			managed is set for typed Vector.
 * 			The other items are not moved.
 */
 
void            vector_set_item( Vector vector, VectorIndex index, void * item );

/** Retrieves an item.
 * Gets the item at position index. The item is not removed from the Vector
 * and is still maintained by it.
 * @param	vector	The Vector.
 * @param	index	Position.
 * @return	Pointer to the item.
 */
 
void *          vector_get_item( Vector vector, VectorIndex index );

/** Swaps two items in the Vector.
 * @param	vector	The Vector.
 * @param	index1	First position to be swapped.
 * @param	index2	Second position to be swapped.
 */
 
void			vector_swap( Vector vector, VectorIndex index1, VectorIndex index2 );

/** Executes a task for each item in the Vector.
 * @param	vector	The Vector
 * @param	task	The task to be executed. The task is a C function, and must have two parameters:
 * 					the first will point to the item, the second holpd the parameter. This is a 
 * 					normal way of calling a class method too.
 * @param	param	The second parameter to be passed to the @c task.
 * @note	You must not modify the vector itself in the task (e.g. deleting an item, etc.).
 */
  
void            vector_foreach( Vector vector, vector_item_executor task, void * param );

/** Executes a task for each item until @c TRUE.
 * Executes the @c checker task for each item in the @c vector, until the @c checker
 * returns @c TRUE. (If @c checker returns @c FALSE then will continue with the next item in
 * the @c vector, otherwise the execution will stop.)
 * @param	vector	The Vector
 * @param	checker	The task to be executed. The @c checker is a C function, and must have two parameters:
 * 					the first will point to the item, the second holpd the parameter. This is a 
 * 					normal way of calling a class method too.
 * 					The @c checker must return @c TRUE or @ FALSE.
 * @param	param	The second parameter to be passed to the @c checker.
 * @return	The position index of the last checked item (for which the @c checker returned @c TRUE).
 * 			If the @c checker did not returned @c TRUE, the return value is an index value pointing
 * 			behind the last item in the vector (== vector_items(), an invalid index!)
 * @note	You must not modify the vector itself in the task (e.g. deleting an item, etc.).
 * @see vector_find_item(), vector_find_item_reverse()
 */
 
VectorIndex		vector_foreach_until_true( Vector vector, vector_item_checker checker, void * param );

/** Finds an item in the Vector.
 * Executes the @c checker task for each item in the @c vector, starting from position @c index,
 * until the @c checker returns @c TRUE. 
 * (If @c checker returns @c FALSE then will continue with the next item in
 * the @c vector, otherwise the execution will stop.)
 * @param	vector	The Vector
 * @param	index	The starting position of the search.
 * @param	checker	The checker to be executed.
 * 					The @c checker is a C function, and must have two parameters:
 * 					the first will point to the item, the second holpd the parameter. This is a 
 * 					normal way of calling a class method too.
 * 					The @c checker must return @c TRUE if we found the item or @c FALSE otherwise.
 * @param	param	The second parameter to be passed to the @c checker.
 * @return	The position index of the item if found (for which the @c checker returned @c TRUE).
 * 			If the @c checker did not returned @c TRUE, the return value is an index value pointing
 * 			behind the last item in the vector (== vector_items(), an invalid index!)
 * @note	You must not modify the vector itself in the task (e.g. deleting an item, etc.).
 * @note	This is a linear search, that may be slow for large Vectors.
 * @see		vector_find_item_reverse(), vector_foreach_until_true()
 */

VectorIndex		vector_find_item        ( Vector vector, VectorIndex index,  vector_item_checker checker, void * param );

/** Finds an item in the Vector in reverse order.
 * Executes the @c checker task for each item in the @c vector in reverse order, 
 * starting from position @c index, until the @c checker returns @c TRUE.
 * (If @c checker returns @c FALSE then will continue with the next item in
 * the @c vector, otherwise the execution will stop.)
 * @param	vector	The Vector
 * @param	index	The starting position of the search.
 * @param	checker	The checker to be executed.
 * 					The @c checker is a C function, and must have two parameters:
 * 					the first will point to the item, the second holpd the parameter. This is a 
 * 					normal way of calling a class method too.
 * 					The @c checker must return @c TRUE if we found the item or @c FALSE otherwise.
 * @param	param	The second parameter to be passed to the @c checker.
 * @return	The position index of the item if found (for which the @c checker returned @c TRUE).
 * 			If the @c checker did not returned @c TRUE, the return value is an index value pointing
 * 			behind the last item in the vector (== vector_items(), an invalid index!)
 * @note	You must not modify the vector itself in the task (e.g. deleting an item, etc.).
 * @note	This is a linear search, that may be slow for large Vectors.
 * @see		vector_find_item(), vector_foreach_until_true()
 */

VectorIndex		vector_find_item_reverse( Vector vector, VectorIndex index,  vector_item_checker checker, void * param );

#endif /*VECTOR_H_*/
