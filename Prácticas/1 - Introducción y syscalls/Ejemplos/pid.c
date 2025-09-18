#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    pid_t pid = getpid(); // pid_t es un renombre de int
    printf("Mi PID es %d\n", pid);
    return 0;
}
