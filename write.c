#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define KEY 24602

int main() {
  int shmd, semd, file;
  int* data;

  semd = semget(KEY, 1, 0);
  if(semget < 0) {
    printf("Error with semaphore.\n");
    return 1;
  }

  file = open("text.txt", O_RDWR | O_APPEND);

  shmd = shmget(KEY, 0, 0);
  data = shmat(shmd, 0, 0);

  struct sembuf sb;
  sb.sem_op = -1;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;

  char* temp = malloc(*data);

  semop(semd, &sb, 1);
  lseek(file, -(*data), SEEK_END);
  read(file, temp, *data);
  printf("Last line: %s\n", temp);

  printf("Input new line: ");
  char newline[100];

  fgets(newline, sizeof(newline), stdin);

  newline[strcspn(newline, "\n")] = '\0';
  write(file, newline, strlen(newline));

  *data = strlen(newline);
  shmdt(data);
  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;

}

