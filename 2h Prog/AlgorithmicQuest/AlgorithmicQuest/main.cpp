#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15
#include <time.h>
#include <string.h>

#define MAX( a, b ) ( ( a > b) ? a : b )
#define MIN( a, b ) ( ( a < b) ? a : b )

char buffer[BSIZE];
char NOF[3], inputfile[100], outputfile[100];
long bpos = 0L, bsize = 0L;
FILE *fp, *fpout; //the input file used for debugging
static long sums[1000][1000][2] = {0};
static int board[1000][1000] = {0};
static int N, M, K;

// the list element node structure.  Add more stuff if you want to.
struct list_el {
	int index;				// ID number
	struct list_el * next;			// link pointer (where the next element is)
};
typedef struct list_el node;

node* maxs[1000] = {NULL}; //GIA TA BONUS PREPEI AN TO KANW MEGALUTERO KAI ISWS DEN EINAI GENIKA SWSTOS TROPOS GIA NA BRISKOUME APO POION THA KANOUME DEVELOP


void addNode(int col, int index) {
	node *curr, *new1;
    node *head = maxs[col];
    long temp1 = sums[index][col][1] + board[index][col];
    long temp0 = sums[index][col][0];
	// Create and setup our new1 link
	new1 = (node *)malloc(sizeof(node));
	if (new1 == NULL) {
		printf("Memory not available to create link. Exiting.\n");
		exit (EXIT_FAILURE);
	}
	new1->index = index;
	// If Head is NULL, the list doesn't yet exist, so we create one.
	if (head == NULL) {
		maxs[col] = new1;
		new1->next = NULL;
		return;
	}

	// does our new1 element go before the first one?
	if ((temp1 > sums[head->index][col][1] + board[head->index][col]) || (temp1 == sums[head->index][col][1] + board[head->index][col] && temp0 < sums[head->index][col][0])) {
		new1->next = head;
		maxs[col] = new1;
		return;
	}

	// if our element goes in the middle, this code will scan through
	// to find out exactly where it belongs.
	curr = head; // start at the begining node.
	while ( (curr->next != NULL) ) {
        if ((temp1 > sums[curr->next->index][col][1] + board[curr->next->index][col]) || (temp1 == sums[curr->next->index][col][1] + board[curr->next->index][col] && temp0 < sums[curr->next->index][col][0])) {
			if(!(temp1 > sums[curr->index][col][1] + board[curr->index][col]) || (temp1 == sums[curr->index][col][1] + board[curr->index][col] && temp0 < sums[curr->index][col][0])) {
				// this inserts the new1 node into the middle if
				// it ID doesn't already exist in the list
				new1->next = curr->next;
				curr->next = new1;
				return;
			}
		}
		curr = curr->next; // move to the next node.
	}

    // else add the new1 element on the end.
    curr->next = new1;
    new1->next = NULL;
    return;

}


void showNodes(int col) {
	node * temp;
	printf("Printing List number: %d :\n", col);
	temp = maxs[col];
	while(temp) {
		printf("(%ld, %ld), i:%d\n",sums[temp->index][col][0], sums[temp->index][col][1] + board[temp->index][col], temp->index);
		temp = temp->next ;
	}
}


void prepare_debug(int index)
{
    itoa(index, NOF,10);
    strcpy (inputfile,"input");
    strcat (inputfile,NOF);
    strcat (inputfile,".txt");
    strcpy (outputfile,"output");
    strcat (outputfile,NOF);
    strcat (outputfile,".txt");
    fp = fopen(inputfile, "r");
    fpout = fopen(outputfile, "r");
    return;
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

void print_matrix(char matrix)
{
    int i , j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(matrix == 'b')
            {
                printf("%d ", board[i][j]);
            }
            else
            {
                printf("(%ld, %ld) ", sums[i][j][0], sums[i][j][1]);
            }
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void print_array()
{
    int i ;
    printf("Ready for dev\n");
    for(i=0;i<N;i++)
    {
        printf("%d ", maxs[i]);
    }
    printf("\n");
    printf("\n");
    return;
}


void read_and_create_matrix(FILE *fp)
{
    int i, j, k, temp;
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            board[i][j] = (int)readLong();
        }
    }
    return;
}

void clear_matrix()
{
    int i, j, k, temp;
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            sums[i][j][0] = 0;
            sums[i][j][1] = 0;
        }
    }
    return;
}



