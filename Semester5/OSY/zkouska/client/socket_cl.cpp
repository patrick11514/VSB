//***************************************************************************
//
// Program example for subject Operating Systems
//
// Petr Olivka, Dept. of Computer Science, petr.olivka@vsb.cz, 2021
//
// Example of socket server/client.
//
// This program is example of socket client.
// The mandatory arguments of program is IP adress or name of server and
// a port number.
//
//***************************************************************************

#include <arpa/inet.h>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
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
#include <sys/wait.h>
#include <unistd.h>

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
           "  Socket client example.\n"
           "\n"
           "  Use: %s [-h -d] ip_or_name port_number\n"
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

int main(int t_narg, char **t_args) {

  if (t_narg <= 2)
    help(t_narg, t_args);

  if (t_narg <= 4) {
    printf("Please enter search string + file name\n");
    return -1;
  }

  int l_port = 0;
  char *l_host = nullptr;

  // parsing arguments
  for (int i = 1; i < t_narg; i++) {
    if (!strcmp(t_args[i], "-d"))
      g_debug = LOG_DEBUG;

    if (!strcmp(t_args[i], "-h"))
      help(t_narg, t_args);

    if (*t_args[i] != '-') {
      if (!l_host)
        l_host = t_args[i];
      else if (!l_port)
        l_port = atoi(t_args[i]);
    }
  }

  if (!l_host || !l_port) {
    log_msg(LOG_INFO, "Host or port is missing!");
    help(t_narg, t_args);
    exit(1);
  }

  log_msg(LOG_INFO, "Connection to '%s':%d.", l_host, l_port);

  addrinfo l_ai_req, *l_ai_ans;
  bzero(&l_ai_req, sizeof(l_ai_req));
  l_ai_req.ai_family = AF_INET;
  l_ai_req.ai_socktype = SOCK_STREAM;

  int l_get_ai = getaddrinfo(l_host, nullptr, &l_ai_req, &l_ai_ans);
  if (l_get_ai) {
    log_msg(LOG_ERROR, "Unknown host name!");
    exit(1);
  }

  sockaddr_in l_cl_addr = *(sockaddr_in *)l_ai_ans->ai_addr;
  l_cl_addr.sin_port = htons(l_port);
  freeaddrinfo(l_ai_ans);

  // socket creation
  int l_sock_server = socket(AF_INET, SOCK_STREAM, 0);
  if (l_sock_server == -1) {
    log_msg(LOG_ERROR, "Unable to create socket.");
    exit(1);
  }

  // connect to server
  if (connect(l_sock_server, (sockaddr *)&l_cl_addr, sizeof(l_cl_addr)) < 0) {
    log_msg(LOG_ERROR, "Unable to connect server.");
    exit(1);
  }

  uint l_lsa = sizeof(l_cl_addr);
  // my IP
  getsockname(l_sock_server, (sockaddr *)&l_cl_addr, &l_lsa);
  log_msg(LOG_INFO, "My IP: '%s'  port: %d", inet_ntoa(l_cl_addr.sin_addr),
          ntohs(l_cl_addr.sin_port));
  // server IP
  getpeername(l_sock_server, (sockaddr *)&l_cl_addr, &l_lsa);
  log_msg(LOG_INFO, "Server IP: '%s'  port: %d", inet_ntoa(l_cl_addr.sin_addr),
          ntohs(l_cl_addr.sin_port));

  log_msg(LOG_INFO, "Enter 'close' to close application.");

  // send search string
  char buffer[255];
  sprintf(buffer, "%s\n", t_args[3]);
  int ret = write(l_sock_server, buffer, strlen(buffer));
  if (ret < 0) {
    log_msg(LOG_INFO, "Unable to send search string");
    return -1;
  }

  // check for ok
  int len = read(l_sock_server, buffer, sizeof(buffer));
  if (len <= 0) {
    log_msg(LOG_INFO, "Unable to read OK");
    return -1;
  }
  buffer[len] = 0;

  if (strcmp(buffer, "OK\n") != 0) {
    log_msg(LOG_INFO, "Didnt get OK");
    return -1;
  }

  if (fork() == 0) {
    dup2(l_sock_server, STDOUT_FILENO);
    execlp("./slowcat", "slowcat", t_args[4], nullptr);
    return EXIT_SUCCESS;
  }

  // open file
  char fileName[255];
  sprintf(fileName, "%s.out", t_args[3]);
  int outFile = open(fileName, O_CREAT | O_WRONLY, 0666);

  // go!
  while (1) {
    char l_buf[128];

    // read data from server
    int l_len = read(l_sock_server, l_buf, sizeof(l_buf));
    if (!l_len) {
      log_msg(LOG_DEBUG, "Server closed socket.");
      break;
    } else if (l_len < 0) {
      log_msg(LOG_ERROR, "Unable to read data from server.");
      break;
    } else
      log_msg(LOG_DEBUG, "Read %d bytes from server.", l_len);

    // display on stdout
    l_len = write(STDOUT_FILENO, l_buf, l_len);
    if (l_len < 0)
      log_msg(LOG_ERROR, "Unable to write to stdout.");

    // write to file
    l_len = write(outFile, l_buf, l_len);
    if (l_len < 0)
      log_msg(LOG_ERROR, "Unable to write to outFile.");
  }

  close(outFile);
  // close socket
  close(l_sock_server);

  wait(NULL);

  return 0;
}
