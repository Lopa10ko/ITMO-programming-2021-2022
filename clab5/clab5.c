#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "clab5.h"
#define FILE_STR 100
#define DIR_STR 200
//CONWAY'S GAME OF LIFE

void show_usage(){
    printf("\nusage: [clab5.exe] --input=input_file.bmp --output=output_file --max_iter=<int_max_iter> --dump_freq=<int_dump_freq>\n");
}

int get_pixel_status(RGB pixel) {
    return ((pixel.blue == 0 && pixel.green == 0 && pixel.red == 0) ? 1 : 0);
}

void create(FILE *file, BIT_MAP_FILE_HEADER header, BIT_MAP_INFO info_bmp, RGB **data) {
    fwrite(&header.bfType, 1, 2, file);
    fwrite(&header.bfSize, 4, 1, file);
    fwrite(&header.bfReserved1, 2, 1, file);
    fwrite(&header.bfReserved2, 2, 1, file);
    fwrite(&header.bfOffBits, 4, 1, file);
    fwrite(&info_bmp.Size, 4, 1, file);
    fwrite(&info_bmp.Width, 4, 1, file);
    fwrite(&info_bmp.Height, 4, 1, file);
    fwrite(&info_bmp.Planes, 2, 1, file);
    fwrite(&info_bmp.BitCount, 2, 1, file);
    fwrite(&info_bmp.Compression, 4, 1, file);
    fwrite(&info_bmp.SizeImage, 4, 1, file);
    fwrite(&info_bmp.XPelsPerMeter, 4, 1, file);
    fwrite(&info_bmp.YPelsPerMeter, 4, 1, file);
    fwrite(&info_bmp.ClrUsed, 4, 1, file);
    fwrite(&info_bmp.ClrImportant, 4, 1, file);
    for (int i = info_bmp.Height - 1; i >= 0; i--) {
        for (int j = 0; j < info_bmp.Width; j++) {
            putc(data[i][j].blue, file);
            putc(data[i][j].green, file);
            putc(data[i][j].red, file);
        }
        if (info_bmp.Width % 4 != 0) {putc(0, file);}
    }
}

RGB **tranform_RGB(int **rgb_mark, BIT_MAP_INFO info_bmp) {
    RGB **map = (RGB **)malloc(info_bmp.Height * sizeof(RGB *));
    for (int i = 0; i < info_bmp.Height; ++i) {map[i] = (RGB *)malloc(info_bmp.Width * sizeof(RGB));}
    for (int i = info_bmp.Height - 1; i >= 0; i--) {
        for (int j = 0; j < info_bmp.Width; j++) {
            if (rgb_mark[i][j] == 1) {
                map[i][j].blue = 0;
                map[i][j].green = 0;
                map[i][j].red = 0;
            } else {
                map[i][j].blue = 255;
                map[i][j].green = 255;
                map[i][j].red = 255;
            }
        }
    }
    return map;
}

