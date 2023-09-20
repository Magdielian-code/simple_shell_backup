#include "my_shell.h"

int myShellFree(void **ptr) {

    if (ptr && *ptr) {

        free(*ptr);

        *ptr = NULL;

        return 1;

    }

    return 0;

}
