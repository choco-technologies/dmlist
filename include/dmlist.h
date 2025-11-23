#ifndef DMLIST_H
#define DMLIST_H

#include "dmod.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Opaque type for linked list node.
 */
typedef struct dmlist_node_t dmlist_node_t;

/**
 * @brief Opaque type for linked list context.
 */
typedef struct dmlist_context_t dmlist_context_t;

/**
 * @brief Callback function type for iterating over list elements.
 * 
 * @param data Pointer to the data stored in the node.
 * @param user_data User-provided data passed to the iteration function.
 * 
 * @return true to continue iteration, false to stop.
 */
typedef bool (*dmlist_iterator_func_t)(void* data, void* user_data);

/**
 * @brief Callback function type for comparing list elements.
 * 
 * @param data1 Pointer to the first data element.
 * @param data2 Pointer to the second data element.
 * 
 * @return 0 if equal, <0 if data1 < data2, >0 if data1 > data2.
 */
typedef int (*dmlist_compare_func_t)(const void* data1, const void* data2);

/**
 * @brief Create a linked list context.
 * 
 * @param module_name Name of the module using the list (for memory tracking).
 * 
 * @return Pointer to the list context, or NULL if creation fails.
 */
DMOD_BUILTIN_API( dmlist, 1.0, dmlist_context_t*, _create, ( const char* module_name ) );

/**
 * @brief Destroy a linked list and free all its nodes.
 * 
 * @param ctx Pointer to the list context.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void, _destroy, ( dmlist_context_t* ctx ) );

/**
 * @brief Get the number of elements in the list.
 * 
 * @param ctx Pointer to the list context.
 * 
 * @return Number of elements in the list.
 */
DMOD_BUILTIN_API( dmlist, 1.0, size_t, _size, ( dmlist_context_t* ctx ) );

/**
 * @brief Check if the list is empty.
 * 
 * @param ctx Pointer to the list context.
 * 
 * @return true if the list is empty, false otherwise.
 */
DMOD_BUILTIN_API( dmlist, 1.0, bool, _is_empty, ( dmlist_context_t* ctx ) );

/**
 * @brief Add an element to the front of the list.
 * 
 * @param ctx  Pointer to the list context.
 * @param data Pointer to the data to add.
 * 
 * @return true if successful, false otherwise.
 */
DMOD_BUILTIN_API( dmlist, 1.0, bool, _push_front, ( dmlist_context_t* ctx, void* data ) );

/**
 * @brief Add an element to the back of the list.
 * 
 * @param ctx  Pointer to the list context.
 * @param data Pointer to the data to add.
 * 
 * @return true if successful, false otherwise.
 */
DMOD_BUILTIN_API( dmlist, 1.0, bool, _push_back, ( dmlist_context_t* ctx, void* data ) );

/**
 * @brief Remove and return the element at the front of the list.
 * 
 * @param ctx Pointer to the list context.
 * 
 * @return Pointer to the data, or NULL if the list is empty.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void*, _pop_front, ( dmlist_context_t* ctx ) );

/**
 * @brief Remove and return the element at the back of the list.
 * 
 * @param ctx Pointer to the list context.
 * 
 * @return Pointer to the data, or NULL if the list is empty.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void*, _pop_back, ( dmlist_context_t* ctx ) );

/**
 * @brief Get the element at the front of the list without removing it.
 * 
 * @param ctx Pointer to the list context.
 * 
 * @return Pointer to the data, or NULL if the list is empty.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void*, _front, ( dmlist_context_t* ctx ) );

/**
 * @brief Get the element at the back of the list without removing it.
 * 
 * @param ctx Pointer to the list context.
 * 
 * @return Pointer to the data, or NULL if the list is empty.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void*, _back, ( dmlist_context_t* ctx ) );

/**
 * @brief Remove all elements from the list.
 * 
 * @param ctx Pointer to the list context.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void, _clear, ( dmlist_context_t* ctx ) );

/**
 * @brief Find an element in the list.
 * 
 * @param ctx          Pointer to the list context.
 * @param data         Pointer to the data to find.
 * @param compare_func Comparison function to use.
 * 
 * @return Pointer to the found data, or NULL if not found.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void*, _find, ( dmlist_context_t* ctx, const void* data, dmlist_compare_func_t compare_func ) );

/**
 * @brief Remove an element from the list.
 * 
 * @param ctx          Pointer to the list context.
 * @param data         Pointer to the data to remove.
 * @param compare_func Comparison function to use.
 * 
 * @return true if the element was found and removed, false otherwise.
 */
DMOD_BUILTIN_API( dmlist, 1.0, bool, _remove, ( dmlist_context_t* ctx, const void* data, dmlist_compare_func_t compare_func ) );

/**
 * @brief Iterate over all elements in the list.
 * 
 * @param ctx           Pointer to the list context.
 * @param iterator_func Callback function to call for each element.
 * @param user_data     User-provided data passed to the iterator function.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void, _foreach, ( dmlist_context_t* ctx, dmlist_iterator_func_t iterator_func, void* user_data ) );

/**
 * @brief Insert an element at a specific position in the list.
 * 
 * @param ctx      Pointer to the list context.
 * @param position Position to insert at (0 = front).
 * @param data     Pointer to the data to insert.
 * 
 * @return true if successful, false otherwise.
 */
DMOD_BUILTIN_API( dmlist, 1.0, bool, _insert, ( dmlist_context_t* ctx, size_t position, void* data ) );

/**
 * @brief Get an element at a specific position in the list.
 * 
 * @param ctx      Pointer to the list context.
 * @param position Position to get (0 = front).
 * 
 * @return Pointer to the data, or NULL if position is out of bounds.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void*, _get, ( dmlist_context_t* ctx, size_t position ) );

/**
 * @brief Remove an element at a specific position in the list.
 * 
 * @param ctx      Pointer to the list context.
 * @param position Position to remove (0 = front).
 * 
 * @return Pointer to the removed data, or NULL if position is out of bounds.
 */
DMOD_BUILTIN_API( dmlist, 1.0, void*, _remove_at, ( dmlist_context_t* ctx, size_t position ) );

#endif // DMLIST_H
