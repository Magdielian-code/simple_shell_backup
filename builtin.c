#include "my_shell.h"

int myExitCommand(info_t *info) {
    int exitCode;

    if (info->arguments[1]) {
        exitCode = convertToInt(info->arguments[1]);

        if (exitCode == -1) {
            info->status = 2;
            printErrorMessage(info, "Illegal number: ");
            printErrorString(info->arguments[1]);
            printNewLine();
            return 1;
        }

        info->errorNumber = convertToInt(info->arguments[1]);
        return -2;
    }

    info->errorNumber = -1;
    return -2;
}

int myChangeDirectory(info_t *info) {
    char *currentDirectory, *newDirectory, buffer[1024];
    int changeDirResult;

    currentDirectory = getCurrentDirectory(buffer, 1024);

    if (!currentDirectory)
        printString("TODO: Handle getcwd failure here\n");

    if (!info->arguments[1]) {
        newDirectory = getenvVariable(info, "HOME=");

        if (!newDirectory)
            changeDirResult = changeDirectory((newDirectory = getenvVariable(info, "PWD=")) ? newDirectory : "/");
        else
            changeDirResult = changeDirectory(newDirectory);
    } else if (compareStrings(info->arguments[1], "-") == 0) {
        if (!getenvVariable(info, "OLDPWD=")) {
            printString(currentDirectory);
            printNewLine();
            return 1;
        }

        printString(getenvVariable(info, "OLDPWD="));
        printNewLine();
        changeDirResult = changeDirectory((newDirectory = getenvVariable(info, "OLDPWD=")) ? newDirectory : "/");
    } else {
        changeDirResult = changeDirectory(info->arguments[1]);
    }

    if (changeDirResult == -1) {
        printErrorMessage(info, "Can't change directory to ");
        printErrorString(info->arguments[1]);
        printNewLine();
    } else {
        setEnvironmentVariable(info, "OLDPWD", getenvVariable(info, "PWD="));
        setEnvironmentVariable(info, "PWD", getCurrentDirectory(buffer, 1024));
    }

    return 0;
}

int myHelpCommand(info_t *info) {
    char **argArray = info->arguments;

    printString("The 'help' command works, but the function is not implemented yet.\n");

    if (0)
        printString(*argArray); // Temporary workaround
    return 0;
}
