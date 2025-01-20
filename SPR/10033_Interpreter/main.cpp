#include <cstdio>
#include <cstdlib>
#include <cstring>

static int count = 0;
static int ip = 0;
static int registers[10];
static int ram[1000];

struct Instruction {
  int n; // number of instruction
  int f; // first arg
  int s; // second arg
  Instruction(int n) : n(n / 100), f((n / 10) % 10), s(n % 10) {}
};

void execute() {

  while (true) {
    ++count;

    auto in = Instruction(ram[ip]);

    switch (in.n) {
    case 1: // HALT
      return;
    case 2:
      registers[in.f] = in.s;
      break;
    case 3:
      registers[in.f] = (registers[in.f] + in.s) % 1000;
      break;
    case 4:
      registers[in.f] = (registers[in.f] * in.s) % 1000;
      break;
    case 5:
      registers[in.f] = registers[in.s];
      break;
    case 6:
      registers[in.f] = (registers[in.f] + registers[in.s]) % 1000;
      break;
    case 7:
      registers[in.f] = (registers[in.f] * registers[in.s]) % 1000;
      break;
    case 8:
      registers[in.f] = ram[registers[in.s]];
      break;
    case 9:
      ram[registers[in.s]] = registers[in.f];
      break;
    case 0:
      int val = registers[in.s];
      if (val != 0) {
        ip = registers[in.f];
        continue;
      }
      break;
    }

    ++ip;
  }
}

void reset() {
  ip = count = 0;

  for (int i = 0; i < 1000; ++i) {
    if (i < 10)
      registers[i] = 0;
    ram[i] = 0;
  }
}

void run() {
  execute();
  printf("%d\n", count);
  reset();
}

int main() {
  int cases;
  scanf("%d\n", &cases);

  char buffer[10];

  int n = 0;

  reset();
  while (fgets(buffer, 10, stdin)) {
    buffer[strlen(buffer) - 1] = 0;

    if (strlen(buffer) == 0) {
      run();
      putchar('\n');
      n = 0;
      continue;
    }

    int num;
    sscanf(buffer, "%d\n", &num);
    ram[n] = num;

    ++n;
  }

  run(); // run last set
  return 0;
}
