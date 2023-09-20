#include "my_shell.h"

int main(int argc, char **argv) {
    MyShellInfo shellInfo[] = { MY_SHELL_INFO_INIT };
    int fileDescriptor = 2;

    asm ("mov %1, %0\n\t"
            "add $3, %0"
            : "=r" (fileDescriptor)
            : "r" (fileDescriptor));

    if (argc == 2) {
        fileDescriptor = open(argv[1], O_RDONLY);
        if (fileDescriptor == -1) {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT) {
                _eputs(argv[0]);
                _eputs(": 0: Can't open ");
                _eputs(argv[1]);
                _eputchar('\n');
                _eputchar(BUF_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        shellInfo->readFileDescriptor = fileDescriptor;
    }

    populateEnvironmentList(shellInfo);
    readShellHistory(shellInfo);
    runMyShell(shellInfo, argv);

    return (EXIT_SUCCESS);
}