void compute_final()
{
    int i, j, k, best_target,best_enemy, k1, k2, temp;
    long total_iterations = 0;
    //GIA N*M KELIA
    for(i=N-1;i>=0;i--)
    {
        //An eimaste sthn teleutaia grammh des ola ektos tou teleutaiou keliou
        if(i==N-1)
        {
            for(j=M-2;j>=0;j--)
            {
                //Gia kathe keli des se pio keli tha thelame na pame
                best_target = sums[i][j+1][1] + board[i][j+1];
                best_enemy = sums[i][j+1][0];
                for(k=2;k<=MIN(K, M-1-j);k++)
                {
                    temp = sums[i][j+k][1] + board[i][j+k];
                    if((temp > best_target) || (temp == best_target && sums[i][j+k][0] < best_enemy ))
                    {
                        best_target = temp;
                        best_enemy = sums[i][j+k][0];
                    }
                }
                sums[i][j][0] = best_target;
                sums[i][j][1] = best_enemy;
                //print_matrix('s');
            }
        }
        else
        {
            //ELEGXOUME GIA OLA TA KELIA SE POIO KELI AOO TA K^2 EOOMENA MPOROUME NA PAME
            for(j=M-1;j>=0;j--)
            {
                best_target = -1;
                best_enemy = -1;
                for(k2=0;k2<=MIN(K, M-1-j);k2++)
                {
                    for(k1=0;k1<=MIN(K, N-1-i);k1++)
                    {
                        total_iterations ++;
                        if(k1 > 0 || k2 > 0)
                        {
                            //printf("MPHKA\n");
                            temp = sums[i+k1][j+k2][1] + board[i+k1][j+k2];
                            if((temp > best_target) || (temp == best_target && sums[i+k1][j+k2][0] < best_enemy ))
                            {
                                best_target = temp;
                                best_enemy = sums[i+k1][j+k2][0];
                            }
                        }

                    }

                }
                sums[i][j][0] = best_target;
                sums[i][j][1] = best_enemy;
                //print_matrix('s');
            }
        }

    }
    printf("Total iterations:%ld\n", total_iterations);
    return;
}


