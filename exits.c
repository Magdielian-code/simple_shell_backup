#include "my_shell.h"

char *copyStringWithLimit(char *destination, char *source, int limit) {
    int i = 0, j = 0;
    char *result = destination;

    while (source[i] != '\0' && i < limit - 1) {
        destination[i] = source[i];
        i++;
    }

    if (i < limit) {
        j = i;
        while (j < limit) {
            destination[j] = '\0';
            j++;
        }
    }

    return result;
}

char *concatenateStrings(char *destination, char *source, int limit) {
    int i = 0, j = 0;
    char *result = destination;

    i = 0;
    j = 0;
    while (destination[i] != '\0')
        i++;

    while (source[j] != '\0' && j < limit) {
        destination[i] = source[j];
        i++;
        j++;
    }

    if (j < limit)
        destination[i] = '\0';

    return result;
}

char *findCharacterInString(char *string, char characterToFind) {
    do {
        if (*string == characterToFind)
            return string;
    } while (*string++ != '\0');

    return NULL;
}
