#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string.h>
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
  struct Pipe pipe1 = createPipe();
  struct Pipe pipe2 = createPipe();

  const int MAX = 1000;

  if (fork() == 0) {
    // child1
    //
    closePipe(pipe1, false, true);
    closePipe(pipe2, true, true);
    int max;
    if (argc > 1) {
      max = atoi(argv[1]);
    }
    while (true) {
      char buffer[10] = {0};
      sprintf(buffer, "%d %d\n", rand() % MAX, rand() % MAX);
      write(pipe1.input, &buffer, strlen(buffer));

      usleep(100 * 1000);

      if (argc > 1) {
        --max;
        if (max <= 0) {
          break;
        }
      }
    }
    closePipe(pipe1, true, false);
    return 0;
  }
  if (fork() == 0) {
    // child2
    //
    closePipe(pipe1, true, false);
    closePipe(pipe2, false, true);

    while (true) {
      char buffer[10];
      int ret = read(pipe1.output, buffer, sizeof(buffer));
      if (ret <= 0)
        break;

      int number1, number2;
      sscanf(buffer, "%d %d\n", &number1, &number2);

      char outputBuffer[10] = {0};
      sprintf(outputBuffer, "%d\n", number1 + number2);
      write(pipe2.input, outputBuffer, strlen(outputBuffer));
    }

    closePipe(pipe1, false, true);
    closePipe(pipe2, true, false);

    return 0;
  }
  if (fork() == 0) {
    // child3
    //
    closePipe(pipe1, true, true);
    closePipe(pipe2, true, false);

    while (true) {
      char buffer[10];
      int ret = read(pipe2.output, buffer, sizeof(buffer));
      if (ret <= 0)
        break;
      int number;
      sscanf(buffer, "%d\n", &number);
      printf("%d\n", number);
    }

    closePipe(pipe2, false, true);

    return 0;
  }

  // parent

  closePipe(pipe1, true, true);
  closePipe(pipe2, true, true);
  wait(NULL);
  wait(NULL);
  wait(NULL);
  return 0;
}
