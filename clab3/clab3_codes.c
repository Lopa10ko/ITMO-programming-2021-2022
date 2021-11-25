#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//access_log is a file with over 1.8*10**6 lines

void parse_lines(FILE *filename){
    //5XX codes only
    unsigned int cnt = 0;
    size_t buffer_size = 300;
    char *string = malloc(buffer_size), *pch, str[300];
    int index = 0, contains_5xx = 0;
    //iterative way to manage lines in the file
    while (fgets(string, 300, filename) != NULL) {
        strcpy(str, string);
        //tokenization
        pch = strtok(string," ");
        index = 0;
        while (pch != NULL){
            if(index == 8 && atoi(pch) >= 500 && strlen(pch) == 3) {
                contains_5xx = 1;
                cnt++;
                pch = strtok(NULL," ");
            } else {
                pch = strtok(NULL," ");
            }
            index++;
        }
        if(contains_5xx) {printf("%s", str);}
        contains_5xx = 0;
    }
    printf("%u", cnt);
    free(string);
    fclose(filename);
}
int main(int argc, char *argv[]) {
    FILE *filename = fopen("access_log_Jul95", "r");
    if (filename == NULL) {perror("Something wrong with log file."); exit(1);}
    else {parse_lines(filename);}
    return 0;
}