void alternative_compute_final()
{
    int i, j, k, k1, k2, temp,temp1,temp2, temp3, bathos;
    int flag =1;
    long best_enemy, best_target, prev_best_index, total_iterations;
    node * curr, * prev;
    total_iterations = 0;
    prev_best_index = -1;
    //GIA N*M KELIA
    for(i=N-1;i>=0;i--)
    {
        //printf("-------------------New line-----------\n");
        //An eimaste sthn teleutaia grammh des ola ektos tou teleutaiou keliou
        if(i==N-1)
        {
            for(j=M-2;j>=0;j--)
            {
                //Gia kathe keli des se pio keli tha thelame na pame
                best_target = sums[i][j+1][1] + board[i][j+1];
                best_enemy = sums[i][j+1][0];
                if(maxs[j+1] == NULL)
                {
                    addNode(j+1, i);
                }
                for(k=2;k<=MIN(K, M-1-j);k++)
                {
                    temp = sums[i][j+k][1] + board[i][j+k];

                    if((temp > best_target) || (temp == best_target && sums[i][j+k][0] < best_enemy ))
                    {
                        best_target = temp;
                        best_enemy = sums[i][j+k][0];
                    }
                }
                sums[i][j][0] = best_target;
                sums[i][j][1] = best_enemy;
                //print_matrix('s');
            }
            addNode(0,i);
            //showNodes(0);
        }

        else
        {
            //ELEGXOUME GIA OLA TA KELIA SE POIO KELI AOO TA K^2 EOOMENA MPOROUME NA PAME
            for(j=M-1;j>=0;j--)
            {
                //printf("New cell-----------\n");
                best_target = -1;
                best_enemy = -1;
                //printf("PREVIOUS BEST INDEX: %d, CURRENT INDEX: %d\n", prev_best_index, j);
                //ASDJIGHAJLSBDBHKJLVHKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKEDW THELEI FTIAXIMO
                //printf("PREV_INDEX: %d, j: %d, j+K: %d, i+K: %d\n", prev_best_index, j, j+K, i+K);
                if(prev_best_index <= j+K && prev_best_index > j && maxs[prev_best_index]->index <= i + K)
                {

                    temp = sums[maxs[prev_best_index]->index][prev_best_index][1] + board[maxs[prev_best_index]->index][prev_best_index];
                    temp1 = sums[maxs[prev_best_index]->index][prev_best_index][0];
                    if(j<M-1)
                    {
                        if(temp > sums[i][j+1][1] + board[i][j+1] || (temp == sums[i][j+1][1] + board[i][j+1] && temp1 < sums[i][j+1][0]))
                        {
                            best_target = temp;
                            best_enemy = temp1;
                            //printf("I DID IT AGAIN\n");
                        }
                        else
                        {
                            best_target = sums[i][j+1][1] + board[i][j+1];
                            best_enemy = sums[i][j+1][0];
                            prev_best_index = j+1;
                            //printf("THE GUY ON THE RIGHT DID IT AGAIN\n");
                        }
                    }
                    else
                    {
                        best_target = temp;
                        best_enemy = temp1;
                        //printf("I DID IT AGAIN\n");
                    }
                    temp1 = best_target;
                    temp2 = best_enemy;
                    temp3 = prev_best_index;




                    //This is not the correct way of coding but i didnt have time----
                    prev = maxs[j+0];
                    flag = 1;
                    //printf("Now checking cell: %d %d\n", prev->index, j+0);
                    if(prev->next == NULL)
                    {
                        temp = sums[prev->index][j+0][1] + board[prev->index][j+0];
                        if((temp > best_target) || (temp == best_target && sums[prev->index][j+0][0] < best_enemy ))
                        {
                            best_target = temp;
                            best_enemy = sums[prev->index][j+0][0];
                            prev_best_index = j;
                            //printf("THIS IS THE BEST TARGET: %ld, and the best enemy: %ld\n", best_target, best_enemy);
                        }

                    }
                    else
                    {
                        temp = sums[prev->index][j+0][1] + board[prev->index][j+0];
                        if(prev->index <= i + K)
                        {
                            flag = 0;
                            //printf("IF TEMP: %ld > %ld\n", temp, best_target);
                            if((temp > best_target) || (temp == best_target && sums[prev->index][j+0][0] < best_enemy ))
                            {
                                best_target = temp;
                                best_enemy = sums[prev->index][j+0][0];
                                prev_best_index = j;
                                //printf("THIS IS THE BEST TARGET: %ld, and the best enemy: %ld\n", best_target, best_enemy);
                            }

                        }
                        //showNodes(j+0);
                        while(prev->next != NULL && flag == 1)
                        {
                            //printf("WHY AM I STILL HERE??????????????\n");
                            total_iterations ++;
                            curr = prev->next;
                            if(curr->index <= i + K)
                            {
                                //printf("Now checking cell: %d %d\n", curr->index, j+0);
                                temp = sums[curr->index][j+0][1] + board[curr->index][j+0];
                                //printf("IF TEMP: %ld > %ld\n", temp, best_target);
                                if((temp > best_target) || (temp == best_target && sums[curr->index][j+0][0] < best_enemy ))
                                {
                                    best_target = temp;
                                    best_enemy = sums[curr->index][j+0][0];
                                    prev_best_index = j;
                                    //printf("THIS IS THE BEST TARGET: %ld, and the best enemy: %ld\n", best_target, best_enemy);
                                }
                                flag = 0;
                                break;
                            }
                            prev = prev->next;
                            if(prev == NULL)
                            {
                                break;
                            }
                        }
                    }

                    //printf("IF THE PREV_BEST > BEST OF COL: %d > %d\n", temp1, best_target);
                    //printf("IF PREV_BEST_ENEMY < BEST OF COL ENEMY: %d < %d\n\n\n", temp2, best_enemy);
                    if(best_target < temp1 || (best_target == temp1 && best_enemy > temp2))
                    {
                        best_target = temp1;
                        best_enemy = temp2;
                        prev_best_index = temp3;
                        //printf("I DID IT FOR SURE\n");
                        //printf("I DID IT FOR SURE-----------------------------------------------------------------------------------------------------\n");
                    }





                }
                else    //AN TO PROHGOUMENO MAX DEN EINAI MESA STA ORIA TOTE BRES TO KAINOURGIO MAX APO THN ARXH
                {
                    for(k2=0;k2<=MIN(K, M-1-j);k2++)
                    {
                        prev = maxs[j+k2];
                        flag = 1;
                        //printf("Now checking cell: %d %d\n", prev->index, j+k2);
                        if(prev->next == NULL)
                        {
                            temp = sums[prev->index][j+k2][1] + board[prev->index][j+k2];
                            if((temp > best_target) || (temp == best_target && sums[prev->index][j+k2][0] < best_enemy ))
                            {
                                best_target = temp;
                                best_enemy = sums[prev->index][j+k2][0];
                                prev_best_index = j+k2;
                                //printf("THIS IS THE BEST TARGET: %ld, and the best enemy: %ld\n", best_target, best_enemy);
                            }

                        }
                        else
                        {
                            temp = sums[prev->index][j+k2][1] + board[prev->index][j+k2];
                            if(prev->index <= i + K)
                            {
                                flag = 0;
                                //printf("IF TEMP: %ld > %ld\n", temp, best_target);
                                if((temp > best_target) || (temp == best_target && sums[prev->index][j+k2][0] < best_enemy ))
                                {
                                    best_target = temp;
                                    best_enemy = sums[prev->index][j+k2][0];
                                    prev_best_index = j+k2;
                                    //printf("THIS IS THE BEST TARGET: %ld, and the best enemy: %ld\n", best_target, best_enemy);
                                }

                            }
                            //showNodes(j+k2);
                            while(prev->next != NULL && flag == 1)
                            {
                                //printf("WHY AM I STILL HERE??????????????\n");
                                total_iterations ++;
                                curr = prev->next;
                                if(curr->index <= i + K)
                                {
                                    //printf("Now checking cell: %d %d\n", curr->index, j+k2);
                                    temp = sums[curr->index][j+k2][1] + board[curr->index][j+k2];
                                    //printf("IF TEMP: %ld > %ld\n", temp, best_target);
                                    if((temp > best_target) || (temp == best_target && sums[curr->index][j+k2][0] < best_enemy ))
                                    {
                                        best_target = temp;
                                        best_enemy = sums[curr->index][j+k2][0];
                                        prev_best_index = j+k2;
                                        //printf("THIS IS THE BEST TARGET: %ld, and the best enemy: %ld\n", best_target, best_enemy);
                                    }
                                    flag = 0;
                                    break;
                                }
                                prev = prev->next;
                                if(prev == NULL)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
                //------------------------------------------------------------------------------------------------------------------------------------------


                sums[i][j][0] = best_target;
                sums[i][j][1] = best_enemy;
                addNode(j, i);
                //print_matrix('s');

            }
            for(j=0;j<M;j++)
            {
                //showNodes(j);
            }
        }

    }
    //printf("Total Iterations are: %ld\n", total_iterations);
    return;
}


int main()
{
    int flag_of_debug = 0;
    int i, j, k, temp;
    long temp1, temp2;
    int num_of_correct, index, index_s, index_e;
    num_of_correct = 0;
    index_s = 1;
    index_e = 21;
    if(flag_of_debug == 0)
    {
        for(index = index_s; index<=index_e;index++)
        {
            //------------------CLOCK START------------------------
            clock_t start, end, checkpoint;
            double cpu_time_used;
            start = clock();
            checkpoint = start;
            // -----------------------------------------------------

            prepare_debug(index);
            fscanf(fp, "%d %d %d\n", &N, &M, &K);
            printf("N = %d and M = %d and K = %d\n", N,M, K);
            read_and_create_matrix(fp);

            //-----------CLOCK ENDING ----------------------------------
            end = clock();
            cpu_time_used = ((double) (end - checkpoint)) / CLOCKS_PER_SEC;
            checkpoint = end;
            //----------------------------------------------------------
            printf("Reading time elapsed: %f\n", cpu_time_used); // Debug

            //print_matrix('b');
            ////////////////////////////////////////////////////////////////////////////////


            for(i=0;i<M;i++)
            {
                maxs[i] = NULL;
            }
            //compute_final();
            alternative_compute_final();
            for(j=0;j<M;j++)
            {
                //showNodes(j);
            }
            //print_matrix('s');
            //printf("%d %d\n", sums[0][0][0], sums[0][0][1]);


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




            fscanf(fpout, "%ld %ld", &temp1, &temp2);
            printf("My answer is: %ld, %ld, and the correct answer is: %ld, %ld\n", sums[0][0][0], sums[0][0][1], temp1, temp2);
            if(sums[0][0][0] == temp1 && sums[0][0][1] == temp2)
            {
                printf("\n\n------  YOU DID IT PAL  ------\n\n");
                num_of_correct++;
            }
            else
            {
                printf("\n\n-----THIS IS WRONG THIS IS WRONG-----\n\nTestcase: %d\n\n", index);
            }
            clear_matrix();
        }
        printf("Number of correct: %d out of: %d", num_of_correct, index_e - index_s + 1);
    }
    else
    {

        fp = stdin;
        fscanf(fp, "%d %d %d\n", &N, &M, &K);
        read_and_create_matrix(fp);
        for(i=0;i<M;i++)
        {
            maxs[i] = NULL;
        }
        alternative_compute_final();
        printf("%ld %ld\n", sums[0][0][0], sums[0][0][1]);
        //code_goes_here

    }
    return 0;
}




