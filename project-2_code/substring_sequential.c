#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024
#define NUM_THREADS 4


int n1,n2;
char *s1,*s2;
FILE *fp;


typedef struct str_threadData
{
    int threadNum;
    int parCount;
} threadData;



int readf(FILE *fp)
{
    if((fp=fopen("strings.txt", "r"))==NULL)
    {
        printf("ERROR: can't open string.txt!\n");
        return 0;
    }
    s1=(char *)malloc(sizeof(char)*MAX);
    if(s1==NULL)
    {
        printf("ERROR: Out of memory!\n");
        return -1;
    }
    s2=(char *)malloc(sizeof(char)*MAX);
    if(s1==NULL)
    {
        printf("ERROR: Out of memory\n");
        return -1;
    }
    // read s1,s2 from the file
    s1=fgets(s1, MAX, fp);
    s2=fgets(s2, MAX, fp);
    n1=strlen(s1);  /*length of s1*/
    n2=strlen(s2)-1; /*length of s2*/
    if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
        return -1;
}

int numSubstring(void)
{
    int i,j,k;
    int count;
    int total = 0;
    
    for (i = 0; i <= (n1-n2); i++){
        count=0;
        
        //search for the next string of size of n2
        for(j = i,k = 0; k < n2; j++,k++){
            if (*(s1+j)!=*(s2+k)){
                break;
            }
            else
                count++;
            // find a substring in this step
            if(count==n2)
                total++;
        }
    }
    return total;
}

void numSubstringPartial( void *ptr ){
    int i,j,k;
    int count;
    int total = 0;
    
    for (i =((threadData*) ptr)->threadNum; i <= (n1-n2); i = i + NUM_THREADS)
    {
        count=0;
        // search for the next string of size of n2
        for(j = i,k = 0; k < n2; j++,k++)
        {
            if (*(s1+j)!=*(s2+k)){
                break;
            }
            else
                count++;
            if(count==n2)
                total++;		/*find a substring in this step*/
        }
    }
    ((threadData*)ptr)->parCount = total;
}

int main(int argc, char *argv[])
{
    int count;
    
    pthread_t t0, t1, t2, t3;
    threadData d0,d1, d2,d3;
    int partialSum;
    
    d0.threadNum = 0;
    d1.threadNum = 1;
    d2.threadNum = 2;
    d3.threadNum = 3;
    
    
    /* create threads 1 and 2 */
    pthread_create (&t0, NULL, (void *) &numSubstringPartial, (void *) &d0);
    pthread_create (&t1, NULL, (void *) &numSubstringPartial, (void *) &d1);
    pthread_create (&t2, NULL, (void *) &numSubstringPartial, (void *) &d2);
    pthread_create (&t3, NULL, (void *) &numSubstringPartial, (void *) &d3);
    
    readf(fp);
    count = num_substring();
    
    partialSum = 0;
    
    pthread_join(thread0, NULL);
    partialSum += d0.parCount;
    pthread_join(t1, NULL);
    partialSum += d1.parCount;
    pthread_join(t2, NULL);
    partialSum += d2.parCount;
    pthread_join(t3, NULL);
    partialSum += d3.parCount;
    
    printf("The number of substrings is: %d\n", count);
    printf("the number of substrings counted in parallel is: %d\n",partialSum);
    return 1;
}
