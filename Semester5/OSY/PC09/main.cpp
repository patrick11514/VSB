#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h> /* For O_* constants */
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <unistd.h>

#define PRODUCER 1
#define CONSUMER 2
#define N 1
#define LEN 255

struct SHM {
  char names[N][LEN];
  int start_item = 0;
  int end_item = 0;
};

SHM *shm;
sem_t *mutex;
sem_t *empty;
sem_t *full;
char current_item[LEN];

FILE *file_fd;

void insert_item() {
  strcpy(shm->names[shm->start_item], current_item);
  shm->start_item = (shm->start_item + 1) % N;
}

void remove_item() {
  strcpy(current_item, shm->names[shm->end_item]);
  shm->end_item = (shm->end_item + 1) % N;
}

void produce_item() {
  char *ptr = fgets(current_item, LEN, file_fd);
  if (ptr == 0) {
    fclose(file_fd);
    exit(EXIT_FAILURE);
  }

  int start = strlen(ptr) - 1; // remove newline
  for (int i = start; i < LEN; ++i) {
    ptr[i] = 0;
  }

  printf("%s\n", ptr);
}

void producer() {
  int fd = shm_open("/names", O_CREAT | O_RDWR, 0600);
  shm =
      (SHM *)mmap(NULL, sizeof(SHM), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  mutex = sem_open("/mutex", O_RDWR);
  empty = sem_open("/empty", O_RDWR);
  full = sem_open("/full", O_RDWR);

  file_fd = fopen("../names.txt", "r");

  while (true) {
    produce_item();
    sem_wait(empty);
    sem_wait(mutex);
    insert_item();
    sem_post(mutex);
    sem_post(full);
  }
}

void consumer() {
  int fd = shm_open("/names", O_CREAT | O_RDWR, 0600);
  shm =
      (SHM *)mmap(NULL, sizeof(SHM), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  mutex = sem_open("/mutex", O_RDWR);
  empty = sem_open("/empty", O_RDWR);
  full = sem_open("/full", O_RDWR);

  while (true) {
    sem_wait(full);
    sem_wait(mutex);
    remove_item();
    sem_post(mutex);
    sem_post(empty);
    printf("%s\n", current_item);
    fflush(stdout);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <p|c> [-i] [-d]\n", argv[0]);
    return EXIT_FAILURE;
  }

  bool del = false;
  bool init = false;
  int role = 0;

  for (int i = 0; i < argc; ++i) {
    char *arg = argv[i];
    if (strcmp(arg, "p") == 0) {
      role = PRODUCER;
    } else if (strcmp(arg, "c") == 0) {
      role = CONSUMER;
    } else if (strcmp(arg, "-i") == 0) {
      init = true;
    } else if (strcmp(arg, "-d") == 0) {
      del = true;
    }
  }

  if (del) {
    shm_unlink("/names");

    sem_unlink("/mutex");
    sem_unlink("/empty");
    sem_unlink("/full");
  }

  if (init) {
    int fd = shm_open("/names", O_CREAT | O_RDWR, 0600);
    ftruncate(fd, sizeof(SHM));

    mutex = sem_open("/mutex", O_CREAT | O_RDWR, 0600, 1);
    empty = sem_open("/empty", O_CREAT | O_RDWR, 0600, N);
    full = sem_open("/full", O_CREAT | O_RDWR, 06600, 0);
  }

  if (role == PRODUCER) {
    producer();
  } else if (role == CONSUMER) {
    consumer();
  }

  return EXIT_SUCCESS;
}
