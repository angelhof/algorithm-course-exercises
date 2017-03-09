
#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15
#include <time.h>
#include <string.h>

#define MAX( a, b ) ( ( a > b) ? a : b )
#define MIN( a, b ) ( ( a < b) ? a : b )

struct adjacent_list_node{
    int cost;
    int planet;
    struct adjacent_list_node *next;
};

typedef struct connection_struct{
    int planet1;
    int planet2;
    int cost;
    int teleport;
} Connection;

char buffer[BSIZE];
char NOF[3], inputfile[100], outputfile[100];
long bpos = 0L, bsize = 0L;
FILE *fp, *fpout; //the input file used for debugging
static struct adjacent_list_node *adjacent[10000] = {NULL};
static long teleporter_hash[10000] = {0};
static int teleporter[10000] = {0};
static int N, M, K;
static int prior_queue[510000][4] = {0}; // The fourth position is 1 when this connection sis a teleport
static int prior_queue_length = 0;
static int active_teleporters[10000] = {0};
static int inside_mst[10000] = {0};
static int number_of_planets_in_mst = 0;

int cmpfunc(const void * a, const void * b)
{
    return ( teleporter_hash[*(const long *)a] - teleporter_hash[*(const long *)b]);// ERORRRRRRRRR--------------------------------------------------------------------
}


void prior_queue_move(int in, int out)
{
    int i;
    for(i=0;i<4;i++)
    {
        prior_queue[in][i] = prior_queue[out][i];
    }
    return;
}

void prior_queue_swap(int in, int out)
{
    int i, temp[4];
    for(i=0;i<4;i++)
    {
        temp[i] = prior_queue[in][i];
        prior_queue[in][i] = prior_queue[out][i];
        prior_queue[out][i] = temp[i];
    }
    return;
}

void bubble_up(int i)
{
    int p;
    p = ((i + 1) / 2) - 1;
    while( i > 0 && prior_queue[p][2] > prior_queue[i][2])
    {
        prior_queue_swap(i , p);
        i = p;
        p = ((i + 1) / 2) - 1;
    }
    return;
}

void combine(int point)
{
    int l, r, mp;
    l = 2 * (point + 1) - 1;
    r = 2 * (point + 1);
    mp = point;
    if(l < prior_queue_length && prior_queue[l][2] <  prior_queue[mp][2]){ mp = l;}
    if(r < prior_queue_length && prior_queue[r][2] <  prior_queue[mp][2]){ mp = r;}
    if(mp != point)
    {
        prior_queue_swap(mp, point);
        combine(mp);
    }
    return;
}

void print_prior()
{
    int i, k;
    k = 0;
    for(i=0;i<prior_queue_length;i++)
    {
        printf("  (%d, %d, %d)", prior_queue[i][0], prior_queue[i][1], prior_queue[i][2]);
        if(prior_queue[i][3] == 1)
        {
            printf(" T");
        }
        if(i == k){
            printf("\n");
            k = k * 2 + 2;
        }
    }
    return;
}


Connection prior_max()
{
    Connection max_s ;
    max_s.planet1 = prior_queue[0][0];
    max_s.planet2 = prior_queue[0][1];
    max_s.cost = prior_queue[0][2];
    max_s.teleport = prior_queue[0][3];
    return max_s;
}

Connection delete_max()
{
    Connection max_s ;
    max_s.planet1 = prior_queue[0][0];
    max_s.planet2 = prior_queue[0][1];
    max_s.cost = prior_queue[0][2];
    max_s.teleport = prior_queue[0][3];
    if(prior_queue_length == 0){ printf("Tried to delete empty queue\n"); exit(1);}
    prior_queue_move(0, --prior_queue_length);
    combine(0);
    return max_s;
}

