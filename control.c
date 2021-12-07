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

union semun {
  int val;                  //used for SETVAL
  struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
  unsigned short  *array;   //used for SETALL
  struct seminfo  *__buf;
};

int main() {
  int shmd, semd, file;
  int* data;

  printf("Create resources? (y for yes, n for removal): ");
  char temp[100];
  fgets(temp, sizeof(temp), stdin);
  temp[strcspn(temp, "\n")] = '\0';

  if(strcmp(temp, "y") != 0 && strcmp(temp, "n") != 0) {
    printf("Please input \"y\" or \"n\".\n");
  }

  else if(strcmp(temp, "y") == 0) {
    semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if(semd == -1) {
      printf("Error with semaphore.\n");
      return 1;
    }
    else {
      union semun us;
      us.val = 1;
      semctl(semd, 0, SETVAL, us);
    }

    shmd = shmget(KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0640);
    if(shmd == -1)  {
      printf("Error with shared memory.\n");
      return 1;
    }
    else {
      data = shmat(shmd, 0, 0);
      *data = 0;
      shmdt(data);
    }

    file = open("text.txt", O_CREAT | O_WRONLY | O_TRUNC, 0664);
  }

  else if(strcmp(temp, "n") == 0) {
    shmd = shmget(KEY, 0, 0);
    if(shmd == -1) {
      printf("Error with shared memory.\n");
      return 1;
    }
    else {
      shmctl(shmd, IPC_RMID, 0);
    }

    semd = semget(KEY, 1, 0);
    if(semd == -1) {
      printf("Error with semaphore.\n");
      return 1;
    }
    else {
      semctl(semd, IPC_RMID, 0);
    }

    file = open("text.txt", O_RDONLY);
    if(file == -1) {
      printf("Error with file.\n");
      return 1;
    }

    struct stat st;
    stat("test.txt", &st);
    char* words = malloc(st.st_size);
    int check = read(file, words, st.st_size);
    if(check == -1) {
      printf("Error with file.\n");
    }

    printf("The story:\n");
    printf("%s\n", words);
    free(words);
    close(file);
  }

  return 0;
}
