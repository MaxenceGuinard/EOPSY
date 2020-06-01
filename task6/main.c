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

bool is_file_existing(const char* filename);

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
            // With -m
            if (is_file_existing(argv[2]))
            {
                copy_mmap(argv[2], argv[3]);
            }
            else
            {
                printf("No such file named %s\n", argv[2]);
                help();
                return 0;
            }  
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
                printf("\nInvalid number of arguments\n");
                help();
                return 0;
            }
            
            // Without -m
            if(is_file_existing(argv[1]))
            {
                copy_read_write(argv[1], argv[2]);
            }
            else
            {
                printf("No such file named %s\n", argv[1]);
                help();
                return 0;
            }
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
    size_t sourceFileSize = 0;

    // Open source file
    source = open(fd_from, O_RDONLY);

    // Compute the size of the source file
    sourceFileSize = lseek(source, 0, SEEK_END);

    // Copy source file in memory
    sourcePtr = mmap(NULL, sourceFileSize, PROT_READ, MAP_PRIVATE, source, 0);

    // Create a file named 'argv[3]' with the rights 0666
    target = open(fd_to, O_CREAT | O_RDWR, 0666);

    // Set the byte length
    ftruncate(target, sourceFileSize);

    // Copy target file in memory
    targetPtr = mmap(NULL, sourceFileSize, PROT_READ | PROT_WRITE, MAP_SHARED, target, 0);

    // Replace targetPtr content by the sourcePtr content
    memcpy(targetPtr, sourcePtr, sourceFileSize);

    // Delete memory allocated to sourcePtr and targetPtr
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

bool is_file_existing(const char* filename)
{
    struct stat buffer;
    int exist = stat(filename, &buffer);
    if(exist == 0)
    {
        return true;
    }
    else
    {
        return false;
    }  
}