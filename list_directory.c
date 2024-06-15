#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    d = opendir(argv[1]);
    if (d == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }
    
    printf("Listing files in directory: %s\n", argv[1]);
    while ((dir = readdir(d)) != NULL) {
        printf("%s\n", dir->d_name);
    }
    
    if (closedir(d) == -1) {
        perror("closedir");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

