// Parallel Machine Scheduling Problem (PMSP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_MACHINES 3
#define MAX_JOBS 5
#define MAX_ITERATIONS 10000
#define INITIAL_TEMPERATURE 100.0
#define COOLING_RATE 0.95
#define MIN_TEMPERATURE 1e-3
#define SWAP_PROBABILITY 0.7

// Global variables
int num_machines = MAX_MACHINES;
int num_jobs = MAX_JOBS;
int processing_times[MAX_MACHINES][MAX_JOBS] = {
    {3, 2, 2, 1, 4},
    {1, 4, 3, 2, 1},
    {2, 3, 1, 4, 3}
};
int current_solution[MAX_JOBS];
int best_solution[MAX_JOBS];
int best_cost;

// Function prototypes
void initialize();
void generate_initial_solution();
int calculate_cost(int solution[]);
void simulated_annealing();
void generate_neighbor_solution(int current_solution[]);
double acceptance_probability(int delta_cost, double temperature);
void update_solution(int new_solution[], int new_cost);

int main() {
    srand(time(NULL));

    initialize();
    simulated_annealing();

    printf("\nBest Solution Found:\n");
    for (int i = 0; i < num_jobs; i++) {
        printf("Job %d assigned to Machine %d\n", i + 1, best_solution[i] + 1);
    }
    printf("Cost of the best solution = %d\n", best_cost);

    return 0;
}

void initialize() {
    generate_initial_solution();
    best_cost = calculate_cost(current_solution);
    for (int i = 0; i < num_jobs; i++) {
        best_solution[i] = current_solution[i];
    }
}

void generate_initial_solution() {
    // Generate a random initial solution
    for (int i = 0; i < num_jobs; i++) {
        current_solution[i] = rand() % num_machines;
    }
}

int calculate_cost(int solution[]) {
    // Calculate the makespan (cost) of a given solution
    int machine_loads[MAX_MACHINES] = {0};
    for (int i = 0; i < num_jobs; i++) {
        int machine = solution[i];
        machine_loads[machine] += processing_times[machine][i];
    }
    int makespan = 0;
    for (int i = 0; i < num_machines; i++) {
        if (machine_loads[i] > makespan) {
            makespan = machine_loads[i];
        }
    }
    return makespan;
}

void simulated_annealing() {
    double temperature = INITIAL_TEMPERATURE;
    int current_cost = calculate_cost(current_solution);

    for (int iter = 0; iter < MAX_ITERATIONS && temperature > MIN_TEMPERATURE; iter++) {
        int neighbor_solution[MAX_JOBS];
        generate_neighbor_solution(current_solution);
        int neighbor_cost = calculate_cost(neighbor_solution);

        int cost_diff = neighbor_cost - current_cost;

        if (cost_diff < 0 || acceptance_probability(cost_diff, temperature) > ((double) rand() / RAND_MAX)) {
            update_solution(neighbor_solution, neighbor_cost);
            current_cost = neighbor_cost;

            if (current_cost < best_cost) {
                best_cost = current_cost;
                for (int i = 0; i < num_jobs; i++) {
                    best_solution[i] = current_solution[i];
                }
            }
        }

        temperature *= COOLING_RATE;
    }
}

void generate_neighbor_solution(int current_solution[]) {
    // Generate a neighbor solution by randomly swapping jobs between machines
    for (int i = 0; i < num_jobs; i++) {
        if ((double) rand() / RAND_MAX < SWAP_PROBABILITY) {
            int job = rand() % num_jobs;
            int machine = rand() % num_machines;
            current_solution[job] = machine;
        }
    }
}

double acceptance_probability(int delta_cost, double temperature) {
    // Calculate acceptance probability using Boltzmann distribution
    return exp(-delta_cost / temperature);
}

void update_solution(int new_solution[], int new_cost) {
    // Update current solution with a new solution
    for (int i = 0; i < num_jobs; i++) {
        current_solution[i] = new_solution[i];
    }
}
