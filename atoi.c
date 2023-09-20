#include "shell.h"

/**
 * Check if the shell is in interactive mode.
 *
 * @param info - A pointer to the info_t struct.
 * @return 1 if in interactive mode, 0 otherwise.
 */
int isInteractiveMode(info_t *info) {
    return (isatty(STDIN_FILENO) && info->readFileDescriptor <= 2);
}

/**
 * Check if a character is a delimiter.
 *
 * @param character - The character to check.
 * @param delimiterString - The delimiter string.
 * @return 1 if it's a delimiter, 0 otherwise.
 */
int isDelimiterCharacter(char character, char *delimiterString) {
    while (*delimiterString) {
        if (*delimiterString == character) {
            return 1;
        }
        delimiterString++;
    }
    return 0;
}

/**
 * Check if a character is alphabetic.
 *
 * @param character - The character to check.
 * @return 1 if it's alphabetic, 0 otherwise.
 */
int isAlphabeticCharacter(int character) {
    return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'));
}

/**
 * Convert a string to an integer.
 *
 * @param inputString - The string to be converted.
 * @return The converted number or 0 if no numbers in the string.
 */
int stringToInteger(char *inputString) {
    int index = 0;
    int sign = 1;
    int numberFlag = 0;
    unsigned int result = 0;

    for (index = 0; inputString[index] != '\0' && numberFlag != 2; index++) {
        if (inputString[index] == '-') {
            sign *= -1;
        }

        if (inputString[index] >= '0' && inputString[index] <= '9') {
            numberFlag = 1;
            result *= 10;
            result += (inputString[index] - '0');
        } else if (numberFlag == 1) {
            numberFlag = 2;
        }
    }

    return (sign == -1) ? -result : result;
}
