#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/types.h>

#define SHMSIZE	27  /* Size of the shared memory */

int main(){

    char c;
    int shmid;
    key_t key;
    char *shm,*s;
    key = 4015; /* Some key to uniquely identify shared memory */
    printf("Please enter the key port\n");
    scanf("%d", &key);

    /* Create Segment */ 

    if((shmid=shmget(key,SHMSIZE,IPC_CREAT | 0666))< 0){
        perror("shmget");
        exit(1);
    }

   /* Attach the segment to our data space */

    if((shm=shmat(shmid,NULL,0))==(char *) -1){
        perror("shmat");
        exit(0);
    }

  /* Now put something into shared memory */

    s = shm;
    // for(c = 'A';c<='Z';c++)
    //    *s++=c;
    while(1){
      printf("> ");
      char mess[50];
      fgets(mess, sizeof(mess), stdin);
      for(int i = 0; mess[i] != '\0'; i++){
        *s++=mess[i];
      }
      // *s++= '\n';
    } /* End with NULL termination */
      *s = 0;

  /* Wait until the other process changes the first character to '*' in the shared memory */

     while(*shm != '*');
       sleep(1);
     exit(0);

}
        

