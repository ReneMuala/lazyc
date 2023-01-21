#include "mutex.h"
#include "../error/error.h"

void lazy_mutex_controller_init(ref(lazy_mutex_controller_t) this){
    check_or_do_and_return(this,
                           lazy_error("lazy_mutex_controller_t is null",__FUNCTION__, __LINE__),);
    lazy_mutex_unlock(this);
}

void lazy_mutex_lock(ref(lazy_mutex_controller_t) this){
    check_or_do_and_return(this,
                           lazy_error("lazy_mutex_controller_t is null", __FUNCTION__, __LINE__), );
    while(this->locked)
        ;
    this->locked = true;
}

void lazy_mutex_force_lock(ref(lazy_mutex_controller_t) this){
    check_or_do_and_return(this,
                           lazy_error("lazy_mutex_controller_t is null", __FUNCTION__, __LINE__), );
    this->locked = true;
}

void lazy_mutex_unlock(ref(lazy_mutex_controller_t) this){
    check_or_do_and_return(this,
                           lazy_error("lazy_mutex_controller_t is null", __FUNCTION__, __LINE__), );
    this->locked = false;
}