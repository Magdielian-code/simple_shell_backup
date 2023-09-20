#include "my_shell.h"

char **getEnvironmentVariables(info_t *info) {
    if (!info->environmentVariables || info->environmentChanged) {
        info->environmentVariables = convertListToStrings(info->environmentList);
        info->environmentChanged = 0;
    }
    return info->environmentVariables;
}

int unsetEnvironmentVariable(info_t *info, char *variable) {
    list_t *node = info->environmentList;
    size_t index = 0;
    char *p;

    if (!node || !variable)
        return 0;

    while (node) {
        p = startsWith(node->str, variable);
        if (p && *p == '=') {
            info->environmentChanged = deleteNodeAtIndex(&(info->environmentList), index);
            index = 0;
            node = info->environmentList;
            continue;
        }
        node = node->next;
        index++;
    }

    return info->environmentChanged;
}

int setEnvironmentVariable(info_t *info, char *variable, char *value) {
    char *buffer = NULL;
    list_t *node;
    char *p;

    if (!variable || !value)
        return 0;

    buffer = malloc(strlen(variable) + strlen(value) + 2);
    if (!buffer)
        return 1;

    strcpy(buffer, variable);
    strcat(buffer, "=");
    strcat(buffer, value);

    node = info->environmentList;
    while (node) {
        p = startsWith(node->str, variable);
        if (p && *p == '=') {
            free(node->str);
            node->str = buffer;
            info->environmentChanged = 1;
            return 0;
        }
        node = node->next;
    }

    addNodeEnd(&(info->environmentList), buffer, 0);
    free(buffer);
    info->environmentChanged = 1;
    return 0;
}
