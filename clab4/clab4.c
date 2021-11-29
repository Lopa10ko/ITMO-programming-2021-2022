#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#pragma pack(1)
#include <string.h>
#include <math.h>

void show_usage() {
    printf("\nusage:\nlab4 --filepath=Song.mp3 --show\n");
    printf("lab4 --filepath=Song.mp3 --get=<prop_name>\n");
    printf("lab4 --filepath=Song.mp3 --set=<prop_name> --value=<prop_name>\n");
}

size_t get_bytes(FILE *filename){
    fseek(filename, 0, SEEK_END);
    size_t filesize = ftell(filename);
    rewind(filename);
    return filesize;
}

unsigned long get_size(int byte1, int byte2, int byte3, int byte4) {
    return (byte1 * (int)pow(2,24) + byte2 * (int)pow(2,16) + byte3 * (int)pow(2,8) + byte4);
}

void show(char *filename, char *trigger) {
    FILE *show_file = fopen(filename, "rb+");
    unsigned long bytes = get_bytes(show_file);
    char id3v2_data[10], *other_data = (char *)malloc(sizeof(char) * bytes);
    fread(&id3v2_data, 1, 10, show_file);
    fread(other_data, 1, bytes - 10, show_file);
    int no_pass = (strcmp(trigger, "all") == 0)? 0 : 1;
    //parsing other_data
    printf("%s\n", id3v2_data);
    for (int cnt = 0; other_data[cnt] != 0; cnt++) {
        char tag[5] = {other_data[cnt], other_data[cnt + 1], other_data[cnt + 2], other_data[cnt + 3], '\0'};
        int print_condition = (strcmp(tag, trigger) == 0 && no_pass == 1 || no_pass == 0);
        //printf("%d", print_condition);
        if(print_condition) {printf("%s ", tag);}
        cnt += 4;
        unsigned long end_tag = get_size(other_data[cnt], other_data[cnt + 1], other_data[cnt + 2], other_data[cnt + 3]);
        cnt += 6;
        for (int j = 0; j < end_tag; j++, cnt++) {
            if (print_condition) {printf("%c", other_data[cnt]);}
        }
        if (print_condition) {printf("\n");}
        cnt--;
    }
    //fread(&test, sizeof(ID3V2), 1, filename);
    //printf("\n%s", test.tag);
    fclose(show_file);
    free(other_data);
}

void change_tags(char *filename, char *tag, char *value_name) {
    FILE *change_tags_file = fopen(filename, "ab+");
    unsigned long bytes = get_bytes(change_tags_file);
    char *data = (char *)malloc(sizeof(char) * bytes);
    char *value = malloc(strlen(value_name) + 2);
    value[0] = ' ';
    fread(data, 1, bytes, change_tags_file);
    for (int i = 0; i < strlen(value_name); i++) {
        value[i + 1] = value_name[i];
    }
    printf("%s", value);
    for (unsigned long long cnt = 0; cnt < bytes - 4; cnt++) {
        if (data[cnt] == tag[0] && data[cnt + 1] == tag[1] && data[cnt + 2] == tag[2] && data[cnt + 3] == tag[3]) {
            FILE *temp_file = fopen("temp_file.mp3", "ab+");
            char buffer[1000] = {0};
            //metadata copy within original tag_length
            fwrite(data, 1, cnt + 10, temp_file);
            cnt += 7;
            int length = data[cnt];
            //fwrite(" ", 1, 1, temp_file);
            for (int i = 0; i < length; i++) {
                const char *temp = (const char *) &value[i];
                if (i >= strlen(value)) {temp = " ";}
                fwrite(temp, 1, 1, temp_file);
            }
//            unsigned long long strlength = strlen(value);
//            while (strlength < length) {
//                fwrite(" ", 1, 1, temp_file);
//                strlength++;
//            }
            fseek(change_tags_file, 3 + cnt + length, SEEK_SET);
            while (!feof(change_tags_file)) {fwrite(&buffer, 1, fread(&buffer, 1, 1000 * sizeof(char), change_tags_file) * sizeof(char), temp_file);}
            fclose(change_tags_file);
            rewind(temp_file);
            fclose(temp_file);
            temp_file = fopen("temp_file.mp3", "rb+");
            change_tags_file = fopen(filename, "wb+");
            while (!feof(temp_file)) {fwrite(&buffer, 1, fread(&buffer, 1, 1000 * sizeof(char), temp_file) * sizeof(char), change_tags_file);}
            fclose(temp_file);
            remove("temp_file.mp3");
            break;
        }
    }
    free(data);
    free(value);
}


typedef struct {
    char tag[2];
    char major;
    char version;
    char flag;
    int tag_size;
} ID3V2;

int main(int argc, char *argv[]) {
    if (argc == 3 || argc == 4) {
        char *tag_name, *value_name, *tag_change, *song_name;
        song_name = strtok(argv[1], "=");
        if (song_name != NULL) {song_name = strtok(NULL, "=");}
        char *filename = song_name;
        if (fopen(filename, "rb") == NULL) {
            perror("\nSomething wrong with mp3 file.\n");
            return -1;
        }
        if (strcmp(argv[2], "--show") == 0) {show(filename,"all");}
        else if (strncmp(argv[2], "--get=xxx", 6) == 0) {
            tag_name = strtok(argv[2], "=");
            if (tag_name != NULL) {tag_name = strtok(NULL, "=");}
            show(filename, tag_name);
        }
        else if (strncmp(argv[2], "--set=xxx", 6) == 0 && strncmp(argv[3], "--value=xxx", 8) == 0) {
            tag_change = strtok(argv[2], "=");
            if (tag_change != NULL) {tag_change = strtok(NULL, "=");}
            value_name = strtok(argv[3], "=");
            if (value_name != NULL) {value_name = strtok(NULL, "=");}
            change_tags(filename, tag_change, value_name);
        }
        return 0;
    }
    else {show_usage(); return -1;}
}




