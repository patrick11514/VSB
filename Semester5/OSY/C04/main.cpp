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

char randomOperation() {
  int op = rand() % 4;
  switch (op) {
  case 0:
    return '+';
  case 1:
    return '-';
  case 2:
    return '*';
  case 3:
    return '/';
  }
  return ' ';
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage %s N speed\n", argv[0]);
    return -1;
  }

  struct Pipe pipe = createPipe();
  struct Pipe pipe1 = createPipe();
  struct Pipe pipe2 = createPipe();

  const int MAX = 1000;

  if (fork() == 0) {
    // child1
    //
    closePipe(pipe, true, false);
    closePipe(pipe1, false, true);
    closePipe(pipe2, true, true);

    char buffer[100];
    int ret = read(pipe.output, buffer, sizeof(buffer));
    buffer[ret] = '\0';

    closePipe(pipe, false, true);

    int N;
    int speed;

    sscanf(buffer, "%d %d", &N, &speed);

    for (int i = 0; i < N; ++i) {
      char buffer[100] = {0};
      sprintf(buffer, "%d. %d\n", i + 1, rand() % MAX);
      write(pipe1.input, buffer, strlen(buffer));

      usleep(1000 * 1000 / speed);
    }
    closePipe(pipe1, true, false);
    return 0;
  }
  if (fork() == 0) {
    // child2
    //
    closePipe(pipe, true, true);
    closePipe(pipe1, true, false);
    closePipe(pipe2, false, true);

    while (true) {
      char buffer[100] = {0};
      int ret = read(pipe1.output, buffer, sizeof(buffer));
      if (ret <= 0)
        break;
      buff[ret] = '\0';
      buffer[strlen(buffer) - 1] = '\0';

      char outputBuffer[200] = {0};
      sprintf(outputBuffer, "%s%c%d\n", buffer, randomOperation(),
              rand() % MAX);
      write(pipe2.input, outputBuffer, strlen(outputBuffer));
    }

    closePipe(pipe1, false, true);
    closePipe(pipe2, true, false);

    return 0;
  }
  if (fork() == 0) {
    // child3
    //
    closePipe(pipe, true, true);
    closePipe(pipe1, true, true);
    closePipe(pipe2, true, false);

    while (true) {
      char buffer[200] = {0};
      int ret = read(pipe2.output, buffer, sizeof(buffer));
      if (ret <= 0)
        break;
      buffer[ret] = '\0';

      int pos, first, second;
      char op;
      sscanf(buffer, "%d. %d%c%d\n", &pos, &first, &op, &second);

      buffer[strlen(buffer) - 1] = '\0';

      int result;
      switch (op) {
      case '+':
        result = first + second;
        break;
      case '-':
        result = first - second;
        break;
      case '*':
        result = first * second;
        break;
      case '/':
        result = first / second;
        break;
      }
      printf("%s=%d\n", buffer, result);
    }

    closePipe(pipe2, false, true);

    return 0;
  }

  // parent
  closePipe(pipe, false, true);
  closePipe(pipe1, true, true);
  closePipe(pipe2, true, true);

  char buffer[100] = {0};
  sprintf(buffer, "%s %s\n", argv[1], argv[2]);

  write(pipe.input, buffer, strlen(buffer));

  closePipe(pipe, true, false);

  wait(NULL);
  wait(NULL);
  wait(NULL);
  return 0;
}
