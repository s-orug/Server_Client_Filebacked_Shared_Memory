#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <sys/un.h>

#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>


#define T_SIZE (64)
#define DONE_SIZE (5)

#define EOT (0x04)
#define ERROR (-1)

#define FAILURE (1)
#define SUCCESS (0)

#define DONE "DONE"
#define VALID "VALID"
#define INVALID "INVALID"
#define SOCKET "/tmp/orugants_domain_socket"


#define THREADS (4)

struct PASS{
  int start, end;
};