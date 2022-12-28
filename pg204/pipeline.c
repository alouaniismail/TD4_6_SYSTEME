#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERROR(msg) do { perror(msg); return EXIT_FAILURE; } while(0)

int main(int argc, char **argv)
{
    int fds[2];
    int in_next = STDIN_FILENO;
    pid_t pid_enfant, pid;
    int status;

    char *ls[] = {"ls", "-l", NULL};
    char *tr[] = {"tr", "-d", "[:blank:]", NULL};
    char* head[]={"head",NULL};
    char **commandes[] = {ls, tr,head};
    size_t nb_commandes = sizeof(commandes) / sizeof(char *), i;

    for (i = 0; i < nb_commandes; ++i) {
        fds[0] = fds[1] = -1;
        if (i < nb_commandes - 1) {
            /* On n'est pas le dernier processus du pipeline */
            if (-1 == pipe(fds)) ERROR("pipe");
        }

        pid_enfant = fork();
        if (-1 == pid_enfant) {
            ERROR("fork");
        } else if (0 == pid_enfant) {
            /* FILS */
            if (STDIN_FILENO != in_next) {
                /* On n'est pas dans le premier processus du pipeline */
                dup2(in_next, STDIN_FILENO);
                close(in_next);
            }
            if (-1 != fds[0]) {
                /* On n'est pas le dernier processus du pipeline */
                dup2(fds[1], STDOUT_FILENO);
                close(fds[0]);
                close(fds[1]);
            }
            execvp(commandes[i][0], commandes[i]);
            ERROR("execvp");
        }
        /* PERE */
        if (STDIN_FILENO != in_next)
            close(in_next);
        if (-1 != fds[0]) {
            close(fds[1]);
            in_next = fds[0];
        }
    }

    /* On attend le bon enfant */
    do {
        pid = wait(&status);
        if (-1 == pid) {
            ERROR("wait");
        }
    } while (pid != pid_enfant);

    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else {
        if (WIFSIGNALED(status))
            fprintf(stderr, "Signal %d\n", WTERMSIG(status));

        return EXIT_FAILURE;
    }
}
