#include "my_shell.h"

ssize_t bufferInput(info_t *info, char **buffer, size_t *bufferLength) {
    ssize_t bytesRead = 0;
    size_t lengthUsed = 0;

    if (!*bufferLength) {
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, interruptHandler);

        bytesRead = getInputLine(buffer, &lengthUsed);

        if (bytesRead > 0) {
            if ((*buffer)[bytesRead - 1] == '\n') {
                (*buffer)[bytesRead - 1] = '\0';
                bytesRead--;
            }
            info->lineCountFlag = 1;
            removeComments(*buffer);
            buildHistoryList(info, *buffer, info->historyCount++);

            if (stringContains(*buffer, ';')) {
                *bufferLength = bytesRead;
                info->commandBuffer = buffer;
            }
        }
    }

    return bytesRead;
}

ssize_t getInputLine(info_t *info) {
    static char *buffer;
    static size_t currentPos = 0, lastPos = 0, bufferLength = 0;
    ssize_t bytesRead = 0;
    char **bufferPointer = &(info->arguments), *position;

    writeCharacter(BUFFER_FLUSH);

    bytesRead = bufferInput(info, &buffer, &bufferLength);

    if (bytesRead == -1)
        return -1;

    if (bufferLength) {
        lastPos = currentPos;
        position = buffer + currentPos;

        checkChain(info, buffer, &currentPos, lastPos, bufferLength);

        while (currentPos < bufferLength) {
            if (isChain(info, buffer, &currentPos))
                break;
            currentPos++;
        }

        lastPos = currentPos + 1;

        if (lastPos >= bufferLength) {
            lastPos = bufferLength = 0;
            info->commandBufferType = CMD_NORMAL;
        }

        *bufferPointer = position;
        return stringLength(position);
    }

    *bufferPointer = buffer;
    return bytesRead;
}

ssize_t readBuffer(info_t *info, char *buffer, size_t *bufferSize) {
    ssize_t bytesRead = 0;

    if (*bufferSize)
        return 0;

    bytesRead = read(info->readFileDescriptor, buffer, READ_BUFFER_SIZE);

    if (bytesRead >= 0)
        *bufferSize = bytesRead;

    return bytesRead;
}

int getInputLine(info_t *info, char **pointer, size_t *length) {
    static char buffer[READ_BUFFER_SIZE];
    static size_t currentPos = 0, bufferLength = 0;
    size_t k;
    ssize_t bytesRead = 0, totalBytesRead = 0;
    char *position = NULL, *newPosition = NULL, *c;

    position = *pointer;

    if (position && length)
        totalBytesRead = *length;

    if (currentPos == bufferLength)
        currentPos = bufferLength = 0;

    bytesRead = readBuffer(info, buffer, &bufferLength);

    if (bytesRead == -1 || (bytesRead == 0 && bufferLength == 0))
        return -1;

    c = findCharacter(buffer + currentPos, '\n');
    k = c ? 1 + (size_t)(c - buffer) : bufferLength;

    newPosition = reallocString(position, totalBytesRead, totalBytesRead ? totalBytesRead + k : k + 1);

    if (!newPosition)
        return (position ? free(position), -1 : -1);

    if (totalBytesRead)
        concatenateString(newPosition, buffer + currentPos, k - currentPos);
    else
        copyString(newPosition, buffer + currentPos, k - currentPos + 1);

    totalBytesRead += k - currentPos;
    currentPos = k;
    position = newPosition;

    if (length)
        *length = totalBytesRead;

    *pointer = position;
    return totalBytesRead;
}

void interruptHandler(_attribute_((unused)) int signalNumber) {
    writeString("\n");
    writeString("$ ");
    writeCharacter(BUFFER_FLUSH);
}
