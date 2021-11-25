//
// Created by Georgy on 13.09.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint-gcc.h>
#ifndef MAIN_C_LAB2_H
#define MAIN_C_LAB2_H

// supremum = 2**1024 = 1797...37216
// len(supremum) = 309
// on top of uint64 --> uint1024

typedef struct {
    uint64_t *values;
}uint1024_t;

// prolong math for the uint1024 field
uint1024_t add_op(uint1024_t x, uint1024_t y, int base);
uint1024_t subtr_op(uint1024_t x, uint1024_t y, int base);

// Karatsuba's method for prolong multiplication
uint1024_t mult_op(uint1024_t x, uint1024_t y, int base);

// uint1024 generation
uint1024_t from_uint(unsigned int x, int base);
uint1024_t from_string(const char * string, int base);

// I/O
void printf_value(uint1024_t x, int base);
void scanf_value(uint1024_t* x, int base);



#endif //MAIN_C_LAB2_H
