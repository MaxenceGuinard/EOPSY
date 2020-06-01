#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 8096

void help();

void copy_mmap(char* fd_from, char* fd_to);
void copy_read_write(char* fd_from, char* fd_to);

int main(int argc, char** argv) 
{
    

    if (argc == 1 || argc > 4 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-H") == 0)
    {
        help();
        return 0;
    }
    else
    {
        if (strcmp(argv[1], "-m") == 0 && argc == 4)
        {
            int source, target;
            // With -m
            copy_mmap(argv[2], argv[3]);
        }
        else
        {
            if (strcmp(argv[1], "-m") == 0)
            {
                printf("\nInvalid number of arguments\n");
                help();
                return 0;
            }
            
            if (*argv[1] == *"-")
            {
                printf("\ncopy: invalid option -- '%s'\n", argv[1]);
                help();
                return 0;
            }
            
            if (argc != 3)
            {
                printf("\nInvalid number of argumentsnew\n");
                help();
                return 0;
            }
            
            // Without -m
            copy_read_write(argv[1], argv[2]);
        }
    }
}

void help()
{
    printf("\ncopy [-m] <file_name> <new_file_name>\n");
    printf("copy [-h]\n\n\n");
    printf("  -m use memory map\n");
    printf("  -h, --help display this help and exit\n\n");
}

void copy_mmap(char* fd_from, char* fd_to)
{
    int source, target;
    char *sourcePtr, *targetPtr;
    size_t sourceFileSize;

    // Open source file
    source = open(fd_from, O_RDONLY);
    sourcePtr = mmap(NULL, sourceFileSize, PROT_READ, MAP_PRIVATE, source, 0);

    sourceFileSize = lseek(source, 0, SEEK_END);

    target = open(fd_to, O_CREAT | O_WRONLY, 0666);

    ftruncate(target, sourceFileSize);
    
    targetPtr = mmap(NULL, sourceFileSize, PROT_READ | PROT_WRITE, MAP_SHARED, target, 0);

    memcpy(targetPtr, sourcePtr, sourceFileSize);

    munmap(sourcePtr, sourceFileSize);
    munmap(targetPtr, sourceFileSize);

    printf("%s successfully copied in %s ... \n", fd_from, fd_to);

    close(source);
    close(target);

}

void copy_read_write(char* fd_from, char* fd_to)
{
    char buffer[BUFF_SIZE];
    int readfile, source, target; 

    // Open source file
    source = open(fd_from, O_RDONLY);
            
    // Create a file named 'argv[2]' with the rights 0666 
    target = open(fd_to, O_CREAT | O_WRONLY, 0666);   

    readfile = read(source, buffer, sizeof(buffer));

    write(target, buffer, readfile);

    printf("%s successfully copied in %s ... \n", fd_from, fd_to);

    close(source);
    close(target);
}