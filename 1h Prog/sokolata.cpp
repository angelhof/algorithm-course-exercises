#include <stdio.h>
#define BSIZE 1<<15
#include <time.h>


char buffer[BSIZE];
long bpos = 0L, bsize = 0L;
FILE *fp; //the input file used for debugging


long readLong() //This function reads longs
{
	long d = 0L, x = 0L;
	char c;

	while (1)  {
		if (bpos >= bsize) {
			bpos = 0;
			if (feof(stdin)) return x;
			bsize = fread(buffer, 1, BSIZE, fp);
		}
		c = buffer[bpos++];
		if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
		else if (d == 1) return x;
	}
	return -1;
}

void printLongArray(long *a, long len) // TODO Make print array function : DOES IT NEED LENGTH?
{
    long i;
    for(i = 0; i < len; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    return;
}

int main()
{

    printf("Start\n");
    fp = fopen("input2.txt", "r");
    //------------------CLOCK START------------------------
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    // -----------------------------------------------------
    long N = readLong();
    long K = readLong(); // ---SKEPSEIS---: Paizei na xreiastei na balw long long
    printf("Debug N = %d, K = %d\n", N , K); //Debug

    long i, j, array_len;
    array_len = 0; // Keep the length of the array in a long and initialize
    static long chocolate_sum[1000000] = {0}; // ---SKEPSEIS---: Den einai to swsto megethos, epishs consider to STATIC
    for(i = 0;i < N; i++)
    {
        long si = readLong();
        long fi = readLong();
        if(fi > array_len){ array_len = fi; }
        printf("Debug si = %d, fi = %d\n", si , fi); //Debug
        for(j = si - 1; j < fi; j ++)   // Apo to Si mexri to Fi prosthetei ena ston pinaka
        {
            chocolate_sum[j]++;
        }
    }
    fclose (fp); // Debug reading from a file -----------------------
    //printLongArray(chocolate_sum, array_len); // Debug print the whole array, not very useful

    long sum = 0;
    i = 0;
    while(sum + chocolate_sum[i] < K && i < array_len)
    {
        sum += chocolate_sum[i++];
    }
    sum += chocolate_sum[i++];
    //-----------CLOCK ENDING -------------------------------
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("The sum is: %d, and the waiting time is: %d.\nTotal time elapsed: %f", sum, i, cpu_time_used); // Debug
    return 0;
}
