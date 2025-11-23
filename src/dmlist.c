#include "dmlist.h"
#include <string.h>

/**
 * @brief Structure to represent a node in the linked list.
 */
struct dmlist_node_t
{
    void* data;                 //!< Pointer to the data stored in the node.
    struct dmlist_node_t* next; //!< Pointer to the next node.
    struct dmlist_node_t* prev; //!< Pointer to the previous node.
};

/**
 * @brief Structure to hold the context of the linked list.
 */
struct dmlist_context_t
{
    dmlist_node_t* head;                         //!< Pointer to the first node.
    dmlist_node_t* tail;                         //!< Pointer to the last node.
    size_t size;                                 //!< Number of elements in the list.
    char module_name[DMOD_MAX_MODULE_NAME_LENGTH]; //!< Name of the module using the list.
};

/**
 * @brief Create a new node.
 * 
 * @param data        Pointer to the data to store in the node.
 * @param module_name Name of the module creating the node.
 * 
 * @return Pointer to the created node, or NULL if allocation fails.
 */
static dmlist_node_t* create_node( void* data, const char* module_name )
{
    dmlist_node_t* node = (dmlist_node_t*)Dmod_MallocEx( sizeof(dmlist_node_t), module_name );
    if( node == NULL )
    {
        DMOD_LOG_ERROR("dmlist: Failed to allocate memory for node.\n");
        return NULL;
    }
    
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    
    return node;
}

/**
 * @brief Free a node.
 * 
 * @param node Pointer to the node to free.
 */
