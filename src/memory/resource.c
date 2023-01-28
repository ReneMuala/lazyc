#include "resource.h"
#include "../error/error.h"
#include <stdio.h>

void lazy_controller_init(ref(lazy_resource_controller_t) this)
{
    this->first = NULL;
    this->last = NULL;
    this->size = 0;
}

ref(lazy_resource_t) lazy_get_last_resource(ref(lazy_resource_controller_t) this)
{
    return this->last;
}

void lazy_make_resource(ref(lazy_resource_controller_t) controller, ref(ref(void)) ptr, const uint64_t data_size, void (*deallocator)(void *))
{
    static const int lazy_resource_size = sizeof(struct lazy_resource_t);
    if(!controller) lazy_error("mapped_resource_controller is null",__FUNCTION__,__LINE__);
    struct lazy_resource_t * last = lazy_get_last_resource(controller);
    if (!last)
    {
        controller->first = last = (struct lazy_resource_t *)malloc(lazy_resource_size);
    }
    else
    {
        last->next = (struct lazy_resource_t*) malloc(lazy_resource_size);
        last = last->next;
    }
    if(data_size)
        (*ptr) = malloc(data_size);
    else
        (*ptr) = 0;
    last->size = data_size + lazy_resource_size;
    last->next = 0;
    last->data = ptr;
    last->deallocator = deallocator;
    controller->last = last;
    controller->size+=last->size;
}

void lazy_free_resources(ref(lazy_resource_controller_t) controller){
    struct lazy_resource_t * last, * current = controller->first;
    while (current) {
        last = current;
        controller->size -= last->size;
        if(last->deallocator)
            last->deallocator(*(last->data));
        else
            free(*(last->data));
        *(last->data) = 0;
        free(last);
        current = current->next;
    }
}

ref(void) lazy_move(void** origin_ptr){
    ref(void) origin = (*origin_ptr);
    (*origin_ptr) = 0;
    return origin;
}