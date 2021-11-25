#include <stdio.h>
#include "clab2.h"
#include <math.h>
uint32_t get_max(uint32_t a, uint32_t b) {return (a >= b) ? a : b;}
int get_length_uint64(uint64_t value) {int cnt = 0; while(value>0){value /= 10; cnt += 1;} return cnt;}
int get_length_uint1024(uint1024_t value, int base) {int cnt = 0; for (int i = base - 1; i >= 0 ; i--) {if (value.values[i] == 0) {cnt++;} else{break;}} return base - cnt;}
int get_number_digit(uint1024_t value, int base){
    int count_digit = 0;
    int flag = 0;
    for (int i = base - 1; i >= 0 ; i--) {
        if (value.values[i] != 0) {flag = 1; count_digit += get_length_uint64(value.values[i]);}
        else if (flag == 1 && value.values[i] == 0){count_digit += 9;}
    }
    return count_digit;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int get_largest_uint1024(uint1024_t first_value, uint1024_t second_value, int base) {
    int first_cnt = 0, second_cnt = 0;
    uint64_t first_largest = 0, second_largest = 0;
    for (int i = base - 1; i > 0 ; i--) {if (first_value.values[i] == 0) {first_cnt++;}}
    for (int i = base - 1; i > 0 ; i--) {if (second_value.values[i] == 0) {second_cnt++;}}
    if (first_cnt == second_cnt){
        for (int i = base - 1; i > 0 ; i--) {
            if (first_value.values[i] != 0 && first_value.values[i] > second_value.values[i]) {return 1;}
        }
        return 0;
    }
    return (first_cnt < second_cnt) ? 1 : 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// iterative in and out
void printf_value(uint1024_t x, int base) {for (int i = 0; i < base; i++) {printf("%llu ", x.values[base - 1 - i]);}}
// x 0 -> x000000000
void scanf_value(uint1024_t * x, int base){}
// generator uint1024 from uint32
uint1024_t from_uint(unsigned int x, int base) {
    uint1024_t num;
    num.values = malloc(base * sizeof(uint64_t));
    for (int i = 1; i < base; i++) {num.values[i] = 0;}
    num.values[0] = x;
    return num;
    //free(num)
    // uint * ptr = from_uint(), free(...)
}
// generator uint1024 from string
uint1024_t from_string(const char * string, int base) {
    uint1024_t num;
    num.values = malloc(base * sizeof(uint64_t));
    for (int i = 0; i < base; i++) {num.values[i] = 0;}
    // if strlen/9 > base -> warning
    unsigned long str_size = strlen(string);
    int power = 0, lim = 3;
    for (unsigned int i = str_size; i > 0; i--){
        num.values[(str_size - i) / lim] += (string[i - 1] - '0') * pow(10, ++power - 1);
        if(power == lim) {power = 0;}
    }
    return num;
}
uint1024_t from_string_less(const char * string, int base) {
    uint1024_t num;
    num.values = malloc(base * sizeof(uint64_t));
    for (int i = 0; i < base; i++) {num.values[i] = 0;}
    // if strlen/9 > base -> warning
    unsigned long str_size = strlen(string);
    int power = 0, lim = 4;
    for (unsigned int i = str_size; i > 0; i--){
        num.values[(str_size - i) / lim] += (string[i - 1] - '0') * pow(10, ++power - 1);
        if(power == lim) {power = 0;}
    }
    return num;
}

uint1024_t add_op(uint1024_t x, uint1024_t y, int base) {
    uint32_t remaining = 0;
    for (int i = 0; i < sizeof(x.values); i++) {
        uint32_t sum = x.values[i] + y.values[i];
        x.values[i] = sum % 1000000000 + remaining;
        remaining = sum / 1000000000;
    }
    return x;
}
uint1024_t subtr_op(uint1024_t x, uint1024_t y, int base){
    // x >= y
    uint1024_t sub = x;
    //printf("%d",get_length_uint1024(x, base) );
    if(get_length_uint1024(x, base) == 1){
        printf("No need in uint1024_t: %d", (int)x.values[0] - (int)y.values[0]);
        return sub;
    }
    for (int i = 0; i < get_length_uint1024(x, base); i++) {
        if(((int)sub.values[i] - (int)y.values[i]) < 0){
            sub.values[i] += 1000000000 - y.values[i];
            sub.values[i + 1]--;
        } else {sub.values[i] -= y.values[i];}

    }
    return sub;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint1024_t mult_op(uint1024_t x, uint1024_t y, int base){
    uint1024_t mult;
    mult.values = malloc(base * sizeof(uint64_t));
    for (int i = 0; i < base; i++) {mult.values[i] = 0;}

    //x>=y
    for (int i = 0; i < get_length_uint1024(x, base); i++) {
        uint32_t remaining = 0;
        for (int j = 0; j < get_length_uint1024(y, base); j++) {
            // caracuba x.values[i]*y.values[i] 1000
            mult.values[i + j] += x.values[i] * y.values[j];
            mult.values[i + j + 1] += mult.values[i + j] / 10000;
            mult.values[i + j] %= 10000;
        }
    }
    return mult;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

    if (argc < 3 || (strcmp(argv[2], "+") != 0 && strcmp(argv[2], "-") != 0 && strcmp(argv[2], "*") != 0 && strcmp(argv[2], "-r") != 0 && strcmp(argv[2], "-g") != 0)) {
        printf("Input requirements not satisfied.\n");
        //show_usage();
        return -1;
    }
    const int MAX_BASE = 128;
    char *pch;
    int base = 64;
    if (strcmp(argv[4], " ") == 0 || strtol(argv[4], &pch, 10) > MAX_BASE){return -1;}
    else{base = strtol(argv[4], &pch, 10);}

    if (strcmp(argv[2], "-") == 0 && strcmp(argv[1], " ") != 0 && strcmp(argv[3], " ") != 0) {
        printf_value(subtr_op(from_string(argv[1], base), from_string(argv[3], base), base), base);
    }

    if (strcmp(argv[2], "+") == 0 && strcmp(argv[1], " ") != 0 && strcmp(argv[3], " ") != 0) {
        printf_value(add_op(from_string(argv[1], base), from_string(argv[3], base), base), base);
    }

    if (strcmp(argv[2], "*") == 0 && strcmp(argv[1], " ") != 0 && strcmp(argv[3], " ") != 0) {
        printf_value(mult_op(from_string_less(argv[1], base), from_string_less(argv[3], base), base), base);
    }

//    if (strcmp(argv[2], "*") == 0 && strcmp(argv[1], " ") != 0 && strcmp(argv[3], "+") != 0) {
//        printf_value(mult_op(from_string(argv[1], base), from_string(argv[3], base), base), base);
//    }


//  if (strcmp(argv[2], "-r") == 0){scanf_value(uint1024_t x);}
//  printf_value(from_string(argv[1], base), base);
//  generate
    if (strcmp(argv[2], "-g") == 0) {
        char *end;
        printf_value(from_uint(strtol(argv[1], &end, 10), base), base);
    }
    return 0;
}






