static void free_node( dmlist_node_t* node )
{
    if( node != NULL )
    {
        Dmod_Free( node );
    }
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, dmlist_context_t*, _create, ( const char* module_name ) )
{
    if( module_name == NULL )
    {
        DMOD_LOG_ERROR("dmlist: _create called with NULL module_name.\n");
        return NULL;
    }
    
    dmlist_context_t* ctx = (dmlist_context_t*)Dmod_MallocEx( sizeof(dmlist_context_t), module_name );
    if( ctx == NULL )
    {
        DMOD_LOG_ERROR("dmlist: Failed to allocate memory for list context.\n");
        return NULL;
    }
    
    ctx->head = NULL;
    ctx->tail = NULL;
    ctx->size = 0;
    strncpy( ctx->module_name, module_name, DMOD_MAX_MODULE_NAME_LENGTH - 1 );
    ctx->module_name[DMOD_MAX_MODULE_NAME_LENGTH - 1] = '\0';
    
    DMOD_LOG_INFO("dmlist: Created list for module %s.\n", module_name);
    
    return ctx;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void, _destroy, ( dmlist_context_t* ctx ) )
{
    if( ctx == NULL )
    {
        return;
    }
    
    dmlist_clear( ctx );
    DMOD_LOG_INFO("dmlist: Destroyed list for module %s.\n", ctx->module_name);
    Dmod_Free( ctx );
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, size_t, _size, ( dmlist_context_t* ctx ) )
{
    if( ctx == NULL )
    {
        return 0;
    }
    
    return ctx->size;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, bool, _is_empty, ( dmlist_context_t* ctx ) )
{
    if( ctx == NULL )
    {
        return true;
    }
    
    return ctx->size == 0;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, bool, _push_front, ( dmlist_context_t* ctx, void* data ) )
{
    if( ctx == NULL )
    {
        DMOD_LOG_ERROR("dmlist: _push_front called with NULL context.\n");
        return false;
    }
    
    dmlist_node_t* node = create_node( data, ctx->module_name );
    if( node == NULL )
    {
        return false;
    }
    
    if( ctx->head == NULL )
    {
        // List is empty
        ctx->head = node;
        ctx->tail = node;
    }
    else
    {
        // Add to front
        node->next = ctx->head;
        ctx->head->prev = node;
        ctx->head = node;
    }
    
    ctx->size++;
    
    return true;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, bool, _push_back, ( dmlist_context_t* ctx, void* data ) )
{
    if( ctx == NULL )
    {
        DMOD_LOG_ERROR("dmlist: _push_back called with NULL context.\n");
        return false;
    }
    
    dmlist_node_t* node = create_node( data, ctx->module_name );
    if( node == NULL )
    {
        return false;
    }
    
    if( ctx->tail == NULL )
    {
        // List is empty
        ctx->head = node;
        ctx->tail = node;
    }
    else
    {
        // Add to back
        node->prev = ctx->tail;
        ctx->tail->next = node;
        ctx->tail = node;
    }
    
    ctx->size++;
    
    return true;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void*, _pop_front, ( dmlist_context_t* ctx ) )
{
    if( ctx == NULL || ctx->head == NULL )
    {
        return NULL;
    }
    
    dmlist_node_t* node = ctx->head;
    void* data = node->data;
    
    if( ctx->head == ctx->tail )
    {
        // Only one element
        ctx->head = NULL;
        ctx->tail = NULL;
    }
    else
    {
        // Multiple elements
        ctx->head = node->next;
        ctx->head->prev = NULL;
    }
    
    free_node( node );
    ctx->size--;
    
    return data;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void*, _pop_back, ( dmlist_context_t* ctx ) )
{
    if( ctx == NULL || ctx->tail == NULL )
    {
        return NULL;
    }
    
    dmlist_node_t* node = ctx->tail;
    void* data = node->data;
    
    if( ctx->head == ctx->tail )
    {
        // Only one element
        ctx->head = NULL;
        ctx->tail = NULL;
    }
    else
    {
        // Multiple elements
        ctx->tail = node->prev;
        ctx->tail->next = NULL;
    }
    
    free_node( node );
    ctx->size--;
    
    return data;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void*, _front, ( dmlist_context_t* ctx ) )
{
    if( ctx == NULL || ctx->head == NULL )
    {
        return NULL;
    }
    
    return ctx->head->data;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void*, _back, ( dmlist_context_t* ctx ) )
{
    if( ctx == NULL || ctx->tail == NULL )
    {
        return NULL;
    }
    
    return ctx->tail->data;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void, _clear, ( dmlist_context_t* ctx ) )
{
    if( ctx == NULL )
    {
        return;
    }
    
    dmlist_node_t* current = ctx->head;
    while( current != NULL )
    {
        dmlist_node_t* next = current->next;
        free_node( current );
        current = next;
    }
    
    ctx->head = NULL;
    ctx->tail = NULL;
    ctx->size = 0;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void*, _find, ( dmlist_context_t* ctx, const void* data, dmlist_compare_func_t compare_func ) )
{
    if( ctx == NULL || compare_func == NULL )
    {
        return NULL;
    }
    
    dmlist_node_t* current = ctx->head;
    while( current != NULL )
    {
        if( compare_func( current->data, data ) == 0 )
        {
            return current->data;
        }
        current = current->next;
    }
    
    return NULL;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, bool, _remove, ( dmlist_context_t* ctx, const void* data, dmlist_compare_func_t compare_func ) )
{
    if( ctx == NULL || compare_func == NULL )
    {
        return false;
    }
    
    dmlist_node_t* current = ctx->head;
    while( current != NULL )
    {
        if( compare_func( current->data, data ) == 0 )
        {
            // Found the node to remove
            if( current == ctx->head && current == ctx->tail )
            {
                // Only one element
                ctx->head = NULL;
                ctx->tail = NULL;
            }
            else if( current == ctx->head )
            {
                // First element
                ctx->head = current->next;
                ctx->head->prev = NULL;
            }
            else if( current == ctx->tail )
            {
                // Last element
                ctx->tail = current->prev;
                ctx->tail->next = NULL;
            }
            else
            {
                // Middle element
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            
            free_node( current );
            ctx->size--;
            
            return true;
        }
        current = current->next;
    }
    
    return false;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void, _foreach, ( dmlist_context_t* ctx, dmlist_iterator_func_t iterator_func, void* user_data ) )
{
    if( ctx == NULL || iterator_func == NULL )
    {
        return;
    }
    
    dmlist_node_t* current = ctx->head;
    while( current != NULL )
    {
        if( !iterator_func( current->data, user_data ) )
        {
            break;
        }
        current = current->next;
    }
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, bool, _insert, ( dmlist_context_t* ctx, size_t position, void* data ) )
{
    if( ctx == NULL )
    {
        DMOD_LOG_ERROR("dmlist: _insert called with NULL context.\n");
        return false;
    }
    
    if( position == 0 )
    {
        return dmlist_push_front( ctx, data );
    }
    
    if( position >= ctx->size )
    {
        return dmlist_push_back( ctx, data );
    }
    
    dmlist_node_t* node = create_node( data, ctx->module_name );
    if( node == NULL )
    {
        return false;
    }
    
    dmlist_node_t* current = ctx->head;
    for( size_t i = 0; i < position; i++ )
    {
        current = current->next;
    }
    
    // Insert before current
    node->next = current;
    node->prev = current->prev;
    current->prev->next = node;
    current->prev = node;
    
    ctx->size++;
    
    return true;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void*, _get, ( dmlist_context_t* ctx, size_t position ) )
{
    if( ctx == NULL || position >= ctx->size )
    {
        return NULL;
    }
    
    dmlist_node_t* current = ctx->head;
    for( size_t i = 0; i < position; i++ )
    {
        current = current->next;
    }
    
    return current->data;
}

DMOD_INPUT_API_DECLARATION( dmlist, 1.0, void*, _remove_at, ( dmlist_context_t* ctx, size_t position ) )
{
    if( ctx == NULL || position >= ctx->size )
    {
        return NULL;
    }
    
    if( position == 0 )
    {
        return dmlist_pop_front( ctx );
    }
    
    if( position == ctx->size - 1 )
    {
        return dmlist_pop_back( ctx );
    }
    
    dmlist_node_t* current = ctx->head;
    for( size_t i = 0; i < position; i++ )
    {
        current = current->next;
    }
    
    void* data = current->data;
    
    current->prev->next = current->next;
    current->next->prev = current->prev;
    
    free_node( current );
    ctx->size--;
    
    return data;
}
