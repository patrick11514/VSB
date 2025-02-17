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
#include <mutex>
#include <netinet/in.h>
#include <poll.h>
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
#include <vector>

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

std::mutex mutex;

void closeClient(int l_sock_client, std::vector<int> &clients) {
  close(l_sock_client);
  for (auto it = clients.begin(), end = clients.end(); it != end; ++it) {
    if (*it == l_sock_client) {
      std::lock_guard<std::mutex> lock(mutex);
      clients.erase(it);
      return;
    }
  }
}

void childHandling(int l_sock_client, std::vector<int> &clients) {
  while (1) { // communication
    char l_buf[256];

    // read data from socket
    int l_len = read(l_sock_client, l_buf, sizeof(l_buf));
    printf("readed_data\n");
    if (!l_len) {
      log_msg(LOG_DEBUG, "Client closed socket!");
      closeClient(l_sock_client, clients);
      return;
    } else if (l_len < 0) {
      log_msg(LOG_ERROR, "Unable to read data from client.");
      closeClient(l_sock_client, clients);
      return;
    } else
      log_msg(LOG_DEBUG, "Read %d bytes from client.", l_len);

    // write data to client
    l_len = write(STDOUT_FILENO, l_buf, l_len);
    if (l_len < 0)
      log_msg(LOG_ERROR, "Unable to write data to stdout.");

    // close request?
    if (!strncasecmp(l_buf, "close", strlen(STR_CLOSE))) {
      log_msg(LOG_INFO, "Client sent 'close' request to close connection.");
      closeClient(l_sock_client, clients);
      return;
    }

    if (l_buf[l_len - 1] != '\n') {
      log_msg(LOG_ERROR, "Message doesn't end with new-line.");
      closeClient(l_sock_client, clients);
      return;
    }

    int index = 0;
    int sum = 0;
    char lastSymbol = '\0';

    // ignore newline at the end
    --l_len;
    l_buf[l_len] = '\0';

    printf("READED: %s\n", l_buf);

    while (index < l_len) {
      int num, offset;
      char symbol;

      int readed = sscanf(l_buf + index, "%d%c%n", &num, &symbol, &offset);

      if (readed < 1) {
        log_msg(LOG_ERROR, "Invalid format");
        break;
      }

      index += offset;

      if (lastSymbol != '\0')
        switch (lastSymbol) {
        case '+':
          sum += num;
          break;
        case '-':
          sum -= num;
          break;
        }
      else
        sum = num;

      if (readed == 2)
        lastSymbol = symbol;

      if (index >= l_len) {
        printf("sending %d, %d, %d\n", index, l_len, getpid());
        char buffer[300];
        sprintf(buffer, "%s=%d\n", l_buf, sum);

        for (auto client : clients) {
          int len = write(client, buffer, strlen(buffer));
          if (len < 0)
            log_msg(LOG_ERROR, "Unable to write data to client.");
        }
      }
    }
  }
}

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

  std::vector<int> clients;

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

        std::thread thread(childHandling, l_sock_client, std::ref(clients));
        thread.detach();
        clients.emplace_back(l_sock_client);
      }

    } // while wait for client

  } // while ( 1 )

  return 0;
}
