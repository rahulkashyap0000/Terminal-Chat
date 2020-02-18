#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/types.h>
#include <unistd.h>  
#include<pthread.h>
#include <math.h>
#include <sys/time.h>

#define SHMSIZE	27  /* Size of the shared memory */


void *writing(char *shm){
  char *s;
  while(1){
    s = shm + 1;
    char mess[50];
    fgets(mess, sizeof(mess), stdin);
    // Person B is writing:
    for(int i = 0; shm[i] != '\0'; i++){
      *s++ = mess[i];
    }
    *shm = 'A';
    *s = 0;
  }
}
void *reading(char *shm){
  // printf("PReading Opened\n");
  char *s;
  *shm = 'i';
  while(1){
    // If there has been no input now, check after 10ms
    if(*shm == 'i'){
      sleep(0.001);
    }
    // If there is input
    else if(*shm != 'A' && *shm != 'i'){
      s = shm + 1;
      printf("%c: ", *shm);
      while(*s != 0){
        putchar(*s++);
      }
      *shm = 'i';
    }
  }
  return;
}

int main(){
    pthread_t pt1, pt2;
    char c;
    int shmid;
    key_t key;
    char *shm,*s;

    key = 4115; /* We need to get the segment named '5678' crated by the server */
    printf("Please enter the key port\n");
    scanf("%d", &key);

    /* Locate the Segment */

    if((shmid=shmget(key,SHMSIZE,IPC_CREAT | 0666))< 0){
        perror("shmget");
        exit(1);
    }

   /* Attach the segment to our data space */

    if((shm=shmat(shmid,NULL,0))==(char *) -1){
        perror("shmat");
        exit(0);
    }

  /* Now read whatever server has put in into shared memory */
    pthread_create(&pt1, NULL, reading,shm);
    pthread_create(&pt2, NULL, writing,shm);
    pthread_join(pt1, NULL);
    // reading(shm);
    // writing(shm);
     exit(0);

}
        

