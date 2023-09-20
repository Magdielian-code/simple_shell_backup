#include "my_shell.h"

int main(int argc, char **argv) {

    ShellInfo shell_info[] = { SHELL_INFO_INIT };

    int file_descriptor = 2;

    asm ("mov %1, %0\n\t"
         "add $3, %0"
         : "=r" (file_descriptor)
         : "r" (file_descriptor));

    if (argc == 2) {
        file_descriptor = open(argv[1], O_RDONLY);
        if (file_descriptor == -1) {
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
            return EXIT_FAILURE;
        }
        shell_info->read_fd = file_descriptor;
    }

    populate_environment_list(shell_info);
    read_shell_history(shell_info);
    shell_loop(shell_info, argv);

    return EXIT_SUCCESS;
}

int shell_loop(ShellInfo *shell_info, char **argv) {

    ssize_t input_length = 0;
    int builtin_result = 0;

    while (input_length != -1 && builtin_result != -2) {
        clear_shell_info(shell_info);

        if (is_interactive(shell_info))
            _puts("$ ");

        _eputchar(BUF_FLUSH);

        input_length = get_shell_input(shell_info);

        if (input_length != -1) {
            set_shell_info(shell_info, argv);
            builtin_result = find_builtin_command(shell_info);

            if (builtin_result == -1)
                find_shell_command(shell_info);
        } else if (is_interactive(shell_info))
            _putchar('\n');

        free_shell_info(shell_info, 0);
    }

    write_shell_history(shell_info);
    free_shell_info(shell_info, 1);

    if (!is_interactive(shell_info) && shell_info->status)
        exit(shell_info->status);

    if (builtin_result == -2) {
        if (shell_info->error_number == -1)
            exit(shell_info->status);
        exit(shell_info->error_number);
    }

    return builtin_result;
}

int find_builtin_command(ShellInfo *shell_info) {

    int i, builtin_ret = -1;
    BuiltinCommand builtin_commands[] = {
        {"exit", _myexit},
        {"env", _myenv},
        {"help", _myhelp},
        {"history", _myhistory},
        {"setenv", _mysetenv},
        {"unsetenv", _myunsetenv},
        {"cd", _mycd},
        {"alias", _myalias},
        {NULL, NULL}
    };

    for (i = 0; builtin_commands[i].command; i++) {
        if (_strcmp(shell_info->argv[0], builtin_commands[i].command) == 0) {
            shell_info->line_count++;
            builtin_ret = builtin_commands[i].function(shell_info);
            break;
        }
    }

    return builtin_ret;
}

void find_shell_command(ShellInfo *shell_info) {

    char *path = NULL;
    int i, count;

    shell_info->path = shell_info->argv[0];

    if (shell_info->line_count_flag == 1) {
        shell_info->line_count++;
        shell_info->line_count_flag = 0;
    }

    count = 0;

    for (i = 0; shell_info->arg[i]; i++) {
        if (!is_delimiter(shell_info->arg[i], " \t\n"))
            count++;
    }

    if (!count)
        return;

    path = find_path(shell_info, _getenv(shell_info, "PATH="), shell_info->argv[0]);

    if (path) {
        shell_info->path = path;
        fork_shell_command(shell_info);
    } else {
        if ((is_interactive(shell_info) || _getenv(shell_info, "PATH=") || shell_info->argv[0][0] == '/') &&
            is_command(shell_info, shell_info->argv[0]))
            fork_shell_command(shell_info);
        else if (*(shell_info->arg) != '\n') {
            shell_info->status = 127;
            print_shell_error(shell_info, "not found\n");
        }
    }
}

void fork_shell_command(ShellInfo *shell_info) {

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Error:");
        return;
    }

    if (child_pid == 0) {
        if (execve(shell_info->path, shell_info->argv, get_shell_environment(shell_info)) == -1) {
            free_shell_info(shell_info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    } else {
        wait(&(shell_info->status));
        if (WIFEXITED(shell_info->status)) {
            shell_info->status = WEXITSTATUS(shell_info->status);
            if (shell_info->status == 126)
                print_shell_error(shell_info, "Permission denied\n");
        }
    }
}
