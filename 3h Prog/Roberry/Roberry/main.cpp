#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15
#include <time.h>
#include <string.h>

#define MAX( a, b ) ( ( a > b) ? a : b )
#define MIN( a, b ) ( ( a < b) ? a : b )

struct adjacent_list_node{
    int cost;
    int node;
    int real_connection[2];
    int already_many;
    struct adjacent_list_node *next;
};

char buffer[BSIZE];
char NOF[3], inputfile[100], outputfile[100];
long bpos = 0L, bsize = 0L;
FILE *fp, *fpout; //the input file used for debugging
static struct adjacent_list_node *adjacent[50000] = {NULL};

static int union_tree[50000] = {0};
static int union_tree_sizes[50000] = {0};

static int connections[500000][3] = {0};

static int nodes_number, nodes[50000], nodes_hash[50000], visited_nodes;

//this is for the second implementation
static int nodes_heights[50000];

static int result[3];

static int N, M, index_international;


int cmpfunc(const void * a, const void * b)
{
    //return ( ((const int *)a)[2] - ((const int *)b)[2]);
    if(((const int *)a)[2] - ((const int *)b)[2] != 0)
    {
        return ( ((const int *)a)[2] - ((const int *)b)[2]);
    }
    else
    {
        return ((const int *)b)[1] - ((const int *)a)[1] ;
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


void print_adjacency_list(int i)
{
    struct adjacent_list_node *curr = adjacent[i];
    printf("\n--- Node: %d ---\n", i);
    while(curr != NULL)
    {
        printf("%d with cost: %d\n", curr->node, curr->cost);
        curr = curr->next;
    }
    printf("\n");
    return;
}

void insert_adj(int x, int y, int cost, int real1, int real2)
{

    int tempo_cost = 1;
    struct adjacent_list_node **curr, *temp;
    curr = &adjacent[x];
    if((*curr) != NULL)
    {
        //An o twrinos kombos einai megaluteros apo ton temp bale ton temp sthn arxh


        while((*curr)->next != NULL)
        {

            if((*curr)->node > y)
            {
                temp = (adjacent_list_node*)malloc(sizeof(struct adjacent_list_node));
                temp->cost = 1;
                temp->node = y;
                temp->real_connection[0] = real1;
                temp->real_connection[1] = real2;
                temp->already_many = 0;
                temp->next = (*curr);
                (*curr) = temp;
                return;
            }
            else if((*curr)->node == y)
            {
                result[1]++;
                if((*curr)->already_many == 0)
                {
                    (*curr)->already_many = 1;
                    (*curr)->cost = 0;
                }
                return;
            }
            curr = &(*curr)->next;
        }

        if((*curr)->node > y)
        {
            temp = (adjacent_list_node*)malloc(sizeof(struct adjacent_list_node));
            temp->cost = 1;
            temp->node = y;
            temp->real_connection[0] = real1;
            temp->real_connection[1] = real2;
            temp->already_many = 0;
            temp->next = (*curr);
            (*curr) = temp;
            return;
        }
        else if((*curr)->node == y)
        {
            result[1]++;
            if((*curr)->already_many == 0)
            {
                (*curr)->already_many = 1;
                (*curr)->cost = 0;
            }
            return;
        }

        temp = (adjacent_list_node*)malloc(sizeof(struct adjacent_list_node));
        temp->cost = 1;
        temp->node = y;
        temp->real_connection[0] = real1;
        temp->real_connection[1] = real2;
        temp->already_many = 0;
        temp->next = NULL;
        //temp->next = (*curr)->next;
        (*curr)->next = temp;

    }
    else
    {
        temp = (adjacent_list_node*)malloc(sizeof(struct adjacent_list_node));
        temp->cost = 1;
        temp->node = y;
        temp->real_connection[0] = real1;
        temp->real_connection[1] = real2;
        temp->already_many = 0;
        temp->next = NULL;
        (*curr) = temp;
    }

    return;
}
int compress_union_tree(int x)
{
    if(x != union_tree[x])
    {
        union_tree[x] = compress_union_tree(union_tree[x]);
    }
    return union_tree[x];
}

int tree_find(int index)
{
    //compress_union_tree(index);
    while(index != union_tree[index]){ index = union_tree[index];}
    return index;
}

int tree_union(int x, int y)
{

    if(x == y){return x;}
    if(union_tree_sizes[x] >= union_tree_sizes[y])
    {
        union_tree[y] = x;
        union_tree_sizes[x] += union_tree_sizes[y];
        return x;
    }else
    {
        union_tree[x] = y;
        union_tree_sizes[y] += union_tree_sizes[x];
        return y;
    }



}


void read_data()
{
    int i;
    N = readLong();
    M = readLong();
    //printf("%d %d %d\n", N, K, M);
    for(i=0;i<N;i++){
        union_tree[i] = i;
        union_tree_sizes[i] = 1;
    }
    for(i=0;i<M;i++){
        connections[i][0] = readLong() - 1;
        connections[i][1] = readLong() - 1;
        connections[i][2] = readLong();
    }

    qsort(connections, M, sizeof(connections[0]), cmpfunc);
    return;
}




int dfs_base2(int cost, int current_node, int father, int real_conn[], int height)
{
    int back_to_the_past = height;
    int temp_back_to_the_past;
    nodes_hash[current_node] = 2;
    nodes_heights[current_node] = height;
    visited_nodes++;
    struct adjacent_list_node * temp = adjacent[current_node];
    int temp_temp;
    while(temp != NULL)
    {
        //printf("%d %d %d\n", result[0], result[2], result[1]);
        if(temp->node != father)
        {
            if(nodes_hash[temp->node] == 1)
            {

                //printf("We add %d to the result1 because of %d -> %d\n",temp->cost, current_node, temp->node);
                temp_back_to_the_past = dfs_base2(temp->cost, temp->node, current_node, temp->real_connection, height+1);
                if(temp_back_to_the_past > height)
                {
                    //printf("We are at node: %d, with height: %d and our son: %d just finished with a back to the past: %d\n", current_node, height, temp->node, temp_back_to_the_past);
                    //printf("We add %d because back_to_the_past > height -> %d > %d\n", temp->cost, temp_back_to_the_past, height);
                    result[0] += temp->cost;
                }
                if(temp_back_to_the_past<back_to_the_past)
                {
                    back_to_the_past = temp_back_to_the_past;
                }

            }
            else if(nodes_hash[temp->node] == 2 )
            {
                if(nodes_heights[temp->node] < back_to_the_past)
                {
                    back_to_the_past = nodes_heights[temp->node];
                }


            }
            else if(nodes_hash[temp->node] == 3)
            {
                //nodes_hash[temp->node] = 4;
                //result[0]--;
                //printf("We cannot add anything here because the one down is fully explored\n");
            }
            else
            {
                printf("HOUSTON WE GOT A BIG PROBLEM :O!!!\n");
                exit(1);
            }
        }

        temp = temp->next;
    }
    nodes_hash[current_node] = 3;
    return back_to_the_past;
}

void execute_dfs2()
{
    //nodes_hash means 0 if not in graph , 1 if in graph but not visited, 2 if visited but not full , 3 if full
    int i, j;
    int real_conn[2] = {-1, -1};
    j = 0;

    while(visited_nodes < nodes_number)
    {
        for(i=j;i<nodes_number;i++)
        {
            if(nodes_hash[nodes[i]] == 1)
            {
                dfs_base2(0, nodes[i], -1, real_conn, 0);
                j = i;
                break;
            }
        }

    }
    return;
}

void make_subgraph(int start, int end)
{
    int i;
    nodes_number = 0;
    visited_nodes = 0;
    for(i=0;i<50000;i++)
    {
        nodes_hash[i] = 0;
    }
    //Make the adjacency list
    for(i=start;i<end;i++)
    {
        if(tree_find(connections[i][0]) != tree_find(connections[i][1]))
        {
            if(nodes_hash[tree_find(connections[i][0])] == 0)
            {
                nodes[nodes_number++] = tree_find(connections[i][0]);
                nodes_hash[tree_find(connections[i][0])] = 1;
            }
            if(nodes_hash[tree_find(connections[i][1])] == 0)
            {
                nodes[nodes_number++] = tree_find(connections[i][1]);
                nodes_hash[tree_find(connections[i][1])] = 1;
            }

            insert_adj(tree_find(connections[i][0]), tree_find(connections[i][1]), connections[i][2], connections[i][0], connections[i][1]);
            insert_adj(tree_find(connections[i][1]), tree_find(connections[i][0]), connections[i][2], connections[i][1], connections[i][0]);

        }
        else
        {
            result[2]++;
        }

    }
    if(nodes_number > 0)
    {
        //execute_dfs();
        execute_dfs2();
        //!!!!!---Make the union find
        for(i=start;i<end;i++)
        {
            tree_union(tree_find(connections[i][0]), tree_find(connections[i][1]));
        }
    }




    return;
}


void find_the_roads()
{
    int temp, j, temp_difference;
    int combo_start = 0;
    int combo_end = 1;
    temp = connections[combo_start][2];
    //Here we find the connections that have the same cost
    while(combo_end <= M)
    {
        while(temp == connections[combo_end][2])
        {
            if(combo_end++ == M){
                break;
            }
        }
        //printf("Current start: %d, end: %d\n", combo_start, combo_end);
        make_subgraph(combo_start, combo_end);
        combo_start = combo_end++;
        temp = connections[combo_start][2];
        //Free up the adjacency list for the next run
        for(j=0;j<N;j++)
        {
            free(adjacent[j]);
            adjacent[j] = NULL;
        }
       // printf("Current results: %d %d %d\n", result[0], result[2] , result[1]);
    }
    return;
}





int main()
{
    int flag_of_debug = 0;
    int i;
    long temp1, temp2, temp3;
    int num_of_correct, index, index_s, index_e;
    num_of_correct = 0;
    index_s = 1;
    index_e = 30;
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

            for(i=0;i<50000;i++)
            {
                union_tree[i] = -1;
                union_tree_sizes[i] = 0;
            }
            for(i=0;i<500000;i++)
            {
                connections[i][0] = 0;
                connections[i][1] = 0;
                connections[i][2] = 0;
            }
            //This prepares the input and output file for the debug to check if everything is ok
            prepare_debug(index);
            //This reads the data from the input file
            read_data();
            for(i=0;i<3;i++)
            {
                result[i] = 0;
            }
            printf("N = %d M = %d\n", N, M);
            //-----------CLOCK ENDING ----------------------------------
            end = clock();
            cpu_time_used = ((double) (end - checkpoint)) / CLOCKS_PER_SEC;
            checkpoint = end;
            //----------------------------------------------------------
            printf("Reading time elapsed: %f\n", cpu_time_used); // Debug

            find_the_roads();


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


            result[1] = M - result[0] - result[2];
            fscanf(fpout, "%d %d %d", &temp1, &temp2, &temp3);
            printf("My answer is: %d %d %d, and the correct answer is: %d %d %d, \n", result[0],result[2],result[1], temp1 , temp2 , temp3);
            if(result[0] == temp1 && result[2] == temp2 && result[1] == temp3)
            {
                printf("\n\n------  YOU DID IT PAL  ------\n\n");
                num_of_correct++;
            }
            else
            {
                printf("\n\n-----THIS IS WRONG THIS IS WRONG-----\n\nTestcase: %d\n\n", index);
            }

        }
        printf("Number of correct: %d out of: %d", num_of_correct, index_e - index_s + 1);
    }
    else
    {

        fp = stdin;
        for(i=0;i<50000;i++)
        {
            union_tree[i] = -1;
            union_tree_sizes[i] = 0;
        }
        read_data();
        for(i=0;i<3;i++)
        {
            result[i] = 0;
        }
        find_the_roads();
        result[1] = M - result[0] - result[2];
        printf("%d\n", result[0]);
        printf("%d\n", result[2]);
        printf("%d\n", result[1]);

    }
    return 0;
}




