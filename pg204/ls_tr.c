#include"utils.h"

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char** argv){
  char  *ls[] = { "ls", "-l", NULL};
  char  *tr[] = { "tr", "-d", "[:blank:]", NULL};
  char **commandes[] = { ls, tr };
  //int nb_commandes = sizeof(commandes) / sizeof(char*);
  //on rappelle pour un processus par rapport à dup2
  //il y a 0 comme stdin, 1 comme stdout et 2 comme stderr(schema en tete).

  
  int fd[2];
  pipe(fd);
  pid_t pid=fork();


  
  if(pid==0){
    close(fd[1]);//toujours la premiere commande.
    dup2(fd[0],0);//toujours dans ce cas, brancher fd[0] sur stdin.
    close(fd[0]);
    //fd[0] ce qui va lire sera le stdin soit l'entree
    //de ce processus est la lecture de fd[0]
    //soit lexecution de la commande partielle en son ensemble: ls -l.
    execvp(commandes[1][0],commandes[1]);
    return EXIT_FAILURE;
  }
  else{
    /*code du père*/
    close(fd[0]);
    dup2(fd[1],1);
    close(fd[1]);
    execvp(commandes[0][0],commandes[0]);
    printf("terminaison execvp\n");
  }
  return 0;
}
