#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15
#include <time.h>


FILE *fp, *fout; //the input file used for debugging
static int park_matrix[700][700] = {0};
static int added_parks_matrix[700][700] = {0};

void print_matrix(int N)
{
    int i, j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            putc((char)(park_matrix[i][j] + 48), stdout);
        }
        putc('\n',stdout);
    }
    putc('\n',stdout);
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            putc((char)(added_parks_matrix[i][j] + 48), stdout);
        }
        putc('\n',stdout);
    }
    return;
}
void read_matrix(int N)
{
    int i, j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            park_matrix[i][j] = (int)(fgetc(fp) - 48);
            if(i > 0)
            {
                added_parks_matrix[i][j] = park_matrix[i][j] + added_parks_matrix[i-1][j];
            }
            else
            {
                added_parks_matrix[i][j] = park_matrix[i][j];
            }
        }
        fgetc(fp);
    }
    return;
}

void insert_in_array(int x, int arr[], int K)
{
    int i;
    for(i = 0;i<K-1;i++)
    {
        arr[i] = arr[i+1];
    }
    arr[K-1] = x;
    return;
}

void print_array(int arr[], int K)
{
    int j;
    for(j=0;j<K;j++)
    {
        printf("%d ", arr[j]);
    }
    printf("\n");
}

void init_array(int x, int arr[], int K)
{
    int j;
    for(j=0;j<K;j++)
    {
        arr[j] = x;
    }
}


