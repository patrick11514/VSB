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
#include <sys/time.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

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

struct Item {
  int fd;
  sem_t mutex;
};

#define MAX_CLIENTS 5

static Item clients[MAX_CLIENTS];
static char word[255] = "a";

void findNewClient(int position) {
  // find new client
  int index = position + 1;
  while (true) {
    if (clients[index].fd == -1) {
      // probably looped
      if (position == index) {
        log_msg(LOG_ERROR,
                "No clients found, setting first place as start point.");
        sem_init(&clients[0].mutex, 0, 1);
        return;
      }
      index = (index + 1) % MAX_CLIENTS;
      continue;
    }

    break;
  }

  printf("CLIENT %d is on the way\n", index);

  sem_post(&clients[index].mutex);
};

void closeClient(int position) {
  Item &client = clients[position];
  close(client.fd);
  client.fd = -1;
  sem_init(&client.mutex, 0, 0);

  findNewClient(position);
}

void client(int position) {
  Item &client = clients[position];
  while (true) {
    sem_wait(&client.mutex);

    printf("Client %d is entering new word\n", position);
    char buffer[512];
    sprintf(buffer, "Current word is: %s\n", word);
    int ret = write(client.fd, buffer, strlen(buffer));
    if (ret <= 0) {
      log_msg(LOG_ERROR, "Unable to send message to client");
      closeClient(position);
      return;
    }

    while (true) {
      char buffer[255];
      int ret = read(client.fd, buffer, sizeof(buffer));
      if (ret <= 0) {
        log_msg(LOG_ERROR, "Unable to read message from client");
        closeClient(position);
        return;
      }

      buffer[ret - 1] = '\0'; // remove new line

      if (strcmp(buffer, "*") == 0) {
        log_msg(LOG_INFO, "Client disconnected.");
        closeClient(position);
        return;
      }

      if (buffer[strlen(buffer) - 1] != word[strlen(word) - 1]) {
        char buffer[255];
        sprintf(buffer, "Please enter word, which ends with: %c\n", word[0]);
        int ret = write(client.fd, buffer, strlen(buffer));
        if (ret <= 0) {
          log_msg(LOG_ERROR, "Unable to send message to client");
          closeClient(position);
          return;
        }
        continue;
      }

      memcpy(word, buffer, strlen(buffer) + 1); // with end of string

      findNewClient(position);

      break;
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

  // initialize
  for (int i = 0; i < MAX_CLIENTS; ++i) {
    clients[i].fd = -1;
    sem_init(&clients[i].mutex, 0, i == 0 ? 1 : 0);
  }

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

        int pos = -1;

        for (int i = 0; i < MAX_CLIENTS; ++i) {
          if (clients[i].fd == -1) {
            pos = i;
            break;
          }
        }

        if (pos == -1) {
          static const char *err = "Player limit is full, please wait.";
          write(l_sock_client, err, strlen(err));
          close(l_sock_client);
          log_msg(LOG_INFO, "Rejected client, because limit is full.");
          continue;
        }

        clients[pos].fd = l_sock_client;

        std::thread thread(client, pos);
        thread.detach();
      }

    } // while wait for client
  } // while ( 1 )

  return 0;
}
