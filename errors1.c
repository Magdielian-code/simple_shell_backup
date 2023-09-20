#include "my_shell.h"

int errorStringToInt(char *s) {
    int i = 0;
    unsigned long int result = 0;

    if (*s == '+')
        s++;  /* TODO: why does this make main return 255? */

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            result *= 10;
            result += (s[i] - '0');
            if (result > INT_MAX)
                return -1;
        } else {
            return -1;
        }
    }
    return (int)result;
}

void printErrorInfo(info_t *info, char *errorMessage) {
    printErrorString(info->fileName);
    printErrorString(": ");
    printErrorInt(info->lineCount, STDERR_FILENO);
    printErrorString(": ");
    printErrorString(info->arguments[0]);
    printErrorString(": ");
    printErrorString(errorMessage);
}

int printErrorInt(int input, int fd) {
    int (*outputCharacter)(char) = writeCharacterToErrorFileDescriptor;

    int i, count = 0;
    unsigned int absoluteValue, current;

    if (fd == STDERR_FILENO)
        outputCharacter = writeCharacterToErrorFileDescriptor;

    if (input < 0) {
        absoluteValue = -input;
        outputCharacter('-');
        count++;
    } else {
        absoluteValue = input;
    }

    current = absoluteValue;

    for (i = 1000000000; i > 1; i /= 10) {
        if (absoluteValue / i) {
            outputCharacter('0' + current / i);
            count++;
        }
        current %= i;
    }

    outputCharacter('0' + current);
    count++;

    return count;
}

char *convertNumber(long int num, int base, int flags) {
    static char *array;
    static char buffer[50];
    char sign = 0;
    char *ptr;
    unsigned long n = num;

    if (!(flags & CONVERT_UNSIGNED) && num < 0) {
        n = -num;
        sign = '-';
    }

    array = (flags & CONVERT_LOWERCASE) ? "0123456789abcdef" : "0123456789ABCDEF";
    ptr = &buffer[49];
    *ptr = '\0';

    do {
        *--ptr = array[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--ptr = sign;

    return ptr;
}

void removeComments(char *buf) {
    int i;
    for (i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == '#' && (!i || buf[i - 1] == ' ')) {
            buf[i] = '\0';
            break;
        }
    }
}
