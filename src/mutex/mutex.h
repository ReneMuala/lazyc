#pragma once
#include <stdbool.h>
#include "../macros.h"

/// @brief mutex controller
struct lazy_mutex_controller_t {
    bool locked;
};

typedef struct lazy_mutex_controller_t  lazy_mutex_controller_t;

/// @brief initializes a lazy_mutex controller
extern void lazy_mutex_controller_init(ref(lazy_mutex_controller_t) controller);

/// @brief waits until the lazy_mutex is available to lock it
extern void lazy_mutex_lock(ref(lazy_mutex_controller_t) controller);

/// @brief locks the lazy_mutex controller immediately
extern void lazy_mutex_force_lock(ref(lazy_mutex_controller_t) controller);

/// @brief unlocks the lazy_mutex controller
extern void lazy_mutex_unlock(ref(lazy_mutex_controller_t) controller);