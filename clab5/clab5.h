#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint-gcc.h>
#ifndef MAIN_C_LAB3_H
#define MAIN_C_LAB3_H

typedef struct {
    char bfType[2]; // format mark
    unsigned long bfSize; // struct.size()
    unsigned short bfReserved1; // == null
    unsigned short bfReserved2; // == null
    unsigned long bfOffBits; // == start_byte of pixel data
} BIT_MAP_FILE_HEADER;

typedef struct {
    unsigned long Size; // struct.size()
    long Width; // width
    long Height; // height
    unsigned short Planes; // == 1, for Win-cursor only
    unsigned short BitCount; // bit per pixel
    unsigned long Compression; // way of pixel formatting
    unsigned long SizeImage; // pixel_data.size() | 0
    long XPelsPerMeter; // bit per meter x-wise
    long YPelsPerMeter; // bit per meter y-wise
    unsigned long ClrUsed; // color.size()
    unsigned long ClrImportant; // start and end of color struct
} BIT_MAP_INFO;

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char rgbReserved;
} RGB;

#endif //MAIN_C_LAB3_H