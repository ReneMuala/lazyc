#include "macros.h"
#include "memory/resource.h"
#include <string.h>
#include <stdio.h>
#include "collections/array_list.h"

void test_memory(){
    void *test;
    void *test2 = lazy_move(&test);

    lazy_controller_t controller;

    lazy_controller_init(get_ref(controller));

    ref(void) data;
    
    lazy_make_resource(get_ref(controller),get_ref(data),10, 0);

    /// use data
    strcpy(data, "hello world");
    printf("%s\n", data);

    lazy_free_resources(get_ref(controller));
    /// data is null
    printf("%s\n", data);
}

void test_collections(){
    lazy_array_list_controller_t controller;
    lazy_array_list_init(&controller);

    int i  = 0;
    for(;i < 10 ; i++){
         new(void) data = malloc(sizeof(int));
        ref(int) data_int = data;
        *data_int = i;

        lazy_array_list_push_back(&controller, data);
    }

//    lazy_array_list_remove(&controller, 99);
    lazy_array_list_remove_set(&controller, 6, 4);
        new(void) data = malloc(sizeof(int));
        ref(int) data_int = data;
        *data_int = 100;
    //lazy_array_list_push_back(&controller, data);
    //lazy_array_list_replace(&controller, 6, 4, data);
    lazy_array_list_insert(&controller, 0, data);   
    printf("list len: %d\n", controller.length);
    for(i = 0;i < controller.length ; i++){
        ref(lazy_array_list_node_t) data_int = lazy_array_list_at(&controller, i);
        printf("%d\n",*(int*)data_int->data);
    }
    lazy_array_list_deinit(&controller);
    //lazy_array_list_remove(&controller, 0);
}

int main(){
    //test_memory();
    test_collections();
}