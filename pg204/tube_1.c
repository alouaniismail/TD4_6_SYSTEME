#include"utils.h"

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define READ_MAX 500
#ifndef NB_WRITE
#define NB_WRITE 10000
#endif

#define ERROR(msg) do { perror(msg);return EXIT_FAILURE;} while(0)

int main(int argc, char** argv){
  int pipe_fd[2],n,pid;
  char buffer[READ_MAX];
  const char* message="Hello World!";
  size_t msg_len=strlen(message),c;
  ssize_t r;

  if(-1==pipe(pipe_fd)){
    ERROR("création du tube");
  }
  pid=fork();
  if(-1==pid){
    ERROR("création du fils");
  }
  else if(pid==0){
    /*code du fils qui va lire depuis pipe_fd[0].*/
    close(pipe_fd[1]);
    r=read(pipe_fd[0],buffer,READ_MAX);
    if(-1==r){
      close(pipe_fd[0]);
      ERROR("fils depuis tube");
    } else if(0<r){
      //on affiche le message d'une façon ordinaire sur stdout a partir
      //du buffer et de r!(car write apres read(pas important(|.|))).
      if(-1==write(STDOUT_FILENO,buffer,r)){
	close(pipe_fd[0]);
	ERROR("fils sur stdout");
      }
    }
    close(pipe_fd[0]);
    return EXIT_SUCCESS;
  }
  close(pipe_fd[0]);
  for(n=0;n<NB_WRITE;++n){
    c=0;//on va ecrire
    //soit c=0
    //while(c<msg_len) a l'interieur de la boucle; faire:
    //r=write(pipe_fd[1],message+c,msg_len-c) puis c+=r;
    //voilà(dans le stdout du pipe(père)).
    while(c<msg_len){
      r=write(pipe_fd[1],message+c,msg_len-c);
      if(-1==r){
	close(pipe_fd[1]);
	ERROR("pere sur pipe");
      }
      c+=r;
    }
  }
  close(pipe_fd[1]);
  return EXIT_SUCCESS;
}
    
//attention à l'écriture
//lecture puis stderr substitution à une simple non bufferisation d'affichage
//sur STDOUT_FILENO(0;write(2)).
//voilà.

//pour définir -DNB_WRITE c'est dans CFLAGS+="??".
    
