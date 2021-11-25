#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*void word_count(char filename){
    unsigned cnt = 0;
    FILE *cfPtr;
    char s[1000]
    //catch {} fileERROR
    if((cfPtr = fopen("filename", "r")) == NULL){
        perror("Something wrong with the file");
        return -1
    }
}*/
void show_usage(){
    printf("\nusage: lab1 [-b, -l, -w] <FILENAME>\n -b --file bytes quantity\n -l --file lines quantity\n -w --file words quantity\n");
 }
void show_file_mask(){
    printf("\nRequested file does not exist. Try putting filename in <filename.txt> mask.\n");
}
void get_bytes(FILE *filename){
    fseek(filename, 0, SEEK_END);
    unsigned long long filesize = ftell(filename);
    rewind(filename);
    printf("%llu byte(s)",  filesize);
}
void get_lines(FILE *filename){
    unsigned int cnt = 0;
    int flag = 0;
    char tmp;
    for (tmp = getc(filename); tmp != EOF; tmp = getc(filename)){
        cnt = (tmp == '\n') ? cnt + 1 : cnt;
        flag = 1;
    }
    printf("%u line(s)", (flag == 1) ? cnt + 1 : cnt);
}
void get_words(FILE *filename){
    unsigned long long cnt = 0;
    size_t buffer_size = 300;
    char *string = malloc(buffer_size), *pch;
    int symb;
    while (fgets(string, 300, filename) != NULL) {
        pch = strtok(string," \n\b-");
        while (pch != NULL){
            if (strncmp(pch, "\bxx", 1) != 0){
                printf("%s\n", pch);
                cnt++;
                pch = strtok(NULL," \n\b-");
            }
        }
    }
    free(string);
    //for (symb = getw(filename); symb != EOF; symb = getw(filename)){cnt++;}
    printf("%llu word(s)", cnt);
}

int main(int argc, char* argv[]){
    FILE *filename;
    //for( count = 0; count < argc; count++ ){printf_s("argv[%d] %s\n", count, argv[count]);}
    //catch unsatisfied input requirements
    if (argc < 3 || (strcmp(argv[1], "-b") != 0 && strcmp(argv[1], "-l") != 0 && strcmp(argv[1], "-w") != 0)){
        printf("Input requirements not satisfied.\n");
        show_usage();
        return -1;
    }
    //catch not existing files or wrong form of filenames
    if ((filename = fopen(argv[2], "a+")) == NULL){
        show_file_mask();
        fclose(filename);
        return -1;
    }
    if (strcmp(argv[1], "-l") == 0){get_lines(filename);}
    if (strcmp(argv[1], "-w") == 0){get_words(filename);}
    if (strcmp(argv[1], "-b") == 0){get_bytes(filename);}

    fclose(filename);
    return 0;
}
