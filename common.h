#pragma once

typedef struct hello_s {
    int number;
    char message[32];
} hello_t;

typedef struct fubar_data_s {
    bool has_data;
    char name[32];
    unsigned int counter;
    unsigned int second_counter;
    hello_t hello;
    bool world;
} fubar_data_t;

fubar_data_t create_fixed_fubar_data();
void hello_world();

