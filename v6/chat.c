#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>
#define p(a) printf("\e[1;%dm", a)
#define normal() printf("\e[0m")
#define rd "\x1b[31m"
#define rst "\x1b[0m"

#define SHMSIZE 27 /* Size of the shared memory */
char uid;
// str title;
// shm - Store karega ki session start ho gaya yaa nahi
// shm1 - Store karega latest person ki UID
// shm2 - Store karega current status

// shm3 - Likhna chalu hoga

void *reading(char *shm)
{
    char *s;
    *(shm + 2) = 'i';
    bool check = 0;
    while (1)
    {
        if (*(shm + 2) == 'i')
        {
            check = 0;
            sleep(0.001);
        }
        else if (*(shm + 2) == uid)
        {
            // printf("UID is %c", *(shm+2));
        }
        else
        {
            if (!check && *(shm + 2) != 'i')
            {
                check = 1;
                s = shm + 3;
                if (*s == '\n')
                {
                    int x = *(shm + 2) - 97 + 31;
                    p(x);
                    printf("\n\t %c \e[0mhas joined the chat\n", *(shm + 2));
                    normal();
                }
                else
                {
                    int x = *(shm + 2) - 97 + 31;
                    p(x);
                    printf("%c: ", *(shm + 2));
                    normal();
                    while (*s != 0)
                    {
                        putchar(*s++);
                    }
                    x = uid - 97 + 31;
                    p(x);
                }
            }
        }
    }
}

//
void *writing(char *shm)
{
    char *s;
    while (1)
    {
        sleep(1);
        *(shm + 2) = 'i';
        s = shm + 3;
        char mess[50];
        fgets(mess, sizeof(mess), stdin);
        // Person B is writing:
        for (int i = 0; shm[i] != '\0'; i++)
        {
            *s++ = mess[i];
        }
        *(shm + 2) = uid;
        *s = 0;
    }
}

int main()
{
    pthread_t pt1, pt2;
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    // key = 4115; /* We need to get the segment named '5678' crated by the server */
    printf("Please enter the key port\n");
    scanf("%d", &key);
    char title[50];
    key_t key1 = 9005;
    char *shm1, *s1;
    int shm1id;

    /* Locate the Segment */
    if ((shmid = shmget(key, SHMSIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }
    /* Attach the segment to our data space */

    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(0);
    }
    if((shmid = shmget(key1, SHMSIZE, IPC_CREAT | 0666)) < 0){
        perror("shmget");
        exit(1);
    }
    if ((shm1 = shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(0);
    }


    if (*shm == 'x')
    {
        s = shm1;
        while (*s != 0)
        {
            putchar(*s++);
        }
        uid = (*(shm + 1)) + 1;
        *(shm + 1) += 1;
        int x = uid - 97 + 31;
        printf("\nWelcome \e[1;%dm%c\n", x, uid);
    }
    else
    {
        printf("Enter the title to the group\n");
        char title[50];
        fgets(title, sizeof(title), stdin);
        s = shm1;
        for (int i = 0; shm1[i] != '\0'; i++)
        {
            *s++ = title[i];
        }
        *(shm1 + 2) = uid;
        *s = 0;
        *shm = 'x';
        uid = *(shm + 1) = 'a';
    }
    /* Now read whatever server has put in into shared memory */
    pthread_create(&pt1, NULL, reading, shm);
    // sleep(1);
    pthread_create(&pt2, NULL, writing, shm);
    pthread_join(pt1, NULL);
    exit(0);
}
