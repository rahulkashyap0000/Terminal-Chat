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

    key = 4115; /* We need to get the segment named '5678' crated by the server */
    printf("Please enter the key port\n");
    scanf("%d", &key);
    char title[50];

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
    int array_id;
    char strings[10][50];

    int fd1;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    char str1[80], str2[80];
    // Mera code yahan se chaalu hota hai
    char session_status = *shm;
    char latest_uid = *(shm + 1);
    if (*shm == 'x')
    {
        system("clear");
        fd1 = open(myfifo, O_RDONLY);
        read(fd1, str1, 80);
        p(32);
        printf("\t%s\n\n", str1);
        normal();
        close(fd1);
        fd1 = open(myfifo, O_WRONLY);
        write(fd1, str1, strlen(str1) + 1);
        close(fd1);

        uid = (*(shm + 1)) + 1;
        *(shm + 1) += 1;
        int x = uid - 97 + 31;
        printf("\nWelcome \e[1;%d%c\n", x, uid);
    }
    else
    {
        printf("Enter the title to the group\n");
        fd1 = open(myfifo, O_WRONLY);
        fgets(str2, 80, stdin);
        write(fd1, str2, strlen(str2) + 1);
        close(fd1);
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
