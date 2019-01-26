#include "common.h"
#include <string.h>
#include <stdio.h>

fubar_data_t create_fixed_fubar_data()
{
    fubar_data_t data;
    data.has_data = true;
    strncpy(data.name, "this is my name", sizeof(data.name));
    data.counter = 12;
    data.second_counter = 89;
    data.hello.number = 42;
    strncpy(data.hello.message, "hello world", sizeof(data.hello.message));
    data.world = false;
    return data;
}

void hello_world()
{
    printf("hello world\n");
}

