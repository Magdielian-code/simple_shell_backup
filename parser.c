#include "my_shell.h"

int isCommand(info_t *info, char *path) {

    struct stat st;

    (void)info;

    if (!path || stat(path, &st))

        return 0;

    if (st.st_mode & S_IFREG)

        return 1;

    return 0;

}

char *copyChars(char *pathString, int start, int stop) {

    static char buffer[1024];

    int i = 0, k = 0;

    for (k = 0, i = start; i < stop; i++)

        if (pathString[i] != ':')

            buffer[k++] = pathString[i];

    buffer[k] = '\0';

    return buffer;

}

char *findCommandPath(info_t *info, char *pathString, char *command) {

    int i = 0, currPos = 0;

    char *path;

    if (!pathString)

        return NULL;

    if ((_strlen(command) > 2) && startsWith(command, "./")) {

        if (isCommand(info, command))

            return command;

    }

    while (1) {

        if (!pathString[i] || pathString[i] == ':') {

            path = copyChars(pathString, currPos, i);

            if (!*path)

                _strcat(path, command);

            else {

                _strcat(path, "/");

                _strcat(path, command);

            }

            if (isCommand(info, path))

                return path;

            if (!pathString[i])

                break;

            currPos = i;

        }

        i++;

    }

    return NULL;

}
