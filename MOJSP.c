// Multi-Operation Job-Shop Scheduling Problem (MOJSP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_JOBS 5
#define MAX_OPERATIONS 3
#define MAX_MACHINES 3
#define MAX_ITERATIONS 10000
#define INITIAL_TEMPERATURE 100.0
#define COOLING_RATE 0.95
#define MIN_TEMPERATURE 1e-3

int n_jobs = MAX_JOBS; // Number of jobs
int n_operations[MAX_JOBS] = {3, 2, 3, 2, 2}; // Number of operations for each job
int processing_time[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES] = {
    {{2, 3, 4}, {3, 2, 1}, {4, 3, 2}},
    {{1, 2, 3}, {2, 1, 2}},
    {{3, 4, 5}, {2, 3, 4}, {4, 5, 6}},
    {{2, 1, 3}, {3, 2, 1}},
    {{1, 2, 1}, {2, 1, 2}}
}; // Processing times

// Function to generate a random initial solution
void generate_initial_solution(int schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES]) {
    srand(time(NULL));
    for (int i = 0; i < n_jobs; ++i) {
        for (int j = 0; j < n_operations[i]; ++j) {
            schedule[i][j][0] = rand() % MAX_MACHINES; // Random machine assignment
        }
    }
}

// Function to calculate makespan for a given solution
int calculate_makespan(int schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES]) {
    int machine_end_time[MAX_MACHINES] = {0};
    for (int i = 0; i < n_jobs; ++i) {
        for (int j = 0; j < n_operations[i]; ++j) {
            int machine = schedule[i][j][0]; // Machine for the operation
            int start_time = machine_end_time[machine];
            int end_time = start_time + processing_time[i][j][machine];
            machine_end_time[machine] = end_time;
        }
    }
    int makespan = 0;
    for (int i = 0; i < MAX_MACHINES; ++i) {
        if (machine_end_time[i] > makespan) {
            makespan = machine_end_time[i];
        }
    }
    return makespan;
}

// Function to generate a neighbor solution
void generate_neighbor(int current_schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES],
                       int neighbor_schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES]) {
    // Copy current solution to neighbor solution
    for (int i = 0; i < n_jobs; ++i) {
        for (int j = 0; j < n_operations[i]; ++j) {
            for (int k = 0; k < MAX_MACHINES; ++k) {
                neighbor_schedule[i][j][k] = current_schedule[i][j][k];
            }
        }
    }

    // Perform a random swap of machines for one operation
    int i = rand() % n_jobs;
    int j = rand() % n_operations[i];
    int k1 = rand() % MAX_MACHINES;
    int k2 = rand() % MAX_MACHINES;
    // Swap machines k1 and k2 for job i, operation j
    int temp = neighbor_schedule[i][j][k1];
    neighbor_schedule[i][j][k1] = neighbor_schedule[i][j][k2];
    neighbor_schedule[i][j][k2] = temp;
}

// Simulated annealing function to solve MOJSP
void simulated_annealing(int initial_schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES]) {
    int current_schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES];
    int neighbor_schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES];

    // Initialize current schedule with initial solution
    for (int i = 0; i < n_jobs; ++i) {
        for (int j = 0; j < n_operations[i]; ++j) {
            for (int k = 0; k < MAX_MACHINES; ++k) {
                current_schedule[i][j][k] = initial_schedule[i][j][k];
            }
        }
    }

    double temperature = INITIAL_TEMPERATURE;
    int current_makespan = calculate_makespan(current_schedule);
    int best_makespan = current_makespan;
    int best_schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES];
    int iteration = 0;

    while (temperature > MIN_TEMPERATURE && iteration < MAX_ITERATIONS) {
        generate_neighbor(current_schedule, neighbor_schedule);
        int neighbor_makespan = calculate_makespan(neighbor_schedule);
        int delta = neighbor_makespan - current_makespan;

        if (delta < 0 || exp(-delta / temperature) > ((double)rand() / RAND_MAX)) {
            // Accept the neighbor solution
            for (int i = 0; i < n_jobs; ++i) {
                for (int j = 0; j < n_operations[i]; ++j) {
                    for (int k = 0; k < MAX_MACHINES; ++k) {
                        current_schedule[i][j][k] = neighbor_schedule[i][j][k];
                    }
                }
            }
            current_makespan = neighbor_makespan;

            // Update best solution found so far
            if (current_makespan < best_makespan) {
                best_makespan = current_makespan;
                for (int i = 0; i < n_jobs; ++i) {
                    for (int j = 0; j < n_operations[i]; ++j) {
                        for (int k = 0; k < MAX_MACHINES; ++k) {
                            best_schedule[i][j][k] = current_schedule[i][j][k];
                        }
                    }
                }
            }
        }

        // Cool down the temperature
        temperature *= COOLING_RATE;
        iteration++;
    }

    // Output the best solution found
    printf("Best Makespan: %d\n", best_makespan);
    printf("Best Schedule:\n");
    for (int i = 0; i < n_jobs; ++i) {
        printf("Job %d: ", i + 1);
        for (int j = 0; j < n_operations[i]; ++j) {
            printf("(Operation %d, Machine %d) ", j + 1, best_schedule[i][j][0] + 1);
        }
        printf("\n");
    }
}

int main() {
    int initial_schedule[MAX_JOBS][MAX_OPERATIONS][MAX_MACHINES];

    generate_initial_solution(initial_schedule);

    printf("Initial Schedule:\n");
    for (int i = 0; i < n_jobs; ++i) {
        printf("Job %d: ", i + 1);
        for (int j = 0; j < n_operations[i]; ++j) {
            printf("(Operation %d, Machine %d) ", j + 1, initial_schedule[i][j][0] + 1);
        }
        printf("\n");
    }

    printf("\nRunning Simulated Annealing...\n");
    simulated_annealing(initial_schedule);

    return 0;
}
