
//***************************************************************************
//
// Program example for labs in subject Operating Systems
//
// Petr Olivka, Dept. of Computer Science, petr.olivka@vsb.cz, 2017
//
// Example of socket server.
//
// This program is example of socket server and it allows to connect and serve
// the only one client.
// The mandatory argument of program is port number for listening.
//
//***************************************************************************

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <fcntl.h> /* For O_* constants */
#include <netinet/in.h>
#include <poll.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define PRODUCER 1
#define CONSUMER 2
#define N 5
#define LEN 255

struct SHM {
  char names[N][LEN];
  int start_item = 0;
  int end_item = 0;
  int items = 0;
};

SHM *shm;
sem_t *mutex;
sem_t *empty;
sem_t *full;

void insert_item(char *item) {
  strcpy(shm->names[shm->start_item], item);
  shm->start_item = (shm->start_item + 1) % N;
  shm->items++;
}

void remove_item(char *item) {
  strcpy(item, shm->names[shm->end_item]);
  shm->end_item = (shm->end_item + 1) % N;
  shm->items--;
}

#define STR_CLOSE "close"
#define STR_QUIT "quit"

//***************************************************************************
// log messages

#define LOG_ERROR 0 // errors
#define LOG_INFO 1  // information and notifications
#define LOG_DEBUG 2 // debug messages

// debug flag
int g_debug = LOG_INFO;

void log_msg(int t_log_level, const char *t_form, ...) {
  const char *out_fmt[] = {"ERR: (%d-%s) %s\n", "INF: %s\n", "DEB: %s\n"};

  if (t_log_level && t_log_level > g_debug)
    return;

  char l_buf[1024];
  va_list l_arg;
  va_start(l_arg, t_form);
  vsprintf(l_buf, t_form, l_arg);
  va_end(l_arg);

  switch (t_log_level) {
  case LOG_INFO:
  case LOG_DEBUG:
    fprintf(stdout, out_fmt[t_log_level], l_buf);
    break;

  case LOG_ERROR:
    fprintf(stderr, out_fmt[t_log_level], errno, strerror(errno), l_buf);
    break;
  }
}

//***************************************************************************
// help

void help(int t_narg, char **t_args) {
  if (t_narg <= 1 || !strcmp(t_args[1], "-h")) {
    printf("\n"
           "  Socket server example.\n"
           "\n"
           "  Use: %s [-h -d] port_number\n"
           "\n"
           "    -d  debug mode \n"
           "    -h  this help\n"
           "\n",
           t_args[0]);

    exit(0);
  }

  if (!strcmp(t_args[1], "-d"))
    g_debug = LOG_DEBUG;
}

//***************************************************************************

void producer(char *item) {
  sem_wait(empty);
  sem_wait(mutex);
  insert_item(item);
  sem_post(mutex);
  sem_post(full);
}

void consumer(char *item) {
  sem_wait(full);
  sem_wait(mutex);
  remove_item(item);
  sem_post(mutex);
  sem_post(empty);
}

void consumeAll(int fd) {
  bool end = false;
  while (true) {
    sem_wait(full);
    char item[LEN];
    sem_wait(mutex);
    remove_item(item);
    int rest = shm->items;
    sem_post(mutex);

    if (rest == 0)
      end = true;

    char wBuffer[255];
    sprintf(wBuffer, "%s\n", item);

    int ret = write(fd, wBuffer, strlen(wBuffer));
    if (ret <= 0) {
      close(fd);
      log_msg(LOG_INFO, "Unable to write to client");
      return;
    }

    sem_post(empty);

    if (end)
      break;
  }
}

void producerClient(int fd) {
  while (true) {
    char buffer[255];

    int ret = read(fd, buffer, sizeof(buffer) - 1);
    if (ret <= 0) {
      close(fd);
      return;
    }

    buffer[ret] = '\0'; // without n-line

    char *restCmd = buffer;
    char *splCmd;
    while ((splCmd = strtok_r(restCmd, "\n", &restCmd))) {

      if (strncmp(splCmd, "#write", 5) != 0) {
        log_msg(LOG_INFO, "Unable to read from client");
        continue;
      }

      char *rest = splCmd + 6;
      char *spl;
      while ((spl = strtok_r(rest, " ", &rest))) {
        printf("Producing %s\n", spl);
        producer(spl);
      }
    }
  }
}

void consumerClient(int fd) {
  while (true) {
    char buffer[255];

    int ret = read(fd, buffer, sizeof(buffer) - 1);
    if (ret <= 0) {
      close(fd);
      return;
    }

    buffer[ret] = '\0';

    char *restCmd = buffer;
    char *splCmd;
    while ((splCmd = strtok_r(restCmd, "\n", &restCmd))) {

      if (strcmp(splCmd, "#read") == 0) {
        char item[LEN];
        consumer(item);

        char wBuffer[255];
        sprintf(wBuffer, "%s\n", item);

        ret = write(fd, wBuffer, strlen(wBuffer));
        if (ret <= 0) {
          close(fd);
          log_msg(LOG_INFO, "Unable to write to client");
          return;
        }
      } else if (strcmp(splCmd, "#readall") == 0) {
        printf("ok\n");
        consumeAll(fd);
      } else {
        log_msg(LOG_INFO, "Unable to read from client");
        continue;
      }
    }
  }
}

