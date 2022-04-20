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
    }
  }

  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

// ************** END OF FUNCTION************** //

int main(int argc, char *argv[]) {

  char valid[T_SIZE];
  struct sockaddr_un address;

// ************** DOMAIN SOCKET *************** //

  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  address.sun_family = AF_UNIX;
  strncpy(address.sun_path, SOCKET, sizeof(address.sun_path) - 1);
  int rv = connect(sockfd, (struct sockaddr *)&address, sizeof address);

// ********* READING THE FILENAME ************* //

  write(sockfd, argv[1], T_SIZE);
  read(sockfd, valid, T_SIZE);
  valid[strlen(valid)] = '\0';

// ****** CHECKING IF THE FILE IS VALID ******* //

  if (!strcmp(valid, INVALID)) {
    fprintf(stderr, "INVALID FILE\n");
    close(sockfd);
    return FAILURE;
  }

// ************ SETUP FOR MMAPING ************* //

  pthread_t threads[THREADS];
  struct PASS pass[THREADS];
  pthread_mutex_init(&mutex, NULL);

  int fdp = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
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

// *********************** CLOSING ************************* //

  close(fdp);
  pthread_mutex_destroy(&mutex);

  write(sockfd, DONE, DONE_SIZE);

  close(sockfd);
  return SUCCESS;
}