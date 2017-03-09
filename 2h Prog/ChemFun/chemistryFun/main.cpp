#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15
#include <time.h>
#include <string.h>

#define MAX( a, b ) ( ( a > b) ? a : b )
#define MIN( a, b ) ( ( a < b) ? a : b )

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;
FILE *fp, *fpout; //the input file used for debugging
static int sums[2500][2500] = {0};
static int N, K;
int curr[2500] = {0};
int prev[2500] = {0};

long long readLong() //This function reads longs
{
	long long d = 0L, x = 0L;
	char c;

	while (1)  {
		if (bpos >= bsize) {
			bpos = 0;
			if (feof(fp)) return x;
			bsize = fread(buffer, 1, BSIZE, fp); // Kanonika tha trexei apo stdin
		}
		c = buffer[bpos++];
		if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
		else if (d == 1) return x;
	}
	return -1;
}

void print_matrix()
{
    int i , j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            printf("%d ", sums[i][j]);
        }
        printf("\n");
    }
    return;
}

void print_arrays()
{
    int i ;
    printf("Current Array\n");
    for(i=0;i<N;i++)
    {
        printf("%d ", curr[i]);
    }
    printf("\n");
    printf("Previous Array\n");
    for(i=0;i<N;i++)
    {
        printf("%d ", prev[i]);
    }
    printf("\n");

    return;
}



//This function reads and creates the sums matrix
void read_and_create_matrix(FILE *fp)
{
    int i, j, k, temp;
    for(i=0;i<N-1;i++)
    {
        for(j=i+1;j<N;j++)
        {
            sums[i][j] = (int)readLong();
        }
    }
    for(i=N-2;i>0;i--)
    {
        for(j=i;j<N;j++)
        {
            sums[i-1][j] += sums[i][j];
        }
    }

    for(j=1;j<N-1;j++)
    {
        for(i=0;i<j;i++)
        {
            sums[i][j+1] += sums[i][j];
        }
    }
    return;
}

int* move_curr_to_prev()
{
    int i, index[2];
    index[0] = 0;
    index[1] = -1;
    for(i=0;i<N;i++)
    {
        prev[i] = curr[i];
        //the first one that is more than zero
        if(prev[i] > 0 and i > 0 and index[0] == 0)
        {
            index[0] = i-1;
        }
        if(i > 0)
        {
            if(prev[i] - prev[i-1] > index[1])
            {
                index[1] = prev[i] - prev[i-1];
            }
        }
    }
    return index;
}



void recursion_fun(int start, int end, int bottle, int lguard, int rguard)
{
    int start_j, end_j, i, j , temp, next_guard;
    int mid = (start + end) / 2;
    next_guard = -1;
    if(start >= end )
    {
        return;
    }
    //printf("Start: %d, End: %d, Mid: %d, Bottle: %d, Left guard: %d, Right guard: %d\n", start, end, mid, bottle, lguard, rguard);
    if(lguard > 0)
    {
        curr[mid] = prev[lguard - 1] + sums[lguard][mid];
    }
    else
    {
        curr[mid] = prev[lguard] + sums[lguard+1][mid];
    }
    //printf("Curr[%d] = prev[%d] + sums[%d][%d] -> %d = %d + %d\n", mid, lguard, lguard + 1, mid, curr[mid], prev[lguard], sums[lguard+1][mid]);
    for(j=lguard + 1;j<=MIN(rguard, mid);j++)
    {
        //printf("if Curr[%d] > prev[%d] + sums[%d][%d] -> %d > %d + %d\n", mid, j-1, j, mid, curr[mid], prev[j-1], sums[j][mid]);
        temp = prev[j-1] + sums[j][mid];
        if(temp < curr[mid])
        {
            next_guard = j;
            curr[mid] = temp;
        }
    }
    //printf("NEXT GUARD: %d\n", next_guard);
    //printf("\n\n");
    //LEIPEI O TERMATISMOS TO PROBLHMA DEN TERMATIZEI KAI DEN XERW KAI POSO DOULEUEI
    if(next_guard == -1)
    {
        recursion_fun(start, mid, bottle, lguard, rguard);
        recursion_fun(mid+1, end, bottle, lguard, rguard);
    }
    else
    {
        recursion_fun(start, mid, bottle, lguard, next_guard);
        recursion_fun(mid+1, end, bottle, next_guard, rguard);
    }
}

