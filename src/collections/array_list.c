#include "array_list.h"
#include "../error/error.h"
#include "stdlib.h"

void lazy_array_list_init(ref(lazy_array_list_controller_t) controller_ptr)
{
    check_or_do_and_return(controller_ptr,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    controller_ptr->first =
        controller_ptr->last = 0;
    controller_ptr->deallocator = 0;
    controller_ptr->length = 0;
}

void lazy_array_list_deinit(ref(lazy_array_list_controller_t) controller){
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    
    ref(lazy_array_list_node_t) node = controller->first;
    ref(lazy_array_list_node_t) aux;
    while(node){
        aux = node->next;
        if(controller->deallocator)
            controller->deallocator(node->data);    
        else 
            free(node->data);
        free(node);
        node=aux;
    }
    lazy_array_list_init(controller);
}

void lazy_array_list_set_deallocator(ref(lazy_array_list_controller_t) controller_ptr, void (*deallocator)(void *))
{
    check_or_do_and_return(controller_ptr,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    check_or_do_and_return(deallocator,
                           lazy_error("deallocator is null", __FUNCTION__, __LINE__), );
    controller_ptr->deallocator = deallocator;
}

ref(lazy_array_list_node_t) lazy_array_list_at(ref(lazy_array_list_controller_t) controller, unsigned int index){
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), 0);
    check_or_do_and_return(index < controller->length,
                           lazy_error("index is out of range", __FUNCTION__, __LINE__), 0);
    
    return (index <= controller->length / 2) ? __lazy_array_list_at_asc(controller, index) : __lazy_array_list_at_desc(controller, index);
}

ref(lazy_array_list_node_t) __lazy_array_list_at_asc(ref(lazy_array_list_controller_t) controller, unsigned int target_index){
    unsigned int index;
    ref(lazy_array_list_node_t) target = controller->first;     
    for(index = 0 ; target && index < target_index ; target = target->next, index++);
    return target;
}

ref(lazy_array_list_node_t) __lazy_array_list_at_desc(ref(lazy_array_list_controller_t) controller, unsigned int target_index){
    unsigned int index;
    ref(lazy_array_list_node_t) target = controller->last;     
    for(index = controller->length-1 ; target && index > target_index ; target = target->prev, index--);
    return target;
}

new (lazy_array_list_node_t) __lazy_array_list_make_node(
    ref(lazy_array_list_controller_t) controller,
    ref(void) new_data, 
    ref(lazy_array_list_node_t) prev, 
    ref(lazy_array_list_node_t) next)
{
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), 0);
    if(next && prev && (next->prev != prev || prev->next != next)){
        lazy_error("malformed next & prev pair for new lazy_array_list node", __FUNCTION__, __LINE__);
        return 0;
    }
    new (lazy_array_list_node_t) this_node = malloc(sizeof(lazy_array_list_node_t));

    this_node->next = next;
    this_node->prev = prev;
    this_node->data = new_data;
    if(!controller->first)
        controller->first = controller->last = this_node;
    if(prev == controller->last)
        controller->last = this_node;
    if(next == controller->first)
        controller->first = this_node;
    if(next)
        next->prev = this_node;
    if(prev)
        prev->next = this_node;
    controller->length++;
    return this_node;
}

void __lazy_array_list_free_node(ref(lazy_array_list_controller_t) controller, ref(lazy_array_list_node_t) node)
{
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    check_or_do_and_return(node,
                           lazy_error("node is null", __FUNCTION__, __LINE__), );
    if (controller->deallocator && node->data)
    {
        controller->deallocator(node->data);
    }
    else if(node->data)
    {
        free(node->data);
    }
    if(node->prev)
        node->prev->next = node->next;
    if(node->next)
        node->next->prev = node->prev;
    if (node == controller->first)
        controller->first = node->next;
    if(node == controller->last)
        controller->last = node->prev;
    controller->length--;
    free(node);
}

void lazy_array_list_push_front(ref(lazy_array_list_controller_t) controller, ref(void) new_data)
{
    check_or_do_and_return(controller,
                           lazy_error("controller is null", __FUNCTION__, __LINE__), );
    check_or_do_and_return(new_data,
                           lazy_error("new_data is null", __FUNCTION__, __LINE__), );
    controller->first = __lazy_array_list_make_node(controller, new_data, 0, controller->first);
}

void lazy_array_list_push_back(ref(lazy_array_list_controller_t) controller, ref(void) new_data)
{
    check_or_do_and_return(controller,
                           lazy_error("controller is null", __FUNCTION__, __LINE__), );
    check_or_do_and_return(new_data,
                           lazy_error("new_data is null", __FUNCTION__, __LINE__), );
    controller->last = __lazy_array_list_make_node(controller, new_data, controller->last, 0);
}

void lazy_array_list_pop_front(ref(lazy_array_list_controller_t) controller){
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    __lazy_array_list_free_node(controller, controller->first);
}

void lazy_array_list_pop_back(ref(lazy_array_list_controller_t) controller){
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    __lazy_array_list_free_node(controller, controller->last);
}

void lazy_array_list_insert(ref(lazy_array_list_controller_t) controller, unsigned int index, ref(void) new_data) {
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    ref(lazy_array_list_node_t) node =  lazy_array_list_at(controller, index);
    check_or_do_and_return(node,
                           lazy_error("unable to get node for index", __FUNCTION__, __LINE__), );
    __lazy_array_list_make_node(controller, new_data, node->prev, node);
}

void lazy_array_list_remove(ref(lazy_array_list_controller_t) controller, unsigned int index){
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    ref(lazy_array_list_node_t) node =  lazy_array_list_at(controller, index);
    check_or_do_and_return(node,
                           lazy_error("unable to get node for index", __FUNCTION__, __LINE__), );
    __lazy_array_list_free_node(controller, node);
}

void lazy_array_list_remove_set(ref(lazy_array_list_controller_t) controller,  unsigned int index, unsigned int size){
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    ref(lazy_array_list_node_t) node =  lazy_array_list_at(controller, index);
    check_or_do_and_return(node,
                           lazy_error("unable to get node for index", __FUNCTION__, __LINE__), );
    ref(lazy_array_list_node_t) start_node = node->prev;
    ref(lazy_array_list_node_t) aux;
    int i = 0;
    for(i = 0 ; node && i < size; i++){
        aux = node->next;
        controller->length--;
        if(node==controller->last){
            controller->last = start_node;
            aux = 0;
        }
        if(controller->deallocator)
            controller->deallocator(node->data);    
        else 
            free(node->data);
        free(node);
        node=aux;
    }
    if(start_node){
        start_node->next=node;
        if(node)
            node->prev = start_node;
    } else {
        controller->first=node;
        if(node)
            node->prev = 0;
    }
}

void lazy_array_list_replace(ref(lazy_array_list_controller_t) controller,  unsigned int index, unsigned int size, ref(void) new_data){
    check_or_do_and_return(controller,
                           lazy_error("controller_ptr is null", __FUNCTION__, __LINE__), );
    const int sized_index = index+size;
    check_or_do_and_return(sized_index <= controller->length,
                           lazy_error("malformed pair of index & size for replacement, index + size sould be <= list.length", __FUNCTION__, __LINE__), );
    lazy_array_list_remove_set(controller, index, size);
    if(index < controller->length)
        lazy_array_list_insert(controller,index, new_data);   
    else
        lazy_array_list_push_back(controller, new_data);
}
