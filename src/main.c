#include "macros.h"
#include "memory/resource.h"
#include <string.h>
#include <stdio.h>

void test_memory(){
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

int main(){
    test_memory();
}