#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "shm.h"

int main() {
  int shmid;
  char *ptr, *pData;
  int *pInt;

  /* SHM_KEY, SHM_SIZE, SHM_MODE in shm.h */
  if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) == -1) {
    perror("shmget");
    exit(1);
  }  // shared mem get
  if ((ptr = shmat(shmid, 0, 0)) == (void *)-1) {
    perror("shmat");
    exit(1);
  }

  pInt = (int *)ptr;
  while ((*pInt) == 0)  // pInt가 변경될 때까지 기다림
    ;

  pData = ptr + sizeof(int);  // 상태를 나타내는 int 넘기기
  printf("Received request: %s.....", pData);
  sprintf(pData, "This is a reply from %d.", getpid());
  *pInt = 0;  // 반환 완료.
  printf("Replied.\n");

  sleep(1);

  /* Detach shared memory */
  if (shmdt(ptr) == -1) {
    perror("shmdt");
    exit(1);
  }

  /* Remove shared memory */
  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("shmctl");
    exit(1);
  }
}
