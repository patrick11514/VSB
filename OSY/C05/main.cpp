#include <cstdio>
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

int main(int argc, char **argv) {

  if (argc < 3) {
    printf("Usage %s <symbol> <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct Pipe pipe1 = createPipe();
  struct Pipe pipe2 = createPipe();
  struct Pipe pipe3 = createPipe();

  if (fork() == 0) {
    closePipe(pipe1, false, true);
    closePipe(pipe2, true, true);
    closePipe(pipe3, true, true);

    dup2(pipe1.input, STDOUT_FILENO);

    closePipe(pipe1, true, false);

    execlp("sort", "sort", nullptr);
  }

  if (fork() == 0) {
    closePipe(pipe1, true, false);
    closePipe(pipe2, false, true);
    closePipe(pipe3, true, true);

    dup2(pipe1.output, STDIN_FILENO);
    closePipe(pipe1, false, true);

    dup2(pipe2.input, STDOUT_FILENO);
    closePipe(pipe2, true, false);

    execlp("tr", "tr", "[a-z]", "[A-Z]", nullptr);
  }

  if (fork() == 0) {
    closePipe(pipe1, true, true);
    closePipe(pipe2, true, false);
    closePipe(pipe3, false, true);

    dup2(pipe2.output, STDIN_FILENO);
    closePipe(pipe2, false, true);

    dup2(pipe3.input, STDOUT_FILENO);
    closePipe(pipe3, true, false);

    char buffer[120];
    int len = sprintf(buffer, "^%c", argv[1][0]);
    buffer[len] = '\0';

    execlp("grep", "grep", "-n", buffer, nullptr);
  }

  if (fork() == 0) {
    closePipe(pipe1, true, true);
    closePipe(pipe2, true, true);
    closePipe(pipe3, true, false);

    int fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
    while (true) {
      char buffer[1024];
      int readed = read(pipe3.output, buffer, sizeof(buffer) - 1);
      if (readed <= 0)
        break;

      buffer[readed] = '\0';

      size_t size = strlen(buffer);
      write(STDOUT_FILENO, buffer, size);
      write(fd, buffer, size);
    }

    close(fd);

    // dup2(pipe3.output, STDIN_FILENO);
    closePipe(pipe3, false, true);

    // execlp("tee", "tee", argv[2], nullptr);
  }

  closePipe(pipe1, true, true);
  closePipe(pipe2, true, true);
  closePipe(pipe3, true, true);

  wait(NULL);
  wait(NULL);
  wait(NULL);
  wait(NULL);

  return 0;
}