//////////////////////////////////////////////////////////////////////////////////////////
void game(BIT_MAP_INFO info_bmp, FILE *file, char *dir_name, BIT_MAP_FILE_HEADER header, int max_iter, int dump_freq) {
    RGB **rgb = (RGB **)malloc(info_bmp.Height * sizeof(RGB *));
    for (int i = 0; i < info_bmp.Height; ++i) {rgb[i] = (RGB *)malloc(info_bmp.Width * sizeof(RGB));}
    for (int i = info_bmp.Height - 1; i >= 0; i--) {
        for (int j = 0; j < info_bmp.Width; j++) {
            rgb[i][j].blue = getc(file); 
            rgb[i][j].green = getc(file); 
            rgb[i][j].red = getc(file);
        }
        if (info_bmp.Width % 4 != 0) {getc(file);}
    }

    int **grid_temp = (int **) malloc(info_bmp.Height * sizeof(int *)), **grid = (int **) malloc(info_bmp.Height * sizeof(int *));
    for (int i = 0; i < info_bmp.Height; ++i) {grid_temp[i] = (int *)malloc(info_bmp.Width * sizeof(int));}
    for (int i = 0; i < info_bmp.Height; i++) {
        for (int j = 0; j < info_bmp.Width; j++) {
            grid_temp[i][j] = ((get_pixel_status(rgb[i][j]) == 1) ? 1 : 0);
        }
    }
    for (int i = 0; i < info_bmp.Height; ++i) {grid[i] = (int *) malloc(info_bmp.Width * sizeof(int));}
    for (int i = 0; i < info_bmp.Height; i++) {
        for (int j = 0; j < info_bmp.Width; j++) {
            grid[i][j] = ((get_pixel_status(rgb[i][j]) == 1) ? 1 : 0);
        }
    }
    fclose(file);
    //CONWAY'S GAME OF LIFE algorhytm
    int up, left, down, right, neighbours;
    for (int iteration = 0; iteration < max_iter; iteration++) {
        for (int i = 0; i < info_bmp.Height; i++) {
            up = i - 1;
            down = i + 1;
            if (i == 0) {up = info_bmp.Height - 1;} 
            if (i == info_bmp.Height - 1) {down = 0;}
            for (int j = 0; j < info_bmp.Width; j++) {
                left = j - 1;
                right = j + 1;
                if (j == 0) {left = info_bmp.Width - 1;}
                if (j == info_bmp.Width - 1) {right = 0;}
                neighbours = grid_temp[up][left] + grid_temp[up][j] + grid_temp[up][right] + grid_temp[i][left] + grid_temp[i][right] + grid_temp[down][left] + grid_temp[down][j] + grid_temp[down][right];
                if (grid_temp[i][j] == 1) {
                    grid[i][j] = (neighbours < 2 || neighbours > 3) ? 0 : 1;
                } else {
                    grid[i][j] = (neighbours == 3) ? 1 : 0;
                }
            }
        }
        for (int i = 0; i < info_bmp.Height; i++) {
            for (int j = 0; j < info_bmp.Width; j++) {
                grid_temp[i][j] = grid[i][j];
            }
        }
        char directory[FILE_STR], full_dir[DIR_STR];
        if (iteration % dump_freq == 0) {
            memset(directory, 0, FILE_STR);
            memset(full_dir, 0, DIR_STR);
            sprintf(directory, "%d.bmp", iteration);
            strcpy(full_dir, dir_name);
            strcat(full_dir, directory);
            file = fopen(full_dir, "w");
            create(file, header, info_bmp, tranform_RGB(grid_temp, info_bmp));
            fclose(file);
        }
    }
}

int main(int argc, char *argv[]) {
    int max_iter = 1000, dump_freq = 1;
    if (argc == 3 || argc == 5 || argc == argc) {
        int max_iter = 1000, dump_freq = 1;
        printf("---------------->");
        char *input_file = argv[1];
        char *dir_name = argv[2];
        mkdir(dir_name);
        max_iter = atoi(argv[4]);
        dump_freq = atoi(argv[6]);
        // if (strcmp(argv[2], "--show") == 0) {show(input_file,"all");}
        // else if (strncmp(argv[2], "--get=xxx", 6) == 0) {
        //     tag_name = strtok(argv[2], "=");
        //     if (tag_name != NULL) {tag_name = strtok(NULL, "=");}
        //     show(input_file, tag_name);
        // }
        FILE *file = fopen(input_file, "r");
        if (file == NULL) {show_usage(); return -1;}
        BIT_MAP_FILE_HEADER header;
        fread(&header.bfType, 1, 2, file);
        fread(&header.bfSize, 4, 1, file);
        fread(&header.bfReserved1, 2, 1, file);
        fread(&header.bfReserved2, 2, 1, file);
        fread(&header.bfOffBits, 4, 1, file);
        BIT_MAP_INFO info_bmp;
        fread(&info_bmp.Size, 4, 1, file);
        fread(&info_bmp.Width, 4, 1, file);
        fread(&info_bmp.Height, 4, 1, file);
        fread(&info_bmp.Planes, 2, 1, file);
        fread(&info_bmp.BitCount, 2, 1, file);
        fread(&info_bmp.Compression, 4, 1, file);
        fread(&info_bmp.SizeImage, 4, 1, file);
        fread(&info_bmp.XPelsPerMeter, 4, 1, file);
        fread(&info_bmp.YPelsPerMeter, 4, 1, file);
        fread(&info_bmp.ClrUsed, 4, 1, file);
        fread(&info_bmp.ClrImportant, 4, 1, file);
        game(info_bmp, file, dir_name, header, max_iter, dump_freq);
    } else {show_usage();}
    return 0;
}