void alternative_compute_final()
{
    int i, j, k, temp, prev_min, start_j;
    int* index;
    for(k=1;k<K;k++)
    {
        //printf("Iteration is: %d\n", k);
        //print_arrays();
        index = move_curr_to_prev();
        recursion_fun(k,N-K+k+1, k, 0, N-1);
    }
    //print_arrays();
    //printf("FUCKING -----  %d -----\n", curr[N-1]);
}


void compute_final()
{
    int i, j, k, temp, prev_min, start_j;
    int* index;
    for(k=1;k<K;k++)
    {
        printf("Iteration is: %d\n", k);
        index = move_curr_to_prev();
        //printf("Max step of previous: %d\n", index[1]);
        prev_min = -1;
        for(i=k;i<N-K+k+1;i++)
        {

            printf("We are filling the %dth cell\n", i);
            curr[i] = sums[k][i] + prev[i-1];
            start_j = 1;
            if(prev_min >= 0)
            {
                start_j = prev_min;
            }
            for(j=start_j;j<i;j++)
            //for(j=i-1;j>0;j--)
            {
                temp = sums[j+1][i] + prev[j];
                if(temp > curr[i] + index[1])
                {
                    break;
                }
                printf("We are checking if A[%d][%d] + prev[%d] < curr[%d]\n", j + 1, i,j, i);
                printf("-- -- %d + %d < %d -- --\n", sums[j+1][i],prev[j], curr[i]);
                if(temp < curr[i])
                {
                    curr[i] = temp;
                    prev_min = j;
                }
                //printf("Temp: %d > curr[i]: %d + index[1]: %d\n", temp, curr[i], index[1]);

            }


        }
    }
}

int main()
{
    int flag_of_debug = 0;
    int i, j, k, temp;
    int num_of_correct, index;
    num_of_correct = 0;
    char NOF[3], inputfile[100], outputfile[100];
    if(flag_of_debug == 0)
    {
        for(index = 1; index<15;index++)
        {
            //------------------CLOCK START------------------------
            clock_t start, end, checkpoint;
            double cpu_time_used;
            start = clock();
            checkpoint = start;
            // -----------------------------------------------------
            itoa(index, NOF,10);
            strcpy (inputfile,"input");
            strcat (inputfile,NOF);
            strcat (inputfile,".txt");
            strcpy (outputfile,"output");
            strcat (outputfile,NOF);
            strcat (outputfile,".txt");
            fp = fopen(inputfile, "r");
            fscanf(fp, "%d %d\n", &N, &K);
            printf("N = %d and K = %d\n", N,K);
            read_and_create_matrix(fp);

            //-----------CLOCK ENDING ----------------------------------
            end = clock();
            cpu_time_used = ((double) (end - checkpoint)) / CLOCKS_PER_SEC;
            checkpoint = end;
            //----------------------------------------------------------
            printf("Reading time elapsed: %f\n", cpu_time_used); // Debug
            //print_matrix(); //Debug Only


            //Fill the first array
            for(i=0;i<N;i++)
            {
                curr[i] = sums[0][i];
            }
            //print_arrays();//Debug Only
            alternative_compute_final();
            //print_arrays();
            //-----------CLOCK ENDING ----------------------------------
            end = clock();
            cpu_time_used = ((double) (end - checkpoint)) / CLOCKS_PER_SEC;
            checkpoint = end;
            //----------------------------------------------------------
            printf("Computational time elapsed: %f\n", cpu_time_used); // Debug

            //-----------CLOCK ENDING ----------------------------------
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            //----------------------------------------------------------
            printf("Total time elapsed: %f\n", cpu_time_used); // Debug



            fpout = fopen(outputfile, "r");
            fscanf(fpout, "%d", &temp);
            printf("My answer is: %d, and the correct answer is: %d\n", curr[N-1], temp);
            if(temp != curr[N-1])
            {
                printf("\n\n-----THIS IS WRONG THIS IS WRONG-----\nTestcase: %d\n\n", index);
            }
            else
            {
                num_of_correct++;
            }
        }
        printf("Number of correct: %d out of: 20", num_of_correct);
    }
    else
    {

        fp = stdin;
        fscanf(fp, "%d %d\n", &N, &K);
        //printf("N = %d and K = %d\n", N,K);
        read_and_create_matrix(fp);


        //Fill the first array
        for(i=0;i<N;i++)
        {
            curr[i] = sums[0][i];
        }
        alternative_compute_final();
        printf("%d\n", curr[N-1]);

    }
    return 0;
}