long long not_so_naive_count_good_parks(int N, int K)
{
    long long total_parks = 0;
    int curr_ind, i, j, k;
    int num_of_cur_0 = 0;
    int index1_longed[7];
    int num_of_0_left_longed[7];
    int matrix_row[700] = {0};
    int hyper_filled[700] = {0};


    for(i=0;i<N;i++)
    {
        init_array(-1, index1_longed, K+1);
        init_array(0, num_of_0_left_longed, K+1);
        num_of_cur_0 = 0;
        init_array(0, hyper_filled, N);
        for(curr_ind=0;curr_ind<N;curr_ind++)
        {

            if(!added_parks_matrix[i][curr_ind])
            {

                if(index1_longed[1] > -1 && index1_longed[0] < index1_longed[1])
                {
                    total_parks += 1 + num_of_0_left_longed[1];
                }
                num_of_cur_0++;
            }
            else
            {

                // This if goes here so that we dont insert on the table if there are more 1s than what we want
                if(added_parks_matrix[i][curr_ind] > K)
                {
                    init_array(curr_ind, index1_longed, K+1);
                    init_array(0, num_of_0_left_longed, K+1);
                    num_of_cur_0 = 0;
                }
                else
                {
                    for(k=0;k<added_parks_matrix[i][curr_ind];k++)
                    {
                        insert_in_array(curr_ind, index1_longed, K + 1);
                        insert_in_array(num_of_cur_0, num_of_0_left_longed,K + 1);
                        num_of_cur_0 = 0;
                    }
                }


                if(index1_longed[1] > -1 && index1_longed[0] < index1_longed[1])
                {
                    total_parks += 1 + num_of_0_left_longed[1];
                }
            }
            //Debugging
//            printf("%d\n", total_parks);
//            print_array(ind1, K);
//            print_array(num_of_0_left, K);
        }

        //Continuing for the rest of the array
        for(j=1;j<=i;j++)
        {
            init_array(-1, index1_longed, K+1);
            init_array(0, num_of_0_left_longed, K+1);
            num_of_cur_0 = 0;

            for(k=0;k<N;k++)
            {
                matrix_row[k] = added_parks_matrix[i][k] - added_parks_matrix[i - j][k];
            }

            for(curr_ind=0;curr_ind<N;curr_ind++)
            {
                // IF THE CURRENT INDEX IS NOT 0 SO THAT WE CAN GO TO THE PREVIOUS HYPERFILLED LOOP
                if(curr_ind > 0)
                {
                    if(!hyper_filled[curr_ind])
                    {
                        if(!matrix_row[curr_ind])
                        {

                            if(index1_longed[1] > -1 && index1_longed[0] < index1_longed[1])
                            {
                                total_parks += 1 + num_of_0_left_longed[1];
                            }
                            num_of_cur_0++;
                        }
                        else
                        {
                            if(matrix_row[curr_ind] > K)
                            {
                                init_array(curr_ind, index1_longed, K+1);
                                init_array(0, num_of_0_left_longed, K+1);
                                hyper_filled[curr_ind] = 1;
                                num_of_cur_0 = 0;
                            }
                            else
                            {
                                for(k=0;k<matrix_row[curr_ind];k++)
                                {
                                    insert_in_array(curr_ind, index1_longed, K + 1);
                                    insert_in_array(num_of_cur_0, num_of_0_left_longed,K + 1);
                                    num_of_cur_0 = 0;
                                }
                            }



                            if(index1_longed[1] > -1 && index1_longed[0] < index1_longed[1])
                            {
                                total_parks += 1 + num_of_0_left_longed[1];
                            }
                        }


                    }
                    else if(hyper_filled[curr_ind] && !hyper_filled[curr_ind - 1])
                    {

                        init_array(-1, index1_longed, K+1);
                        init_array(0, num_of_0_left_longed, K+1);
                        num_of_cur_0 = 0;
                    }
                }
                else
                {
                    if(!hyper_filled[curr_ind])
                    {
                        if(!matrix_row[curr_ind])
                        {

                            if(index1_longed[1] > -1 && index1_longed[0] < index1_longed[1])
                            {
                                total_parks += 1 + num_of_0_left_longed[1];
                            }
                            num_of_cur_0++;
                        }
                        else
                        {
                            if(matrix_row[curr_ind] > K)
                            {
                                init_array(curr_ind, index1_longed, K+1);
                                init_array(0, num_of_0_left_longed, K+1);
                                hyper_filled[curr_ind] = 1;
                                num_of_cur_0 = 0;
                            }
                            else
                            {
                                for(k=0;k<matrix_row[curr_ind];k++)
                                {
                                    insert_in_array(curr_ind, index1_longed, K + 1);
                                    insert_in_array(num_of_cur_0, num_of_0_left_longed,K + 1);
                                    num_of_cur_0 = 0;
                                }
                            }



                            if(index1_longed[1] > -1 && index1_longed[0] < index1_longed[1])
                            {
                                total_parks += 1 + num_of_0_left_longed[1];
                            }
                        }


                    }
                }

            }
            //Debugging
//            printf("Total parks at this point: %d\n", total_parks);
//            printf("Start at row: %d, ends at row: %d\n", i-j+1, i+1);
//            print_array(matrix_row, N);
//            print_array(index1_longed, K + 1);
//            print_array(num_of_0_left_longed, K + 1);
        }



    }
    return total_parks;


}
int main()
{
    int flag_of_debugging = 0;
    int N, K, i, j;
    long long number_of_good_parks = 0;
    long long expected;
    char c; //For reading
    int debug = 1;
    if(!debug)
    {
        fp = stdin;
        fscanf(fp, "%d %d\n", &N, &K);
        read_matrix(N);
        number_of_good_parks = not_so_naive_count_good_parks(N, K);
        printf("%lld\n", number_of_good_parks);
    }
    else
    {
            printf("Start\n");
        fp = fopen("input24.txt", "r"); // Debug, kanonika tha trexei apo stdin
        fout = fopen("output24.txt", "r"); // Debug, kanonika tha trexei apo stdin
        fscanf(fout, "%lld", &expected);
        //------------------CLOCK START------------------------
        clock_t start, end;
        double cpu_time_used;
        start = clock();
        // -----------------------------------------------------

        fscanf(fp, "%d %d\n", &N, &K);
        printf("%d %d\n", N, K);
        read_matrix(N);
        //print_matrix(N);
        number_of_good_parks = not_so_naive_count_good_parks(N, K);



    //-----------CLOCK ENDING ----------------------------------
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        //----------------------------------------------------------
        printf("Total time elapsed: %f \nTotal good parks: %lld, Expected output: %lld\n", cpu_time_used, number_of_good_parks, expected); // Debug
        if(number_of_good_parks == expected){
            printf("----CORRECT----\n");
        }
    }

    return 0;
}
