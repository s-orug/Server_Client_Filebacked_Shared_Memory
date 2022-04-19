#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/mman.h>
#include <ctype.h>

#define M_SIZE 64
#define T_SIZE 64
#define VALID_SIZE 14
#define DONE_SIZE 5

#define FAILURE (1)
#define SUCCESS (0)

#define VALID "VALID_"
#define DONE "DONE"
#define INVALID "IN_VALID FILE"
#define MYFIFO "/tmp/myfifo"

#define THREADS (2)

struct PASS{
  int start, end;
};