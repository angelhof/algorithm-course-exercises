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

int cmpfunc(const void * a, const void * b)
{
    return ( *(const long *)a - *(const long *)b );// ERORRRRRRRRR--------------------------------------------------------------------
}


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


void printLongArray(long a[100000][2], long len) // TODO Make print array function : DOES IT NEED LENGTH?
{
    long i;
    for(i = 0; i < len; i++)
    {
        printf("%lld %lld\n", a[i][0] , a[i][1]);
    }
    return;
}

int main()
{
    int flag_of_debug = 1;
    if(flag_of_debug == 0)
    {
        fp = stdin;
        long long N = readLong();
        long long K = readLong();

        long long i, j, array_len, array_start, waiting_time;
        array_len = 0; // Keep the length of the array in a long and initialize
        array_start = 100000000;
        static long chocolate_sum[100000][2] = {0};
        for(i = 0;i < N; i++)
        {
            long si = readLong();
            long fi = readLong();
            if(fi > array_len){ array_len = fi; }
            if(si < array_start){ array_start = si; }
            chocolate_sum[i][0] = si;
            chocolate_sum[i][1] = fi;
        }

        qsort(chocolate_sum, N, sizeof(chocolate_sum[0]), cmpfunc);


        long long start_bin, end_bin, median, old_small_med, old_big_med, index, sum;
        start_bin = array_start;
        end_bin = array_len;
        median = (start_bin + end_bin)/2;
        waiting_time = -1;
        old_small_med = median;
        old_big_med = median;
        //Just check if the whole array is not enough
        sum = 0;
        for(i=0;i<N;i++){
            sum += chocolate_sum[i][1] - chocolate_sum[i][0] + 1;
        }
        if(sum <= K){
            waiting_time = end_bin;
        }
        while(waiting_time == -1 and end_bin > start_bin)
        {
            sum = 0;
            for(i=0;i<N;i++){
                sum += MAX(0, MIN(median, chocolate_sum[i][1]) - chocolate_sum[i][0] + 1);
                if(chocolate_sum[i][0] > median){
                    break;
                }
            }
            if(sum == K){
                waiting_time = median;
                break;
            }
            if(sum>K){
                old_big_med = median;
                end_bin = median;
                if(old_small_med == median - 1){
                    waiting_time = median;
                    break;
                }
            }
            else if(sum<K){
                old_small_med = median;
                start_bin = median;
                if(old_big_med == median + 1){
                    waiting_time = old_big_med;
                    break;
                }
            }
            median = (start_bin + end_bin)/2;
        }

        printf("%lld\n", waiting_time);
    }
    else
    {
        int index_of_case;
        int sum_of_correct = 0;
        char file_name[100];
        char number_of_file[3];
        // This checks for the next 10 how many are correct
        for(index_of_case=1;index_of_case<10;index_of_case++){
            file_name[0] = '\0';
            number_of_file[0] = (char)(index_of_case + 48);
            number_of_file[1] = '\0';
            strcat(file_name, "input");
            strcat(file_name, number_of_file);
            strcat(file_name, ".txt");
            fp = fopen(file_name, "r"); // Debug, kanonika tha trexei apo stdin
            file_name[0] = '\0';
            strcat(file_name, "output");
            strcat(file_name, number_of_file);
            strcat(file_name, ".txt");
            fpout = fopen(file_name, "r"); // Debug, kanonika tha trexei apo stdin
            printf("Start\n");


            //------------------CLOCK START------------------------
            clock_t start, end;
            double cpu_time_used;
            start = clock();
            // -----------------------------------------------------
            long long N = readLong();
            long long K = readLong(); // ---SKEPSEIS---: Paizei na xreiastei na balw long long
        //    printf("Debug N = %lld, K = %lld\n", N , K); //Debug

            long long i, j, array_len, array_start, waiting_time;
            array_len = 0; // Keep the length of the array in a long and initialize
            array_start = 100000000;
            static long chocolate_sum[100000][2] = {0}; // ---SKEPSEIS---: Den einai to swsto megethos, epishs consider to STATIC
            for(i = 0;i < N; i++)
            {
                long si = readLong();
                long fi = readLong();
                if(fi > array_len){ array_len = fi; }
                if(si < array_start){ array_start = si; }
                //printf("Debug si = %lld, fi = %lld\n", si , fi); //Debug
                chocolate_sum[i][0] = si;
                chocolate_sum[i][1] = fi;
            }
        //    printf("%lld %lld\n", array_start, array_len); //debug
        //    printLongArray(chocolate_sum, N);  //debug
            fclose (fp); // Debug reading from a file -----------------------
            //-----------CLOCK ENDING ----------------------------------
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            //----------------------------------------------------------
            printf("Total time elapsed: %f\n", cpu_time_used); // Debug

            qsort(chocolate_sum, N, sizeof(chocolate_sum[0]), cmpfunc);

            //printLongArray(chocolate_sum, N); // Debug print the whole array, not very useful
            //----------------------------------------------------------------------



            long long start_bin, end_bin, median, old_small_med, old_big_med, index, sum;
            start_bin = array_start;
            end_bin = array_len;
            median = (start_bin + end_bin)/2;
            waiting_time = -1;
            old_small_med = median;
            old_big_med = median;
            //Just check if the whole array is not enough
            sum = 0;
            for(i=0;i<N;i++){
                sum += chocolate_sum[i][1] - chocolate_sum[i][0] + 1;
            }
            if(sum <= K){
                waiting_time = end_bin;
            }
            while(waiting_time == -1 and end_bin > start_bin)
            {
        //        printf("%lld\n", median); //DEBUG
                sum = 0;
                for(i=0;i<N;i++){
                    sum += MAX(0, MIN(median, chocolate_sum[i][1]) - chocolate_sum[i][0] + 1);
                    //printf("%lld ", sum); //Debug
                    if(chocolate_sum[i][0] > median){
                        break;
                    }
                }
        //        printf("%lld %lld\n", sum, K); //DEBUG
        //        getchar(); //DEBUG
                if(sum == K){
                    waiting_time = median;
                    break;
                }
                if(sum>K){
                    old_big_med = median;
                    end_bin = median;
                    if(old_small_med == median - 1){
                        waiting_time = median;
                        break;
                    }
                }
                else if(sum<K){
                    old_small_med = median;
                    start_bin = median;
                    if(old_big_med == median + 1){
                        waiting_time = old_big_med;
                        break;
                    }
                }
                median = (start_bin + end_bin)/2;
            }

            //------------------------------------------------------------------------------------------------------------------------


            //-----------CLOCK ENDING ----------------------------------
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            //----------------------------------------------------------
            printf("The waiting time is: %lld.\nTotal time elapsed: %f\n", waiting_time, cpu_time_used); // Debug
            fp = fpout;
            long long expected_wait = readLong();
            printf("The expected correct time is: %lld.\n", expected_wait);
            printf("%lld\n", waiting_time);
            if(expected_wait == waiting_time){
                sum_of_correct ++;
            }

        }
        printf("%d\n", sum_of_correct);
    }
    return 0;
}
