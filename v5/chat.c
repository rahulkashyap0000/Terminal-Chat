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
char uid;

// shm - Store karega ki session start ho gaya yaa nahi
// shm1 - Store karega latest person ki UID
// shm2 - Store karega current status

// shm3 - Likhna chalu hoga


void *reading(char *shm){
  char *s;
  *(shm+2) = 'i';
  
  while(1){
    if(*(shm+2) == 'i'){
      sleep(0.001);
    }
    else if(*shm + 2 == uid){
    }
    else{
      s = shm + 3;
      printf("%c: ", *(shm+2));
      while(*s != 0){
        putchar(*s++);
      }
      *(shm+2) = 'i';
    }
  }
}

// 
void *writing(char *shm){
  char *s;
  while(1){
    s = shm + 3;
    char mess[50];
    fgets(mess, sizeof(mess), stdin);
    // Person B is writing:
    for(int i = 0; shm[i] != '\0'; i++){
      *s++ = mess[i];
    }
    *(shm+2) = uid;
    *s = 0;
  }
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


    // Mera code yahan se chaalu hota hai
    char session_status = *shm;
    char latest_uid = *(shm + 1);
    if(*shm == 'x'){
      printf("Already Opened\n");
      uid = (*(shm+1)) + 1;
      printf("\nThe uid of the new user is %c", uid);
    }
    else{
      printf("Welcome Admin\n");
      *shm = 'x';
      uid = *(shm+1) = 'a';
    }
  /* Now read whatever server has put in into shared memory */
    pthread_create(&pt1, NULL, reading,shm);
    pthread_create(&pt2, NULL, writing,shm);
    pthread_join(pt1, NULL);
    // reading(shm);
    // writing(shm);
     exit(0);

}
        

