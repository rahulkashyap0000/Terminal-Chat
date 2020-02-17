#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/types.h>
#include <unistd.h>

#define SHMSIZE	27  /* Size of the shared memory */

int main(){

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
    s = shm;
    int a = 0;
    while(1){
      // for(s=shm; *s!=0;s++)
       putchar(*s++);
       a++;
       if(*s == 0){
         int check = 0;
         while(*(shm+a) == 0){
           sleep(1);
         }
         s = shm+a;
       }
      }
    //  *shm = '*';
     exit(0);

}
        

