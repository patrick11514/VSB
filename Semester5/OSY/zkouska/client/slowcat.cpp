#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }

  struct stat filestat;

  if (stat(argv[1], &filestat) == -1) {
    printf("Unable to get file status");
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[1], "r");

  if (!file) {
    printf("Error opening file");
    return EXIT_FAILURE;
  }

  int size = filestat.st_size;

  char line[512];
  while (fgets(line, 512, file)) {
    printf("%s", line);
    usleep(size > 2000 ? 100 : 100000 - (size * 10));
  }

  fclose(file);
}
