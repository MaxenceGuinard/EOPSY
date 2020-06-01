#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

void help();

void copy_mmap(int fd_from, int fd_to);
void copy_read_write(int fd_from,   int fd_to);


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
            printf("\n-m section\n");
            copy_mmap(1, 1);
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
            printf("\nnormal section\n");


            FILE* source;
            FILE* destination;

            source = fopen(argv[1], "r");
            //destination = open(argv[2], "w");

            copy_read_write(1, 1);
        }
    }
}

void help()
{
    printf("\ncopy [-m] <file_name> <new_file_name>\n");
    printf("copy [-h]\n\n\n");
    printf("  -m dedicated to map files to memory regions and copy them\n");
    printf("  -h, --help display this help and exit\n\n");
}

void copy_mmap(int fd_from, int fd_to)
{

}

void copy_read_write(int fd_from,   int fd_to)
{
    /*FILE* source;
    FILE* destination;

    source = open(argv[1], "r");

    open(source, O_RDONLY);*/
}