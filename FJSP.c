// Flexible Job-Shop Scheduling Problem (FJSP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_JOBS 3
#define MAX_MACHINES 2
#define MAX_OPERATIONS 10
#define MAX_ITERATIONS 10000

// Structure definitions
typedef struct {
    int job_id;
    int machine_id;
    int processing_time;
} Operation;

typedef struct {
    int num_operations;
    Operation operations[MAX_OPERATIONS];
} Job;

// Global variables
Job jobs[MAX_JOBS];
int num_jobs;
int num_machines;

// Function prototypes
void initialize_problem();
void initialize_solution(int solution[]);
int calculate_makespan(int solution[]);
void generate_neighbor(int current_solution[], int neighbor_solution[]);
double acceptance_probability(int current_makespan, int neighbor_makespan, double temperature);
void simulated_annealing(int initial_solution[]);

int main() {
    // Initialize random seed
    srand(time(NULL));

    // Initialize problem instance
    initialize_problem();

    // Start simulated annealing
    int initial_solution[MAX_OPERATIONS];
    initialize_solution(initial_solution);
    simulated_annealing(initial_solution);

    return 0;
}

// Function to initialize the FJSP problem instance
void initialize_problem() {
    // Example initialization: Define jobs and operations
    num_jobs = 3; // Example: Three jobs
    num_machines = 2; // Example: Two machines

    // Example: Job 0 has two operations: (0, 0, 3) and (0, 1, 5) -> Job 0, Machine 0, Processing Time 3
    jobs[0].num_operations = 2;
    jobs[0].operations[0] = (Operation){0, 0, 3};
    jobs[0].operations[1] = (Operation){0, 1, 5};

    // Similar initialization for other jobs...
}

// Function to initialize a random initial solution
void initialize_solution(int solution[]) {
    // Example: Random permutation of operations
    int index = 0;
    for (int i = 0; i < num_jobs; ++i) {
        for (int j = 0; j < jobs[i].num_operations; ++j) {
            solution[index++] = i; // Assign all operations of job i sequentially
        }
    }

    // Randomly shuffle the initial solution
    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        int temp = solution[i];
        int swap_index = rand() % MAX_OPERATIONS;
        solution[i] = solution[swap_index];
        solution[swap_index] = temp;
    }
}

// Function to calculate the makespan of a solution
int calculate_makespan(int solution[]) {
    int machine_completion_times[MAX_MACHINES] = {0};

    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        int job_index = solution[i];
        for (int j = 0; j < jobs[job_index].num_operations; ++j) {
            Operation op = jobs[job_index].operations[j];
            int machine_id = op.machine_id;
            int processing_time = op.processing_time;
            machine_completion_times[machine_id] += processing_time;
        }
    }

    int max_completion_time = 0;
    for (int i = 0; i < MAX_MACHINES; ++i) {
        if (machine_completion_times[i] > max_completion_time) {
            max_completion_time = machine_completion_times[i];
        }
    }

    return max_completion_time;
}

// Function to generate a neighboring solution
void generate_neighbor(int current_solution[], int neighbor_solution[]) {
    // Generate a neighbor by swapping two random operations
    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        neighbor_solution[i] = current_solution[i];
    }

    // Randomly select two indices and swap them
    int index1 = rand() % MAX_OPERATIONS;
    int index2 = rand() % MAX_OPERATIONS;

    int temp = neighbor_solution[index1];
    neighbor_solution[index1] = neighbor_solution[index2];
    neighbor_solution[index2] = temp;
}

// Function to calculate acceptance probability in simulated annealing
double acceptance_probability(int current_makespan, int neighbor_makespan, double temperature) {
    if (neighbor_makespan < current_makespan) {
        return 1.0;
    } else {
        return exp(-((double)(neighbor_makespan - current_makespan)) / temperature);
    }
}

// Function implementing simulated annealing
void simulated_annealing(int initial_solution[]) {
    int current_solution[MAX_OPERATIONS];
    int neighbor_solution[MAX_OPERATIONS];
    double temperature = 100.0;
    double cooling_rate = 0.99;
    int current_makespan, neighbor_makespan;
    int best_solution[MAX_OPERATIONS];
    int best_makespan;

    // Initialize current solution
    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        current_solution[i] = initial_solution[i];
    }
    current_makespan = calculate_makespan(current_solution);

    // Initialize best solution
    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        best_solution[i] = initial_solution[i];
    }
    best_makespan = current_makespan;

    // Simulated Annealing loop
    int iteration = 0;
    while (iteration < MAX_ITERATIONS && temperature > 1.0) {
        // Generate a neighbor solution
        generate_neighbor(current_solution, neighbor_solution);
        neighbor_makespan = calculate_makespan(neighbor_solution);

        // Decide whether to move to the neighbor solution
        double probability = acceptance_probability(current_makespan, neighbor_makespan, temperature);
        double rand_prob = ((double)rand() / RAND_MAX);

        if (rand_prob < probability) {
            for (int i = 0; i < MAX_OPERATIONS; ++i) {
                current_solution[i] = neighbor_solution[i];
            }
            current_makespan = neighbor_makespan;
        }

        // Update the best solution found so far
        if (current_makespan < best_makespan) {
            for (int i = 0; i < MAX_OPERATIONS; ++i) {
                best_solution[i] = current_solution[i];
            }
            best_makespan = current_makespan;
        }

        // Cool down the temperature
        temperature *= cooling_rate;
        iteration++;
    }

    // Output the best solution found
    printf("Best Makespan found: %d\n", best_makespan);
    printf("Best Solution order:\n");
    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        printf("%d ", best_solution[i]);
    }
    printf("\n");
}
