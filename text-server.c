#include "text-memory.h"

static char *FILE__;

int main() {

// *********** USING DOMAIN SOCKETS *********** //

  int server_sockfd, client_sockfd, n;
  struct sockaddr_un server_addr;
  struct sockaddr_un client_addr;

  if (get_nprocs_conf() > 1) {
    n = get_nprocs_conf() - 1;
  } else {
    n = get_nprocs_conf();
  }
  
  unlink(SOCKET);
  server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET, sizeof(server_addr.sun_path) - 1);
  int success =
      bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof server_addr);

  listen(server_sockfd, n);

// ************ START OF THE SERVER ************ //

  fprintf(stdout, "SERVER STARTED\n");

  while (1) {
    int client_len = sizeof client_addr;
    char filename[T_SIZE], state[T_SIZE];
    client_sockfd =
        accept(server_sockfd, (struct sockaddr *)&client_addr, &(client_len));

// *********** RECEIVING THE FILE NAME ********* //

    fprintf(stderr, "CLIENT REQUEST RECEIVED");
    read(client_sockfd, filename, T_SIZE);
    fprintf(stderr, "\tOPENING: %s", filename);

    int fd = open(filename, O_RDWR);

// * CHECKING IF THE FILE IS VALID USING FILE DESCRIPTOR * //

    if (fd == -1) {
      write(client_sockfd, INVALID, sizeof(INVALID));
      fprintf(stderr, "\tINVALID FILE\n");
      close(client_sockfd);
      continue;
    }

// ********* IF VALID, THEN THE PROGRAM PROCEDES ********* //

    write(client_sockfd, VALID, sizeof(VALID));

// ****************** MEMORY MAPPED ********************** //

    struct stat sb;
    fstat(fd, &sb);
    FILE__ = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    fprintf(stderr, "\tFILE MAPPED TO SHARED MEMORY");

// * AFTER RECEIVING A SIGNAL FROM THE CLIENT, THE FILE IS CLOSED * //

    read(client_sockfd, state, T_SIZE);
    state[strlen(state)] = '\0';

    if (!strcmp(state, DONE)) {
      close(fd);
      fprintf(stderr, "\tFILE CLOSED\n");
    }

    close(client_sockfd);
  }
  return 0;
}