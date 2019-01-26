#include "common.h"
#include <string.h>
#include <stdio.h>

fubar_data_t fubar_data_create_fixed()
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

void fubar_data_print(const fubar_data_t& fb)
{
    printf("fb.has_data = %d\n", fb.has_data);
    printf("fb.name = %s\n", fb.name);
    printf("fb.counter = %d\n", fb.counter);
    printf("fb.second_counter = %d\n", fb.second_counter);
    printf("fb.hello.number = %d\n", fb.hello.number);
    printf("fb.hello.message = %s\n", fb.hello.message);
    printf("fb.name = %d\n", fb.world);
}

void hello_world()
{
    printf("hello world\n");
}

