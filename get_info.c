#include "my_shell.h"

void initializeInfo(info_t *info) {
    info->arguments = NULL;
    info->argumentVector = NULL;
    info->path = NULL;
    info->argumentCount = 0;
}

void setInfo(info_t *info, char **argumentVector) {
    int i = 0;
    
    info->programName = argumentVector[0];
    
    if (info->arguments) {
        info->argumentVector = splitString(info->arguments, " \t");
        
        if (!info->argumentVector) {
            info->argumentVector = malloc(sizeof(char *) * 2);
            
            if (info->argumentVector) {
                info->argumentVector[0] = duplicateString(info->arguments);
                info->argumentVector[1] = NULL;
            }
        }
        
        for (i = 0; info->argumentVector && info->argumentVector[i]; i++)
            ;
        
        info->argumentCount = i;
        
        replaceAliases(info);
        replaceVariables(info);
    }
}

void freeInfo(info_t *info, int freeAll) {
    freeStringArray(info->argumentVector);
    info->argumentVector = NULL;
    info->path = NULL;
    
    if (freeAll) {
        if (!info->commandBuffer)
            free(info->arguments);
        
        if (info->environmentList)
            freeList(&(info->environmentList));
        
        if (info->historyList)
            freeList(&(info->historyList));
        
        if (info->aliasList)
            freeList(&(info->aliasList));
        
        freeStringArray(info->environmentVariables);
        info->environmentVariables = NULL;
        
        free(info->commandBuffer);
        
        if (info->readFileDescriptor > 2)
            close(info->readFileDescriptor);
        
        write(STDOUT_FILENO, BUF_FLUSH, 1);
    }
}
