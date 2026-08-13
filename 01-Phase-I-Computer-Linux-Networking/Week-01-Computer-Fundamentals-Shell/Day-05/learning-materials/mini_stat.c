#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


void format_permissions(mode_t mode, char *perm_str) {

    perm_str[0] = (mode & S_IRUSR) ? 'r' : '-';
    perm_str[1] = (mode & S_IWUSR) ? 'w' : '-';
    perm_str[2] = (mode & S_IXUSR) ? 'x' : '-';


    perm_str[3] = (mode & S_IRGRP) ? 'r' : '-';
    perm_str[4] = (mode & S_IWGRP) ? 'w' : '-';
    perm_str[5] = (mode & S_IXGRP) ? 'x' : '-';

    perm_str[6] = (mode & S_IROTH) ? 'r' : '-';
    perm_str[7] = (mode & S_IWOTH) ? 'w' : '-';
    perm_str[8] = (mode & S_IXOTH) ? 'x' : '-';


    perm_str[9] = '\0';
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    struct stat st;

    
    if (stat(filename, &st) != 0) {
        perror("Error inspecting file");
        return EXIT_FAILURE;
    }

    
    char perm_str[10];
    format_permissions(st.st_mode, perm_str);


    printf("=========================================\n");
    printf("         INODE INSPECTOR (mini_stat)     \n");
    printf("=========================================\n");
    printf(" File Name   : %s\n", filename);
    printf(" Inode Number: %lu\n", (unsigned long)st.st_ino);
    printf(" Link Count  : %lu\n", (unsigned long)st.st_nlink);
    printf(" UID         : %u\n", (unsigned int)st.st_uid);
    printf(" GID         : %u\n", (unsigned int)st.st_gid);
    printf(" Size (Bytes): %lld\n", (long long)st.st_size);
    printf(" Permissions : %s\n", perm_str);
    printf("=========================================\n");

    return EXIT_SUCCESS;
}