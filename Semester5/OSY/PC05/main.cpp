#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
struct Pipe {
  int input;
  int output;
};

Pipe createPipe() {
  int _pipe[2];
  pipe(_pipe);
  return Pipe{.input = _pipe[1], .output = _pipe[0]};
}

void closePipe(Pipe pipe, bool input, bool output) {
  if (input) {
    close(pipe.input);
  }

  if (output) {
    close(pipe.output);
  }
}

// pstree -p | less
int main(int argc, char *argv[]) {
  struct Pipe pipe1 = createPipe();
  struct Pipe pipe2 = createPipe();

  if (fork() == 0) {
    closePipe(pipe1, false, true);
    closePipe(pipe2, true, true);

    dup2(pipe1.input, STDOUT_FILENO);

    closePipe(pipe1, true, false);

    execlp("sort", "sort", nullptr);
  }

  if (fork() == 0) {
    closePipe(pipe1, true, false);
    closePipe(pipe2, false, true);

    dup2(pipe1.output, STDIN_FILENO);
    closePipe(pipe1, false, true);

    dup2(pipe2.input, STDOUT_FILENO);
    closePipe(pipe2, true, false);

    execlp("tr", "tr", "[a-z]", "[A-Z]", nullptr);
  }

  if (fork() == 0) {
    closePipe(pipe1, true, true);
    closePipe(pipe2, true, false);

    dup2(pipe2.output, STDIN_FILENO);
    closePipe(pipe2, false, true);

    execlp("nl", "nl", "-s", ". ", nullptr);
  }

  closePipe(pipe1, true, true);
  closePipe(pipe2, true, true);

  wait(NULL);
  wait(NULL);
  wait(NULL);

  return 0;
}
