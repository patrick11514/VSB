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
#include <cmath>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/time.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#define STR_CLOSE "close"
#define STR_QUIT "quit"
#define MUTEX_SEM "/mutex"

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

#define LOAD_BY_CHARS 1 // load by chars or 0 to load data and then split by \n

sem_t *mutex;

void client(int fd) {
  char search[255];

  int len = read(fd, search, sizeof(search));
  if (len <= 0) {
    log_msg(LOG_INFO, "Unable to read search string!");
    return;
  }

  search[len - 1] = 0; // replace \n with \0

  sem_wait(mutex);

  static const char *OK = "OK\n";
  int ret = write(fd, OK, strlen(OK));
  if (ret < 0) {
    log_msg(LOG_INFO, "Unable to send OK message!");
    return;
  }

#if LOAD_BY_CHARS == 1
  int charN = 0;
#else
  int startAt = 0;
#endif
  int n = 0;
  char buffer[20];

  while (true) {
#if LOAD_BY_CHARS == 1
    char c;
    int len = read(fd, &c, 1);
    if (len <= 0) {
      // Error
      log_msg(LOG_INFO, "Error reading");
      return;
    }

    if (c != '\n') {
      buffer[charN++] = c;
      continue;
    }

    buffer[charN] = 0; // end of string
    charN = 0;
    if (strlen(buffer) == 0) {
      break;
    }

    char numbered[512];
    ++n;
    sprintf(numbered, "%d. %s\n", n, buffer);
    int ret = write(fd, numbered, strlen(numbered));
    if (ret < 0) {
      log_msg(LOG_INFO, "Unable to write numbered word");
    }

#else

    int len = read(fd, buffer + startAt, sizeof(buffer) - startAt);
    if (len <= 0) {
      log_msg(LOG_INFO, "Finished");
      break; // end
    }

    buffer[startAt + len] = 0;

    char *TOK = buffer;
    char *WORD;
    bool end = false;

    while ((WORD = strtok_r(TOK, "\n", &TOK))) {
      if (*TOK == '\0' && buffer[startAt + len - 1] != '\n')
        break; // if its last element we should add it to start of buffer and
               // then let rest of data append

      if (strlen(WORD) == 0) {
        end = true;
        break;
      }

      // process word
      if (strcasestr(WORD, search) == NULL) {
        // notfound
        continue;
      }

      char numbered[512];
      ++n;
      sprintf(numbered, "%d. %s\n", n, WORD);
      int ret = write(fd, numbered, strlen(numbered));
      if (ret < 0) {
        log_msg(LOG_INFO, "Unable to write numbered word");
      }
    }

    if (end) {
      break;
    }

    if (buffer[startAt + len - 1] != '\n') {
      memmove(buffer, WORD, strlen(WORD));
      startAt = strlen(WORD);
    } else {
      startAt = 0;
    }
#endif
  }

  sem_post(mutex);

  close(fd);
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

  bool reset = false; // reset semaphore (delete before creation)
  bool init = false;  // initialize semaphore with 1

  for (int i = 1; i < t_narg; ++i) {
    if (strcmp(t_args[i], "-r") == 0) {
      reset = true;
    } else if (strcmp(t_args[i], "-i") == 0) {
      init = true;
    }
  }

  if (reset) {
    sem_unlink(MUTEX_SEM);
  }

  mutex = sem_open(MUTEX_SEM, O_RDWR);
  if (!mutex) {
    mutex = sem_open(MUTEX_SEM, O_RDWR | O_CREAT, 0666, 1);
  }

  if (init) {
    sem_init(mutex, 1, 1);
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

        auto thread = std::thread(client, l_sock_client);
        thread.detach();
      }

    } // while wait for client

  } // while ( 1 )

  return 0;
}