void prior_insert(int start, int end, int cost, int teleport)
{
    int i, p;
    prior_queue[prior_queue_length][0] = start;
    prior_queue[prior_queue_length][1] = end;
    prior_queue[prior_queue_length][2] = cost;
    prior_queue[prior_queue_length++][3] = teleport;
    i = prior_queue_length - 1;
    p = ((i + 1) / 2) - 1;
    while( i > 0 && prior_queue[p][2] > prior_queue[i][2])
    {
        prior_queue_swap(i , p);
        i = p;
        p = ((i + 1) / 2) - 1;
    }
    return;
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
    printf("\n--- Planet: %d ---\n", i);
    while(curr != NULL)
    {
        printf("%d with cost: %d\n", curr->planet, curr->cost);
        curr = curr->next;
    }
    printf("\n");
    return;
}

void insert_adj(int x, int y, int cost)
{

    struct adjacent_list_node **curr, *temp;
    curr = &adjacent[x];
    if((*curr) != NULL)
    {
        temp = (adjacent_list_node*)malloc(sizeof(struct adjacent_list_node));
        temp->cost = cost;
        temp->planet = y;
        temp->next = (*curr)->next;
        (*curr)->next = temp;

    }
    else
    {
        temp = (adjacent_list_node*)malloc(sizeof(struct adjacent_list_node));
        temp->cost = cost;
        temp->planet = y;
        temp->next = NULL;
        (*curr) = temp;
    }

    return;
}

void read_data()
{
    int i, temp_planet_1, temp_planet_2, temp_cost, temp;
    N = readLong();
    K = readLong();
    M = readLong();
    //printf("%d %d %d\n", N, K, M);
    for(i=0;i<10000;i++){
        teleporter_hash[i] = -1;
    }
    for(i=0;i<K;i++){
        temp = readLong() - 1;
        teleporter[i] = temp;
        teleporter_hash[temp] = readLong() ;
    }

    for(i=0;i<M;i++)
    {
        temp_planet_1 = readLong() - 1;
        temp_planet_2 = readLong() - 1;
        temp_cost = readLong();
        insert_adj(temp_planet_1, temp_planet_2, temp_cost);
        insert_adj(temp_planet_2, temp_planet_1, temp_cost);
    }
    qsort(teleporter, K, sizeof(teleporter[0]), cmpfunc);
    return;
}


void add_teleporter_connections()
{
    int i, teleport_cost;
    for(i=1;i<K;i++)
    {
        teleport_cost = 0;
        if(active_teleporters[teleporter[0]] == 0)
        {
            teleport_cost += teleporter_hash[teleporter[0]];
        }
        teleport_cost += teleporter_hash[teleporter[i]];
        //printf("--Teleportation--The planet we are on now: %d the one we want to go: %d and the cost: %d\n", planet, teleporter[i], teleport_cost);
        prior_insert(teleporter[0], teleporter[i], teleport_cost, 1);
//        print_prior();
//        printf("\n\n");

    }
}

void add_close_planets_to_prior(int planet)
{
    int i, teleport_cost;
    struct adjacent_list_node *curr = adjacent[planet];
    //This adds all the adjacent planets on the priority queue
    while(curr != NULL)
    {

        if(inside_mst[curr->planet] == 0)
        {
            //printf("The planet we are on now: %d the one we want to go: %d and the cost: %d\n", planet, curr->planet, curr->cost);
            prior_insert(planet, curr->planet, curr->cost, 0);
        }
//        print_prior();
//        printf("\n\n");
        curr = curr->next;
    }

}



