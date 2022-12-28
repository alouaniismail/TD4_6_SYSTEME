#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<memory.h>
#include "utils.h"

#define SIZE 500

int main(int argc, char** argv){
  int fd[2];
  pipe(fd);
  pid_t pid=fork();
  if(pid!=0){
    close(fd[0]);
    int i;
    for(i=0;i<atoi(argv[1]);i++){
      write(fd[1],"Hello World",strlen("Hello World"));
    }
    close(fd[1]);
    fprintf(stderr,"Je suis le pere et je quitte");
    return 1;
  }
  else{
    close(fd[1]);
    char buffer[SIZE+1];
    read(fd[0],buffer,SIZE+1);
    printf("%s\n",buffer);
    fprintf(stderr,"Je suis le fils et je quitte");
    close(fd[0]);
    return 2;
  }
  //l'execution sera un certain affichage puis le message du fils
  //car c'est en parallèle ; le plut tôt est le fils.
  //si on lance en tâche de fond, soit ./mypipe 1001(correct/999) &
  //puis entrée ; cela affiche l'état de complétion du processus
  //soit relais brisé ; c'est logique.
  //on a corrigé le problème grâce à ce qui précède.
  return 0;
}
