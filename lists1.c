#include "my_shell.h"

size_t myListLength(const list_t *h) {
    size_t size = 0;

    while (h) {
        h = h->next;
        size++;
    }

    return size;
}

char **listToStrings(list_t *head) {
    list_t *node = head;
    size_t i = myListLength(head), j;
    char **strArray;
    char *str;

    if (!head || !i)
        return NULL;

    strArray = malloc(sizeof(char *) * (i + 1));
    if (!strArray)
        return NULL;

    for (i = 0; node; node = node->next, i++) {
        str = malloc(_strlen(node->data) + 1);
        if (!str) {
            for (j = 0; j < i; j++)
                free(strArray[j]);
            free(strArray);
            return NULL;
        }

        str = _strcpy(str, node->data);
        strArray[i] = str;
    }

    strArray[i] = NULL;

    return strArray;
}

size_t printMyList(const list_t *h) {
    size_t size = 0;

    while (h) {
        _puts(convertNumber(h->index, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(h->data ? h->data : "(nil)");
        _puts("\n");
        h = h->next;
        size++;
    }

    return size;
}

list_t *findNodeStartingWith(list_t *node, char *prefix, char nextChar) {
    char *p = NULL;

    while (node) {
        p = startsWith(node->data, prefix);
        if (p && ((nextChar == -1) || (*p == nextChar)))
            return node;
        node = node->next;
    }

    return NULL;
}

ssize_t getNodeIndex(list_t *head, list_t *node) {
    size_t index = 0;

    while (head) {
        if (head == node)
            return index;
        head = head->next;
        index++;
    }

    return -1;
}
