#include "my_shell.h"

int unsetAlias(info_t *info, char *aliasName) {
    char *equalsSign, originalCharacter;
    int result;

    equalsSign = findCharacter(aliasName, '=');

    if (!equalsSign)
        return 1;

    originalCharacter = *equalsSign;
    *equalsSign = 0;

    result = deleteAliasNodeAtIndex(&(info->aliases),
                                    getNodeIndex(info->aliases, nodeStartsWith(info->aliases, aliasName, -1)));

    *equalsSign = originalCharacter;

    return result;
}

int setAlias(info_t *info, char *aliasString) {
    char *equalsSign;

    equalsSign = findCharacter(aliasString, '=');

    if (!equalsSign)
        return 1;

    if (!*++equalsSign)
        return unsetAlias(info, aliasString);

    unsetAlias(info, aliasString);

    return addAliasNodeEnd(&(info->aliases), aliasString, 0) == NULL;
}

int printAlias(list_t *aliasNode) {
    char *equalsSignPosition = NULL, *aliasName = NULL;

    if (aliasNode) {
        equalsSignPosition = findCharacter(aliasNode->str, '=');
        for (aliasName = aliasNode->str; aliasName <= equalsSignPosition; aliasName++)
            printCharacter(*aliasName);
        printCharacter('\'');
        printString(equalsSignPosition + 1);
        printCharacter('\'');
        printNewLine();
        return 0;
    }

    return 1;
}

int myAlias(info_t *info) {
    int i = 0;
    char *equalsSignPosition = NULL;
    list_t *aliasNode = NULL;

    if (info->argumentCount == 1) {
        aliasNode = info->aliases;
        while (aliasNode) {
            printAlias(aliasNode);
            aliasNode = aliasNode->next;
        }
        return 0;
    }

    for (i = 1; info->arguments[i]; i++) {
        equalsSignPosition = findCharacter(info->arguments[i], '=');
        if (equalsSignPosition)
            setAlias(info, info->arguments[i]);
        else
            printAlias(nodeStartsWith(info->aliases, info->arguments[i], '='));
    }

    return 0;
}