long find_mst(char special_case)
{
    Connection curr_connection;
    int i, flag1;
    long total_cost = 0;
    flag1 = 0;
    add_close_planets_to_prior(teleporter[0]);
    number_of_planets_in_mst++;
    inside_mst[teleporter[0]] = 1;


    //In this case we turn on the cheapest teleporter and then we find the mst
    if(special_case == 's')
    {
        active_teleporters[teleporter[0]] = 1;
        total_cost += teleporter_hash[teleporter[0]];
    }

    add_teleporter_connections();
//    printf("Total cost now is: %ld, Because the planet: %d was added\n", total_cost, teleporter[0]);
//    print_prior();
//    getchar();
    while(number_of_planets_in_mst < N)
    {
        curr_connection = delete_max();

        //printf("Current best is: planets: %d - %d and cost: %d\n", curr_connection.planet1, curr_connection.planet2, curr_connection.cost);
        while(inside_mst[curr_connection.planet1] == 1 && inside_mst[curr_connection.planet2] == 1)
        {
            //printf("But it was already in the graph :(\n");
            curr_connection = delete_max();
            //printf("Current best is: planets: %d - %d and cost: %d\n", curr_connection.planet1, curr_connection.planet2, curr_connection.cost);
        }

        if(curr_connection.teleport == 1 && flag1 == 0)
        {
            if(active_teleporters[curr_connection.planet1] == 0)
            {
                flag1 = 1;
            }
            active_teleporters[curr_connection.planet1] = 1;
            for(i=0;i<prior_queue_length;i++)
            {
                if(prior_queue[i][3] == 1)
                {
                    if( flag1 == 1 && (prior_queue[i][0] == curr_connection.planet1 || prior_queue[i][1] == curr_connection.planet1) )
                    {
                        prior_queue[i][2] -= teleporter_hash[curr_connection.planet1];
                        bubble_up(i);
                    }
                }
            }
        }
        add_close_planets_to_prior(curr_connection.planet2);
        number_of_planets_in_mst++;
        inside_mst[curr_connection.planet2] = 1;
        total_cost += curr_connection.cost;
        //printf("Total cost now is: %ld, Because the planet: %d was added\n", total_cost, curr_connection.planet2);
        //print_prior();
//        getchar();
        //printf("%ld\n", prior_queue_length);
    }
    return total_cost;
}


int main()
{
    int flag_of_debug = 0;
    int i, j, k, temp, result, result2;
    long temp1, temp2;
    int num_of_correct, index, index_s, index_e;
    num_of_correct = 0;
    index_s = 1;
    index_e = 41;
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

            //This prepares the input and output file for the debug to check if everything is ok
            prepare_debug(index);
            prior_queue_length = 0;
            number_of_planets_in_mst = 0;
            for(i=0;i<10000;i++)
            {
                inside_mst[i] = 0;
                teleporter[i] = 0;
                active_teleporters[i] = 0;
                teleporter_hash[i] = 0;
                adjacent[i] = NULL;
            }
            //This reads the data from the input file
            read_data();
            printf("%ld %ld %ld\n", N, K, M);
            //-----------CLOCK ENDING ----------------------------------
            end = clock();
            cpu_time_used = ((double) (end - checkpoint)) / CLOCKS_PER_SEC;
            checkpoint = end;
            //----------------------------------------------------------
            printf("Reading time elapsed: %f\n", cpu_time_used); // Debug


            result = find_mst('n');
            prior_queue_length = 0;
            number_of_planets_in_mst = 0;
            for(i=0;i<10000;i++)
            {
                inside_mst[i] = 0;
            }
            result2 = find_mst('s');

            printf("%ld %ld\n", result, result2);
            result = MIN(result,result2);

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




            fscanf(fpout, "%ld", &temp1);
            printf("My answer is: %ld, and the correct answer is: %ld, \n", result, temp1 );
            if(result == temp1)
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

        read_data();
        result = find_mst('n');

        //Just clearin up before refinding the MST
        prior_queue_length = 0;
        number_of_planets_in_mst = 0;
        for(i=0;i<10000;i++)
        {
            inside_mst[i] = 0;
        }
        //If there are no teleporters either way no need to do it
        if(K > 0)
        {
            result2 = find_mst('s');
            result = MIN(result,result2);
        }


        printf("%ld\n", result);
    }
    return 0;
}




