#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <string_to_write>\n", argv[0]);
        return 1;
    }

    // Open the specified file in write-only mode, creating it if it doesn't exist
    //O_WRONLY: open for writing only
    //O_CREAT: create the file if it doesn't exist
    //O_TRUNC: if the file already exists and is open for write, it's length is trunc to 0
    //0644: file permissions
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644); 

    if (fd == -1) { // if file cannot be opened or created
        perror("Error opening/creating file");
        return 2;
    }

    const char *str = argv[2];//write the string from argv[2]
    ssize_t bytesToWrite = strlen(str);//number of bytes to write
    ssize_t bytesWritten;

    bytesWritten = write(fd, str, bytesToWrite);//writes the string to the file descriptor

    if (bytesWritten == -1) { // if there was an error writing to the file
        perror("Error writing to file");
        close(fd);
        return 3;
    }

    if (bytesWritten < bytesToWrite) {//checks if all bytes were written to the file.
        printf("Warning: Not all bytes were written to the file.\n");
    }

    close(fd); // close the file descriptor
    return 0;
}
