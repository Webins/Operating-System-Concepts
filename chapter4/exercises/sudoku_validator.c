#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int sud[9][9]=
{
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0}
};

void initialize_sudoku(){
    int i, j;
    srand(time(NULL));
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            sud[i][j] = rand() % 10;
        }
    }
}

void display_sudoku(){
int i, j;
    for(i = 0; i < 9; i++){
    	printf("|");
        for(j = 0; j < 9; j++){
            printf("%d|", sud[i][j]);
        }
        printf("\n");
    }
}

struct data
{
    int row;
    int col;
};

/*Threads for Checking row and column*/
void *row_col(void *d)
{
    int n,m,h=0;
    m=(int)d;
    int k,i,j;

    for(i=0; i<9; i++)
    {
        k=1;
        while(k<10)
        {
            for(j=0; j<9; j++)
            {
                if(m==0 && sud[i][j]==k)
                {
                    h++;
                    goto aam;
                }
                else if(m==1 && sud[j][i]==k)
                {
                    h++;
                    goto aam;
                }
            }
aam:
            k++;
        }
    }
    if(h==81)
    {
        n=1;
    }
    else
    {
        n=-1;
    }
    pthread_exit((void*)n);
}
/*Threads for Checking each  square*/

void *sq_check(void *mn)
{
    struct data *my_data=(struct data *)mn;
    int i=(*my_data).row;
    int j=(*my_data).col;
    int n,m,p;
    n=i+3;
    m=j+3;
    int k,h=0;
    for(k=1; k<10; k++)
    {
        for(i=0; i<n; i++)
        {
            for(j=0; j<m; j++)
            {
                if(sud[i][j]==k)
                {
                    h++;
                    i=n;
                    j=m;
                }
            }
        }

    }
    if(h==9)
    {
        p=1;
    }
    else
    {
        p=-1;
    }
    pthread_exit((void*)p);
}
int main()
{
    initialize_sudoku();
    display_sudoku();
    struct data *p;
    p=(struct data*)malloc(sizeof(struct data));
    pthread_t thread[11];
    int i,a;
    void *b;
    int t,l,k;
    for(i=0; i<2; i++)
    {
        t=i;
        /* Creation of Threads for Checking row and column */
        a=pthread_create(&thread[i],NULL,row_col,(void*)t);
        if(a)
        {
            printf("error");
        }
    }


    /* Creation of Threads for Checking 9 squares */
    for(l=0; l<=6; l=l+3)
    {
        for(k=0; k<=6; k=k+3)
        {
            (*p).row=l;
            (*p).col=k;

            a=pthread_create(&thread[i++],NULL,sq_check,(void*)p);
            if(a)
            {
                printf("error");
            }
        }
    }

    int s=0;
    for(i=0; i<11; i++)
    {
        pthread_join(thread[i],&b);
        s=s+(int)b;
    }
    if(s==11)
    {
        printf("THE GIVEN SUDOKU IS VALID\n");
    }
    else
    {
        printf("THE GIVEN SUDOKU IS INVALID\n");
    }
    pthread_exit(NULL);
}
