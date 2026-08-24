#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    // Silently tries to open a required configuration file in /etc/
    int fd = open("/etc/custom_app_config.json", O_RDONLY);
    if (fd < 0) {
        // Exits silently with code 1 without printing anything to stdout/stderr
        exit(1);
    }
    close(fd);
    return 0;
}
