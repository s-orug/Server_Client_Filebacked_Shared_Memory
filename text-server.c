#include "text-memory.h"

static char *FILE__;

int main() {

  fprintf(stdout, "SERVER STARTED\n");

  while (1) {
    char filename[M_SIZE], state[M_SIZE], valid[VALID_SIZE];

// *************** NAMED PIPE W/READ_ONLY ****************** //

    mkfifo(MYFIFO, 0666);
    int npd = open(MYFIFO, O_RDONLY);

// *** CLEARING THE BUFFER BEFORE READING THE FILE NAME *** //

    memset(filename, '\0', sizeof(filename));

// **** READS THE FILENAME AND LOGS THE CLIENT REQUEST **** //

    if (read(npd, filename, T_SIZE)) {
      fprintf(stderr, "CLIENT REQUEST RECEIVED");
      close(npd);

// ************** NAMED PIPE W/WRITE_ONLY ***************** //

      npd = open(MYFIFO, O_WRONLY);
      int fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR);

// ******* LOGGING THE NAME OF THE RECEIVED FILE ********** //

      fprintf(stderr, "\tOPENING: %s", filename);

// ************ CHECKING IF THE FILE IS VALID ************** //
// * IF THE FILE IS INVALID CONTINUES TO THE NEXT ITERATION  //
      
      memset(valid, '\0', sizeof(valid));
      struct stat sb;
      if (fstat(fd, &sb) == -1) {
        strcpy(valid, INVALID);
        write(npd, INVALID, VALID_SIZE);
      }

// * IF THE FILE IS VALID, THEN THE FILE IS MAPPED USING MMAP //

      else {
        strcpy(valid, VALID);
        write(npd, valid, VALID_SIZE);
        FILE__ =
            mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        fprintf(stderr, "\tFILE MAPPED TO SHARED MEMORY");
      }

// *************** NAMED PIPE W/READ_ONLY ****************** //
      int npd = open(MYFIFO, O_RDONLY);
      memset(state, '\0', sizeof(state));

      while(!read(npd, state, T_SIZE)){
        /* Waits */
      }
// ******** CLOSING THE FILE USING FILE DESCRIPTOR ********* //
      fprintf(stderr, "\tFILE CLOSED\n");
      munmap(FILE__, sb.st_size);
      close(npd);
      close(fd);
    }
  }

  return 0;
}