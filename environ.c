#include "my_shell.h"

char *getEnvironmentVariable(info_t *info, const char *name) {
    list_t *node = info->environment;
    char *p;

    while (node) {
        p = startsWith(node->str, name);
        if (p && *p)
            return p;
        node = node->next;
    }

    return NULL;
}

int mySetEnvironmentVariable(info_t *info) {
    if (info->argumentCount != 3) {
        printError("Incorrect number of arguments\n");
        return 1;
    }

    if (setEnvironmentVariable(info, info->arguments[1], info->arguments[2]))
        return 0;

    return 1;
}

int myUnsetEnvironmentVariable(info_t *info) {
    int i;

    if (info->argumentCount == 1) {
        printError("Too few arguments.\n");
        return 1;
    }

    for (i = 1; i <= info->argumentCount; i++)
        unsetEnvironmentVariable(info, info->arguments[i]);

    return 0;
}

int populateEnvironmentList(info_t *info) {
    list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++)
        addNodeEnd(&node, environ[i], 0);

    info->environment = node;
    return 0;
}
