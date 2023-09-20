#include "my_shell.h"

list_t *createNode(list_t **head, const char *string, int number) {
    list_t *newHead;

    if (!head)
        return NULL;

    newHead = malloc(sizeof(list_t));
    if (!newHead)
        return NULL;

    memset((void *)newHead, 0, sizeof(list_t));
    newHead->index = number;

    if (string) {
        newHead->data = duplicateString(string);
        if (!newHead->data) {
            free(newHead);
            return NULL;
        }
    }

    newHead->next = *head;
    *head = newHead;

    return newHead;
}

list_t *addNodeEnd(list_t **head, const char *string, int number) {
    list_t *newNode, *currentNode;

    if (!head)
        return NULL;

    currentNode = *head;
    newNode = malloc(sizeof(list_t));
    if (!newNode)
        return NULL;

    memset((void *)newNode, 0, sizeof(list_t));
    newNode->index = number;

    if (string) {
        newNode->data = duplicateString(string);
        if (!newNode->data) {
            free(newNode);
            return NULL;
        }
    }

    if (currentNode) {
        while (currentNode->next)
            currentNode = currentNode->next;
        currentNode->next = newNode;
    } else {
        *head = newNode;
    }

    return newNode;
}

size_t printListData(const list_t *head) {
    size_t size = 0;

    while (head) {
        _puts(head->data ? head->data : "(nil)");
        _puts("\n");
        head = head->next;
        size++;
    }

    return size;
}

int deleteNodeAtIndex(list_t **head, unsigned int index) {
    list_t *node, *prevNode;
    unsigned int i = 0;

    if (!head || !*head)
        return 0;

    if (!index) {
        node = *head;
        *head = (*head)->next;
        free(node->data);
        free(node);
        return 1;
    }

    node = *head;
    while (node) {
        if (i == index) {
            prevNode->next = node->next;
            free(node->data);
            free(node);
            return 1;
        }
        i++;
        prevNode = node;
        node = node->next;
    }

    return 0;
}

void freeList(list_t **headPtr) {
    list_t *node, *nextNode, *head;

    if (!headPtr || !*headPtr)
        return;

    head = *headPtr;
    node = head;
    while (node) {
        nextNode = node->next;
        free(node->data);
        free(node);
        node = nextNode;
    }

    *headPtr = NULL;
}
