#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() close()
#include <sys/wait.h> // wait()

// Constantes 0 / 1 para READ / WRITE
enum {READ, WRITE};

// Debe ejecutar "ls -al"
void ejecutar_hijo_1(int pipe_fd[]) {
  
  close(pipe_fd[READ]);

  dup2(pipe_fd[WRITE], STDOUT_FILENO);

  execlp("ls", "ls", "-la", NULL);

  close(pipe_fd[WRITE]);

  exit(EXIT_SUCCESS);
}

// Debe ejecutar "wc -l"
void ejecutar_hijo_2(int pipe_fd[]) {

  close(pipe_fd[WRITE]);

  dup2(pipe_fd[READ], STDIN_FILENO);

  execlp("wc", "wc", "-l", NULL);

  exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
  // Completar...
  int pidHijos[2];
  int pipeHijo1aHijo2[2];

  pipe(pipeHijo1aHijo2);
  
  for (int i = 0; i < 2; i++)
  {
    pidHijos[i] = fork();

    if ((i == 0) && pidHijos[i] == 0)
    {
      ejecutar_hijo_1(pipeHijo1aHijo2);
    }else if((i == 1) && (pidHijos[i] == 0)){
      ejecutar_hijo_2(pipeHijo1aHijo2);
    }
  }

  close(pipeHijo1aHijo2[READ]);
  close(pipeHijo1aHijo2[WRITE]);

  wait(NULL);
  wait(NULL);
  
  return 0;
}
