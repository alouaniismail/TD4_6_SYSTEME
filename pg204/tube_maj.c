#include"utils.h"

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define READ_MAX 500


#include<ctype.h> //pour toupper(char).//

//le père est un processus
//va faire execlp avant dup2(fd[1],STDOUT_FILENO)
//pour que l'affichage soit dans fd[1] du pipe.

//le fils va lire en continu, passer en majuscules et
//faire un simple affichage sur la sortie standard.

//code simple sans gestion des erreurs.

int main(int argc, char** argv){
  int pipe_fd[2],pid;
  char buffer[READ_MAX];
  ssize_t r,i;
  
  pipe(pipe_fd);
  pid=fork();

  switch(pid){
  case 0:
    /*code du fils*/
    close(pipe_fd[1]);
    r=1;
    while(r!=0){
      r=read(pipe_fd[0],buffer,READ_MAX);
      if(0<r){
	for(i=0;i<r;i++)
	  buffer[i]=toupper(buffer[i]);
	write(STDOUT_FILENO,buffer,r);
      }
    }
    close(pipe_fd[0]);
    return EXIT_SUCCESS;

  default:
    close(pipe_fd[0]);
    dup2(pipe_fd[1],STDOUT_FILENO);
    close(pipe_fd[1]);
    execlp("ps","ps",NULL);
    printf("error execlp\n");
    return EXIT_FAILURE;
  }
  return 0;
}
