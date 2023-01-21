#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "../macros.h"

/// @brief a single unit of allocated resource
struct lazy_resource_t {
    void ** data;
    void (*deallocator)(void*);
    struct lazy_resource_t * next;
    int size;
};

typedef struct lazy_resource_t
lazy_resource_t;

/// @brief controls a set of resources
struct lazy_resource_controller_t {
    struct lazy_resource_t * first, * last;
    long long size;
};

typedef struct lazy_resource_controller_t lazy_resource_controller_t;
typedef struct lazy_resource_controller_t lazy_controller_t;

/// @brief initializes a lazy resource controller
/// @param controller the lazy controller
extern void lazy_controller_init(ref(lazy_resource_controller_t) controller);

/// @returns the last resource on the controller
extern ref(lazy_resource_t) lazy_get_last_resource(ref(lazy_resource_controller_t) controller);

/// @brief allocates size bytes and gives the control to the controller
/// @param controller the lazy controller
/// @param size number of bytes to allocate 
/// @param deallocator a custom callback for deallocating the resource (or 0)
/// @returns a void reference
/// @see ref(t)
extern void lazy_make_resource(ref(lazy_resource_controller_t) controller, ref(ref(void)) ptr , const uint64_t size, void (*deallocator)(void*));

/// @brief deallocates all resources associated with the controller
/// @param controller the lazy controller
extern void lazy_free_resources(ref(lazy_resource_controller_t) controller);