#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//access_log is a file with over 1.8*10**6 lines
void show_usage(){
    printf("\nusage: lab2 <D> <H> <M> <S>\nD- days quantity; H - hours quantity; M - minutes quantity; S - seconds quantity.\n");
}
unsigned long get_max(unsigned long a, unsigned long b) {return((a>=b) ? a : b);}
unsigned long get_seconds(const char * time_string) {
    unsigned long seconds = 0;
    char character;
    for (int i = 0; i < strlen(time_string); i++) {
        character = time_string[i];
        if (i == 1){seconds += 10*24*3600*(character - '0');}
        if (i == 2){seconds += 24*3600*(character - '0');}
        if (i == 13){seconds += 10*3600*(character - '0');}
        if (i == 14){seconds += 3600*(character - '0');}
        if (i == 16){seconds += 10*60*(character - '0');}
        if (i == 17){seconds += 60*(character - '0');}
        if (i == 19){seconds += 10*(character - '0');}
        if (i == 20){seconds += (character - '0');}
    }
    return seconds - 86400;
    //strtol
}

void parse_lines_time(FILE *filename, unsigned long time_param) {
    unsigned long cnt = 0, max_cnt = 0;
    size_t buffer_size = 300;
    char *string = malloc(buffer_size), *buffer_string = malloc(buffer_size), *pch, str[300];
    //index of tokenization
    int index = 0, index_array = 0;

    //iterative lines counter
    while (fgets(string, 300, filename) != NULL) {cnt++;}
    fclose(filename);
    unsigned long cnt1 = 0;


    //unsigned long time_array[cnt];
    unsigned long *time_array = NULL;
    time_array = (unsigned long *) malloc(cnt*sizeof(unsigned long));
    // time_array generation and conversion: time -> int
    filename = fopen("access_log_Jul95", "r");
    while (fgets(string, 300, filename) != NULL) {
        //tokenization
        pch = strtok(string," ");
        index = 0;
        while (pch != NULL){
            if(index == 3) {
                strcpy(buffer_string,pch);
                time_array[index_array] = get_seconds(buffer_string);
                index_array++;
                //printf("%s ", pch);
            }
            pch = strtok(NULL," ");
            index++;
        }
    }

    free(buffer_string);
    fclose(filename);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    // getting the first and the last indexes of time in seconds
//    unsigned long first_index = 0, last_index = 0, counter = 1, max_counter = 1;
//    for (int i = 0; i < cnt; ++i) {
//        counter = 1;
//        //printf("%lu\n", time_array[i]);
//        for (int j = i + 1; j < cnt; ++j) {
//            if (time_array[i] + time_param - 1 >= time_array[j]) {counter++;}
//        }
//        if(counter >= max_counter){max_counter = counter; first_index = i; last_index = i + counter - 1;}
//    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// getting the first and the last indexes of time in seconds
    unsigned long first_index = 0, last_index = 0, max_counter = 1, temp = 0, first = 0, last = 0;
    while (last_index <= cnt) {
        temp = time_array[last_index] - time_array[first_index];
        while (temp > time_param) {first_index++; temp = time_array[last_index] - time_array[first_index];}
        if (temp <= time_param) {
            if (last_index - first_index > max_counter) {max_counter = last_index - first_index; last = last_index; first = first_index;}
        }
        last_index++;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned long answer = last - first + 1;
    printf("there are %lu request(s)\n", answer);
    printf("%lu %lu\n", last, first);
    // get strings with time in original format
    filename = fopen("access_log_Jul95", "r");
    index = 0;
    while (fgets(string, 300, filename) != NULL) {
        if (index == first || index == last){
            printf("%s", string);
        }
        index++;
    }
    free(time_array);
    free(string);
    fclose(filename);
}
int main(int argc, char *argv[]) {
    if (argc != 5){show_usage(); return -1;}
    char *end;
    //DD HH MM SS in seconds
    unsigned long time_param = (strtol(argv[1], &end, 10)) * 3600 * 24 + (strtol(argv[2], &end, 10)) * 3600  + (strtol(argv[3], &end, 10)) * 60 + strtol(argv[4], &end, 10);
    FILE *filename = fopen("access_log_Jul95", "r");
    //catch errors with file opening and unexpected time_param
    if (filename == NULL || time_param >= 31 * 24 * 60 * 60) {
        perror("\nSomething wrong with log file or with value of time parameter.\n");
        exit(1);
    } else { parse_lines_time(filename, time_param); }
    return 0;
}