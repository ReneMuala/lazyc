#pragma once

#define LAZYC_VERSION "0.0.2" 

#include <stdlib.h>
/// @brief if x is false, then y will be returned
#define check_or_return(x, y) \
    if (!(x))               \
        return y;

/// @brief if x is false, then y will executed
#define check_or_do(x, y) \
    if (!(x))                       \
    {                               \
        y;                          \
    }

/// @brief if x is false, then y will executed and z will be returned
#define check_or_do_and_return(x, y, z) \
    if (!(x))                       \
    {                               \
        y;                          \
        return z;                   \
    }

/// @brief represents unmapped resources, should be used in contexts where he returned resource must to be deleted manually
#define new(x) x*

/// @brief represents pointers that cannot be null
#define ref(t) t*

/// @brief returns the reference of v
#define get_ref(v) &v

/// @see new(x)
#define new_resource(t,x) t * x = malloc(sizeof(t))

#define renew_resource(t,x) delete_resource(x); x = malloc(sizeof(t))

// #define new_mapped_resource(x, t, c)

#define safely_delete_resource(ptr_ptr) \
    if(ptr_ptr)                  \
    {                            \
        free((void*)ptr_ptr);    \
        ptr_ptr = 0;             \
    }