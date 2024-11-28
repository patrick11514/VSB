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
#include <array>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
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

struct Pokladnicka {
  int suma;
  char mince[1024];
};

const char *securityQueue = "/security";
const char *donorsQueue = "/donors";

void closeCl(int fd) {
  log_msg(LOG_INFO, "Closing client");
  close(fd);
}

const std::array<int, 6> coins{1, 2, 5, 10, 20, 50};

void donorHandler(int fd, mqd_t &mQueue) {
  mqd_t security = -1;

  int mainBuffer[1024];
  int mainIndex = 0;

  int nEmptyLines = 0;
  bool multiline = false;

  while (true) {
    char buffer[1024];
    int ret = read(fd, buffer, sizeof(buffer));
    if (ret <= 0) {
      return closeCl(fd);
    }
    buffer[ret] = 0;

    /*if (ret == 1 && buffer[0] == '\n') {
      if (multiline) {
        --nEmptyLines;
      } else {
        ++nEmptyLines;
      }

      if (!multiline && nEmptyLines >= 2) {
        multiline = true;
      }

      if (multiline && nEmptyLines == 0) {
        mainBuffer[mainIndex] = 0;
        if (mq_send(mQueue, mainBuffer, strlen(mainBuffer), 0) < 0) {
          log_msg(LOG_INFO, "Unable to send message to mq");
        }

        mainIndex = 0;
        multiline = false;
      }

      continue;
    }

    if (multiline) {
      memcpy(mainBuffer + mainIndex, buffer, ret);
      mainIndex += ret;
      continue;
    }*/

    printf("BUff: %s\n", buffer);

    int money;
    sscanf(buffer, "%d\n", &money);

    int valid = false;
    for (auto coin : coins) {
      if (coin == money) {
        valid = true;
        break;
      }
    }

    printf("Got %d %d\n", money, valid);

    if (!valid) {
      const char *err = "Invalid amount.\n";
      int ret = write(fd, err, strlen(err));
      if (ret <= 0) {
        log_msg(LOG_INFO, "Unable to send message to client");
      }

      continue;
    }

    Pokladnicka pokladnicka;

    ret = mq_receive(mQueue, (char *)&pokladnicka, sizeof(pokladnicka), NULL);
    if (ret <= 0) {
      log_msg(LOG_INFO, "Unable to get Pokladnicka from queue");
      continue;
    }

    pokladnicka.suma += money;
    sprintf(pokladnicka.mince, "%s%d", pokladnicka.mince, money);

    printf("%d money\n", pokladnicka.suma);

    if (pokladnicka.suma < 100) {
      int ret =
          mq_send(mQueue, (const char *)&pokladnicka, sizeof(pokladnicka), 0);
      if (ret < 0) {
        log_msg(LOG_INFO, "Unable to return Pokladnicka into donor queue");
      }
      continue;
    }

    if (security == -1) {
      security = mq_open(securityQueue, O_CREAT | O_RDWR, 0600, NULL);
    }

    ret = mq_send(security, (const char *)&pokladnicka, sizeof(pokladnicka), 0);
    if (ret < 0) {
      log_msg(LOG_INFO, "Unable to return Pokladnicka into security queue");
    }
    continue;
  }
};

void securityHandler(int fd, mqd_t &mQueue) {
  mqd_t donors = mq_open(donorsQueue, O_CREAT | O_RDWR, 0600, NULL);

  while (true) {
    Pokladnicka pokladnicka;
    int ret =
        mq_receive(mQueue, (char *)&pokladnicka, sizeof(pokladnicka), NULL);
    if (ret <= 0) {
      log_msg(LOG_INFO, "Unable to receive Pokladnicka from security queue");
      continue;
    }

    char buffer[1024];
    sprintf(buffer, "Emptying Pokladnicka\nMoney: %d\nList: %s\n",
            pokladnicka.suma, pokladnicka.mince);
    ret = write(fd, buffer, strlen(buffer));
    if (ret <= 0) {
      log_msg(LOG_INFO, "Unable to send message to security");
      continue;
    }

    Pokladnicka newPokladnicka;
    newPokladnicka.suma = 0;
    ret =
        mq_send(donors, (const char *)&newPokladnicka, sizeof(Pokladnicka), 0);
    if (ret < 0) {
      log_msg(LOG_INFO, "Unable to return Pokladnicka into dononors queue");
    }
  }
}

struct ClientConnection {
  int fd;
  int pid;
};

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

  for (int i = 0; i < t_narg; ++i) {
    char *arg = t_args[i];
    if (strcmp(arg, "-i") == 0) {
      init = true;
    } else if (strcmp(arg, "-d") == 0) {
      del = true;
    }
  }

  if (del) {
    mq_unlink(securityQueue);
    mq_unlink(donorsQueue);
  }

  if (init) {
    mq_attr attrs;
    attrs.mq_msgsize = sizeof(Pokladnicka);
    attrs.mq_maxmsg = 8;
    mq_open(securityQueue, O_CREAT | O_RDWR, 0600, &attrs);
    int mqueue = mq_open(donorsQueue, O_CREAT | O_RDWR, 0600, &attrs);

    Pokladnicka pokladnicka;
    pokladnicka.suma = 0;

    mq_send(mqueue, (const char *)&pokladnicka, sizeof(pokladnicka), 0);
  }

  mqd_t msgQueue = mq_open(l_port % 2 == 0 ? securityQueue : donorsQueue,
                           O_CREAT | O_RDWR, 0600, NULL);

  // go!
  while (1) {
    int l_sock_client = -1;

    // list of fd sources
    pollfd l_read_poll[3];

    l_read_poll[0].fd = STDIN_FILENO;
    l_read_poll[0].events = POLLIN;
    l_read_poll[1].fd = l_sock_listen;
    l_read_poll[1].events = POLLIN;
    while (1) // wait for new client
    {
      // select from fds
      int l_poll = poll(l_read_poll, 3, -1);

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

        int pid = fork();

        if (pid == 0) {
          close(l_sock_listen);

          if (l_port % 2 == 0) {
            securityHandler(l_sock_client, msgQueue);
          } else {
            donorHandler(l_sock_client, msgQueue);
          }
          return 0;
        }

        close(l_sock_client);
      }

    } // while wait for client

  } // while ( 1 )

  mq_unlink(l_port % 2 == 0 ? securityQueue : donorsQueue);

  return 0;
}
