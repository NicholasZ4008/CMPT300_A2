#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// argc = number of arguments passed to the program
// argv = array of strings, each string is a command-line argument
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);// open the specified file in read-only mode with open()

    if (fd == -1) {// if file cannot be opened
        perror("Error opening file");
        return 2;
    }

    char buffer[1024];
    ssize_t bytesRead;

    //read up to 1023 bytes from the file into buffer
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';  // Null-terminate the string
        printf("%s\n", buffer);//print buffer's contents
    }

    if (bytesRead == -1) {//if there was an error reading from file
        perror("Error reading from file");
        close(fd);
        return 3;
    }

    close(fd);//close file descriptor then exit.
    return 0;
}
