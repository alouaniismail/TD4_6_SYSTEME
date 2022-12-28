#include"utils.h"

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define READ_MAX 500


#include<ctype.h>

//si on met pipe(fd) avec fd[2] de type int[2]
//on aura:(rappels seulement)
//fd[1] pour l'ecriture
//fd[0] pour la lecture.

int main(int argc, char** argv){
  pid_t pid;
  int fd[2],i;
  if(pipe(fd)==-1) return 1;
  if((pid=fork())<0) return 1;
  if(pid==0){
    /*code du fils*/
    /*qui va lire passer en majuscules et afficher en continu!*/
    close(fd[1]);
    char buffer[READ_MAX];
    ssize_t r=1;
    while(r!=0){
      r=read(fd[0],buffer,READ_MAX);
      exit_if(r<0,"probleme de lecture depuis le pipe");
      if(r>0){//r!=0=>si r==0;on a:aucune lecture!
	//ecriture sur la sortie standard apres passage en majuscules.
	//la variable buffer contient le rslt.
	for(i=0;i<r;++i){
	  buffer[i]=toupper(buffer[i]);
	}
	exit_if(write(STDOUT_FILENO,buffer,r)==-1,"probleme report sur la sortie 0");
      }
    }
    close(fd[0]);
    return EXIT_SUCCESS;
  }
  else{
    /*code du père*/
    //qui va ecrire en executant via une redirection avec dup2(execlp cmd).
    close(fd[0]);
    dup2(fd[1],STDOUT_FILENO);
    close(fd[1]);
    execlp("ps","ps",NULL);
    fprintf(stderr,"arret execlp retour .");
  }
  return EXIT_FAILURE;
}
  

      
