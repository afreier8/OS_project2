
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 5

int end;
int i,j;
char buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
pthread_cond_t empty,full;

typedef struct strThreadData{
    int thread_num;
    int par_count;
} thdata;

void Producer(FILE *fp)
{
    
    char s1;
    
    s1 = fgetc(fp);
    while(1)
    {
        pthread_mutex_lock(&mutex);
        buffer[i] = s1;
        i = (i+1)%BUFFER_SIZE;
        
        if((i-1)%BUFFER_SIZE == j)
        {
            //   signal empty
            pthread_cond_signal(&empty);
        }
        if(i == j)
        {
            //		producer sleep
            pthread_cond_wait(&full, &mutex);
            //		producer awaken
        }
        pthread_mutex_unlock(&mutex);
        
        s1=fgetc(fp);
        if(feof(fp))
        {
            pthread_mutex_lock(&mutex);
            end = 1;
            
            pthread_cond_signal(&empty);
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
}
void Consumer(void *ptr)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(i == j)
        {
            if(end) break;
            else
            {
                pthread_cond_wait(&empty, &mutex);
                if(i == j && end) break;
            }
        }
        printf("%c",buffer[j]);
        j = (j+1)%BUFFER_SIZE;

        if(i == (j-1)%BUFFER_SIZE)
        {
            // signal full
            pthread_cond_signal(&full);
        }
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
    
}


int main(int argc, char *argv[])
{
    // PTHREADS producer and consumer
    pthread_t p, c;
    FILE *fp;
    
    i = 0;
    j = 0;
    
    // open messages.txt
    if((fp=fopen("messages.txt", "r"))==NULL)
    {
        printf("ERROR: within string.txt!\n");
        return -1;
    }
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&empty, NULL);
    pthread_cond_init (&full, NULL);
    
    end = 0;
    
    // create thread 1 and 2
    pthread_create (&p, NULL, (void *) &Producer, (FILE *) fp);
    pthread_create (&c, NULL, (void *) &Consumer, NULL);
    
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    
    return 1;
}
