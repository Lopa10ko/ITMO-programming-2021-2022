#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 256

void show_usage() {
    printf("\nusage:\nlab6.exe arc --file [filename.arc] --create FILE1 FILE2 ...\nlab6.exe arc --file [filename.arc] --list\nlab6.exe arc --file [filename.arc] --extract\n");
}

unsigned long long get_bytes(FILE *filename) {
    fseek(filename, 0, SEEK_END);
    unsigned long long filesize = ftell(filename);
    rewind(filename);
    return filesize;
}

void extract_archive(char* archive) {
    FILE* archive_file = fopen(archive, "rb+wb"), *file;
    unsigned long long start_set = 1, current_set = 0, ch, filesize;
    char filename[BUFFER_SIZE];
    while ((ch = getc(archive_file)) != '\n') {start_set++;}
    fseek(archive_file, 0, SEEK_SET);
    while (fscanf(archive_file, "/%s %llu ", filename, &filesize) != 0) {
        if ((file = fopen(filename, "wb+")) != NULL) {
            current_set = ftell(archive_file);
            fseek(archive_file, start_set, SEEK_SET);
            start_set += filesize;
            while (filesize-- > 0) {putc((ch = getc(archive_file)), file);}
            fseek(archive_file, current_set, SEEK_SET);
            fclose(file);
        }
    }
}

void list_archive(char* archive) {
    FILE* archive_file = fopen(archive, "rb");
    char filename[BUFFER_SIZE];
    unsigned long long filesize;
    while (fscanf(archive_file, "/%s %llu ", filename, &filesize) != 0) {printf("%s - %llu bytes\n", filename, filesize);}
    fclose(archive_file);
}

int main(int argc, char* argv[]) {
    char* archive = "data.arc";
    if (argc < 5 || strcmp(argv[2], "--file") != 0 || (strcmp(argv[4], "--create") != 0 && strcmp(argv[4], "--extract") != 0 && strcmp(argv[4], "--list") != 0) != 0) {
        printf("Input requirements not satisfied.\n");
        show_usage();
        return -1;
    }
    if (strcmp(argv[4], "--create") == 0 && strcmp(argv[5], " ") != 0) {
        FILE* archive_file = fopen(archive = argv[3], "wb"), *filename;
        unsigned long long file_sizes[BUFFER_SIZE], ch;
        for (int i = 5; i < argc; i++) {
            if ((filename = fopen(argv[i], "rb")) != NULL) {
                file_sizes[i - 5] = get_bytes(filename);
                fclose(filename);
            }
        }
        for (int i = 0; i < argc - 5; i++) {fprintf(archive_file, "/%s %llu ", argv[i + 5], file_sizes[i]);}
        fprintf(archive_file, "\n");
        for (int i = 5; i < argc; i++) {
            if ((filename = fopen(argv[i], "rb")) != NULL) {
                while ((ch = getc(filename)) != EOF) {putc(ch, archive_file);}
                fclose(filename);
            } else {
                printf("File have null properties or some other requirements are not satisfied.\n");
                return -1;
            }
        }
    }
    else if (strcmp(argv[4], "--extract") == 0 && argc == 5) {extract_archive(archive);}
    else if (strcmp(argv[4], "--list") == 0 && argc == 5) {list_archive(archive);}
    return 0;
}