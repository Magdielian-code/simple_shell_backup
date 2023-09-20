#include "my_shell.h"

void printErrorMessage(char *message) {
    int i = 0;

    if (!message)
        return;

    while (message[i] != '\0') {
        writeErrorMessageCharacter(message[i]);
        i++;
    }
}

int writeErrorMessageCharacter(char c) {
    static int i;
    static char buffer[WRITE_BUFFER_SIZE];

    if (c == FLUSH_BUFFER || i >= WRITE_BUFFER_SIZE) {
        writeToErrorFileDescriptor(2, buffer, i);
        i = 0;
    }

    if (c != FLUSH_BUFFER)
        buffer[i++] = c;

    return 1;
}

int writeCharacterToFileDescriptor(char c, int fd) {
    static int i;
    static char buffer[WRITE_BUFFER_SIZE];

    if (c == FLUSH_BUFFER || i >= WRITE_BUFFER_SIZE) {
        writeToErrorFileDescriptor(fd, buffer, i);
        i = 0;
    }

    if (c != FLUSH_BUFFER)
        buffer[i++] = c;

    return 1;
}

int writeStringToFileDescriptor(char *str, int fd) {
    int i = 0;

    if (!str)
        return 0;

    while (*str) {
        i += writeCharacterToFileDescriptor(*str++, fd);
    }

    return i;
}
