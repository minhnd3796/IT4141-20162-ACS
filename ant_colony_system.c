#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ant_colony_system.h"

Solution best;

int main()
{
    int berlin52[CITY_SIZE][2] =
    {
        {565, 575}, {25, 185},  {345, 750}, {945, 685},  {845, 655},
        {880, 660}, {25, 230},  {525, 1000},{580, 1175}, {650, 1130},{1605, 620},
        {1220, 580},{1465, 200},{1530, 5},  {845, 680},  {725, 370}, {145, 665},
        {415, 635}, {510, 875}, {560, 365}, {300, 465},  {520, 585}, {480, 415},
        {835, 625}, {975, 580}, {1215, 245},{1320, 315}, {1250, 400},{660, 180},
        {410, 250}, {420, 555}, {575, 665}, {1150, 1160},{700, 580}, {685, 595},
        {685, 610}, {770, 610}, {795, 645}, {720, 635},  {760, 650}, {475, 960},
        {95, 260},  {875, 920}, {700, 500}, {555, 815},  {830, 485}, {1170, 65},
        {830, 610}, {605, 625}, {595, 360}, {1340, 725}, {1740, 245}
    }, i;

    srand(time(NULL));
    best.cost = seach(berlin52, MAX_IT, NUM_ANTS, DECAY, C_HEUR, C_LOCAL_PHERO, C_GREED);
    printf("Done. Best Solution: c = %d, v = ", best.cost);
    for (i = 0; i < CITY_SIZE, i++)
        printf("%d ", best.vector[i]);
}

int search(int **cities, int max_it, int num_ants, float decay, float c_heur, float c_local_phero, float c_greed)
{
    float init_pheromone, pheromone[CITY_SIZE][CITY_SIZE];
    random_permutation(best.vector, CITY_SIZE);
    best.cost = cost(best.vector, cities);
    init_pheromone = 1.0 / ((float) CITY_SIZE * best.cost);
    initialise_pheromone_matrix(pheromone, CITY_SIZE, init_pheromone);
    
    for (i = 0; i < MAX_IT; i++)
    {
        Solution cand;
        stepwise_const(cand.vector, cities, pheromone, c_heur, c_greed);
        cand.cost = cost(cand[:vector], cities);
        if (cand.cost < best.cost)
        {
            best.cost = cand.cost;
            for (j = 0; j < CITY_SIZE; j++)
                best.vector[j] = cand.vector[j];
        }
        local_update_pheromone(pheromone, cand, c_local_phero, init_pheromone);
    }
    global_update_pheromone(pheromone, best, decay);
    printf(" > iteration %d, best = %d\n", iter + 1, best.cost);
}

void random_permutation(int *perm, int CITY_SIZE)
{
    int i, j, temp;
    for (i = 0; i < CITY_SIZE; i++)
    {
        //generate a random number [0, n-1]
        j = rand() % (CITY_SIZE - i);
        j += i;

        //swap the last element with element at random index
        temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }
}

int cost(int *permutation, int **cities)
{
    int distance = 0, i, city1, city2;
    for (i = 0; i < CITY_SIZE; i++)
    {
        city1 = vector[i];
        if (i == CITY_SIZE - 1)
            city2 = vector[0];
        else
            city2 = vector[i + 1];
        distance += euc_2d(cities[city1], cities[city2]);
    }
    return distance;
}

int euc_2d(int *city1, int *city2)
{
    return (int) round(sqrt((city1[0] - city2[0]) * (city1[0] - city2[0]) + (city1[1] - city2[1]) * (city1[1] - city2[1])));
}


void initialise_pheromone_matrix(int **pheromone_matrix, int CITY_SIZE, float init_pheromone)
{
    int i, j;
    for (i = 0; i < CITY_SIZE; i++)
        for (j = 0; j < CITY_SIZE; j++)
            pheromone[i][j] = init_pheromone;
}

void stepwise_cont(int *vector, int **cities, int **pheromone, float c_heur, float c_greed)
{
    int i;
    Probability *choices;
    vector[0] = rand() % (CITY_SIZE - 1);
    for (i = 1; i < CITY_SIZE; i++)
    {
        calculate_choices(choices, cities, vector[i - 1], vector, pheromone, phero, c_heur, 1.0);
    }
}