//***************************************************************************

int main(int t_narg, char **t_args) {
  if (t_narg <= 1)
    help(t_narg, t_args);

  int l_port = 0;

  // parsing arguments
  for (int i = 1; i < t_narg; i++) {
    if (!strcmp(t_args[i], "-d"))
      g_debug = LOG_DEBUG;

    if (!strcmp(t_args[i], "-h"))
      help(t_narg, t_args);

    if (*t_args[i] != '-' && !l_port) {
      l_port = atoi(t_args[i]);
      break;
    }
  }

  if (l_port <= 0) {
    log_msg(LOG_INFO, "Bad or missing port number %d!", l_port);
    help(t_narg, t_args);
  }

  log_msg(LOG_INFO, "Server will listen on port: %d.", l_port);

  // socket creation
  int l_sock_listen = socket(AF_INET, SOCK_STREAM, 0);
  if (l_sock_listen == -1) {
    log_msg(LOG_ERROR, "Unable to create socket.");
    exit(1);
  }

  in_addr l_addr_any = {INADDR_ANY};
  sockaddr_in l_srv_addr;
  l_srv_addr.sin_family = AF_INET;
  l_srv_addr.sin_port = htons(l_port);
  l_srv_addr.sin_addr = l_addr_any;

  // Enable the port number reusing
  int l_opt = 1;
  if (setsockopt(l_sock_listen, SOL_SOCKET, SO_REUSEADDR, &l_opt,
                 sizeof(l_opt)) < 0)
    log_msg(LOG_ERROR, "Unable to set socket option!");

  // assign port number to socket
  if (bind(l_sock_listen, (const sockaddr *)&l_srv_addr, sizeof(l_srv_addr)) <
      0) {
    log_msg(LOG_ERROR, "Bind failed!");
    close(l_sock_listen);
    exit(1);
  }

  // listenig on set port
  if (listen(l_sock_listen, 1) < 0) {
    log_msg(LOG_ERROR, "Unable to listen on given port!");
    close(l_sock_listen);
    exit(1);
  }

  log_msg(LOG_INFO, "Enter 'quit' to quit server.");

  bool del = false;
  bool init = false;
  int role = 0;

  for (int i = 0; i < t_narg; ++i) {
    char *arg = t_args[i];
    if (strcmp(arg, "-i") == 0) {
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

  int fd = shm_open("/names", O_CREAT | O_RDWR, 0600);
  shm =
      (SHM *)mmap(NULL, sizeof(SHM), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  mutex = sem_open("/mutex", O_RDWR);
  empty = sem_open("/empty", O_RDWR);
  full = sem_open("/full", O_RDWR);

  // go!
  while (1) {
    int l_sock_client = -1;

    // list of fd sources
    pollfd l_read_poll[2];

    l_read_poll[0].fd = STDIN_FILENO;
    l_read_poll[0].events = POLLIN;
    l_read_poll[1].fd = l_sock_listen;
    l_read_poll[1].events = POLLIN;

    while (1) // wait for new client
    {
      // select from fds
      int l_poll = poll(l_read_poll, 2, -1);

      if (l_poll < 0) {
        log_msg(LOG_ERROR, "Function poll failed!");
        exit(1);
      }

      if (l_read_poll[0].revents & POLLIN) { // data on stdin
        char buf[128];
        int len = read(STDIN_FILENO, buf, sizeof(buf));
        if (len < 0) {
          log_msg(LOG_DEBUG, "Unable to read from stdin!");
          exit(1);
        }

        log_msg(LOG_DEBUG, "Read %d bytes from stdin");
        // request to quit?
        if (!strncmp(buf, STR_QUIT, strlen(STR_QUIT))) {
          log_msg(LOG_INFO, "Request to 'quit' entered.");
          close(l_sock_listen);
          exit(0);
        }
      }

      if (l_read_poll[1].revents & POLLIN) { // new client?
        sockaddr_in l_rsa;
        int l_rsa_size = sizeof(l_rsa);
        // new connection
        l_sock_client =
            accept(l_sock_listen, (sockaddr *)&l_rsa, (socklen_t *)&l_rsa_size);
        if (l_sock_client == -1) {
          log_msg(LOG_ERROR, "Unable to accept new client.");
          close(l_sock_listen);
          exit(1);
        }
        uint l_lsa = sizeof(l_srv_addr);
        // my IP
        getsockname(l_sock_client, (sockaddr *)&l_srv_addr, &l_lsa);
        log_msg(LOG_INFO, "My IP: '%s'  port: %d",
                inet_ntoa(l_srv_addr.sin_addr), ntohs(l_srv_addr.sin_port));
        // client IP
        getpeername(l_sock_client, (sockaddr *)&l_srv_addr, &l_lsa);
        log_msg(LOG_INFO, "Client IP: '%s'  port: %d",
                inet_ntoa(l_srv_addr.sin_addr), ntohs(l_srv_addr.sin_port));

        if (fork() == 0) {
          close(l_sock_listen);

          if (l_port % 2 == 0) {
            producerClient(l_sock_client);
          } else {
            consumerClient(l_sock_client);
          }
          return 0;
        }

        close(l_sock_client);
      }

    } // while wait for client
  } // while ( 1 )

  return 0;
}
