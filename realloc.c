#include "my_shell.h"

char *_setMemory(char *memoryArea, char fillByte, unsigned int size) {

    unsigned int i;

    for (i = 0; i < size; i++)

        memoryArea[i] = fillByte;

    return memoryArea;

}

void freeStringArray(char **stringArray) {

    char **ptr = stringArray;

    if (!stringArray)

        return;

    while (*stringArray)

        free(*stringArray++);

    free(ptr);

}

void *reallocateMemory(void *previousBlock, unsigned int oldSize, unsigned int newSize) {

    char *newBlock;

    if (!previousBlock)

        return (malloc(newSize));

    if (!newSize)

        return (free(previousBlock), NULL);

    if (newSize == oldSize)

        return previousBlock;

    newBlock = malloc(newSize);

    if (!newBlock)

        return NULL;

    oldSize = oldSize < newSize ? oldSize : newSize;

    while (oldSize--)

        newBlock[oldSize] = ((char *)previousBlock)[oldSize];

    free(previousBlock);

    return newBlock;

}
