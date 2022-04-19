#include "text-memory.h"
pthread_mutex_t mutex;

int FILE_SIZE;
static char *FILE__;

// ************** THREAD FUNCTION ************** //

void *thread(void *arg) {

  pthread_mutex_lock(&mutex);

  struct PASS *pass = (struct PASS *)arg;

  for (int i = pass->start; i < pass->end; i++) {
    if (islower(FILE__[i])) {
      FILE__[i] = toupper(FILE__[i]);
    } else if (isupper(FILE__[i])) {
      FILE__[i] = tolower(FILE__[i]);
    }
  }

  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

// ************** END OF FUNCTION************** //

int main(int argc, char *argv[]) {

  char filename[M_SIZE], tmp[M_SIZE];

  // ************** NAMED PIPE W/WRITE_ONLY ***************** //

  mkfifo(MYFIFO, 0666);
  int fd = open(MYFIFO, O_WRONLY);
  strcpy(filename, argv[1]);
  write(fd, filename, T_SIZE);
  close(fd);

  // *************** NAMED PIPE W/READ_ONLY ****************** //

  fd = open(MYFIFO, O_RDONLY);
  read(fd, tmp, VALID_SIZE);
  close(fd);

  if (strstr(VALID, tmp)) {

    pthread_t threads[THREADS];
    struct PASS pass[THREADS];
    pthread_mutex_init(&mutex, NULL);

    int fdp = open(filename, O_RDWR, S_IRUSR | S_IWUSR);
    struct stat sb;
    fstat(fdp, &sb);

    FILE__ = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdp, 0);
    int size = strlen(FILE__);
    FILE_SIZE = sb.st_size;

    // ***************** SPINNING THREADS ********************* //
    for (int i = 0; i < THREADS; i++) {
      int start = i * FILE_SIZE / THREADS, end = (i + 1) * FILE_SIZE / THREADS;
      pass[i].start = start;
      pass[i].end = end;
      if (pthread_create(&threads[i], NULL, thread, &pass[i]) != 0) {
        perror("thread failed to create\n");
      }
    }

    // ****************** JOINING THREADS ******************** //

    for (int i = 0; i < THREADS; i++) {
      if (pthread_join(threads[i], NULL) != 0) {
        perror("failed to join\n");
      }
    }

    close(fdp);
    pthread_mutex_destroy(&mutex);
    fd = open(MYFIFO, O_WRONLY);
    strcpy(tmp, DONE);
    write(fd, DONE, DONE_SIZE);
    close(fd);
  } else if (strstr(INVALID, tmp)) {
    fprintf(stderr, "INVALID FILE\n");
    exit(FAILURE);
  }

  return SUCCESS;
}