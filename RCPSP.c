// Resource-Constrained Project Scheduling Problem (RCPSP)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// Constants
#define MAX_TASKS 10
#define MAX_RESOURCES 5
#define INITIAL_TEMPERATURE 1000.0
#define COOLING_RATE 0.95
#define MIN_TEMPERATURE 0.1
#define MAX_ITERATIONS 1000

// Task structure
typedef struct {
    int duration;
    int resource_req[MAX_RESOURCES];
    int num_successors;
    int successors[MAX_TASKS];
} Task;

// Global variables
int num_tasks;
Task tasks[MAX_TASKS];
int resource_avail[MAX_RESOURCES];

// Function prototypes
void initialize();
int calculate_makespan(int schedule[]);
void generate_neighbor(int current_schedule[], int neighbor_schedule[]);
double acceptance_probability(int energy, int new_energy, double temperature);
void simulated_annealing();
void print_schedule(int schedule[]);

int main() {
    initialize();
    simulated_annealing();
    return 0;
}

// Initialize tasks and resources
void initialize() {
    srand(time(NULL)); // Initialize random seed

    // Example data for tasks and resource availability
    num_tasks = 5;

    tasks[0].duration = 3;
    tasks[0].num_successors = 1;
    tasks[0].successors[0] = 1;
    tasks[0].resource_req[0] = 1;
    tasks[0].resource_req[1] = 0;
    tasks[0].resource_req[2] = 0;
    tasks[0].resource_req[3] = 0;
    tasks[0].resource_req[4] = 0;

    tasks[1].duration = 5;
    tasks[1].num_successors = 1;
    tasks[1].successors[0] = 2;
    tasks[1].resource_req[0] = 0;
    tasks[1].resource_req[1] = 2;
    tasks[1].resource_req[2] = 1;
    tasks[1].resource_req[3] = 0;
    tasks[1].resource_req[4] = 0;

    tasks[2].duration = 2;
    tasks[2].num_successors = 1;
    tasks[2].successors[0] = 3;
    tasks[2].resource_req[0] = 0;
    tasks[2].resource_req[1] = 0;
    tasks[2].resource_req[2] = 1;
    tasks[2].resource_req[3] = 0;
    tasks[2].resource_req[4] = 0;

    tasks[3].duration = 4;
    tasks[3].num_successors = 1;
    tasks[3].successors[0] = 4;
    tasks[3].resource_req[0] = 1;
    tasks[3].resource_req[1] = 1;
    tasks[3].resource_req[2] = 0;
    tasks[3].resource_req[3] = 0;
    tasks[3].resource_req[4] = 0;

    tasks[4].duration = 1;
    tasks[4].num_successors = 0;
    tasks[4].resource_req[0] = 0;
    tasks[4].resource_req[1] = 0;
    tasks[4].resource_req[2] = 0;
    tasks[4].resource_req[3] = 1;
    tasks[4].resource_req[4] = 1;

    // Initialize resource availability
    resource_avail[0] = 2; // Resource R1
    resource_avail[1] = 3; // Resource R2
    resource_avail[2] = 2; // Resource R3
    resource_avail[3] = 1; // Resource R4
    resource_avail[4] = 1; // Resource R5
}

// Calculate makespan of a schedule
int calculate_makespan(int schedule[]) {
    int start_time[MAX_TASKS] = {0};
    int end_time[MAX_TASKS] = {0};

    for (int i = 0; i < num_tasks; ++i) {
        int task_index = schedule[i];
        int task_duration = tasks[task_index].duration;
        int earliest_start = 0;

        // Calculate earliest start time considering predecessors
        for (int j = 0; j < tasks[task_index].num_successors; ++j) {
            int successor_index = tasks[task_index].successors[j];
            int finish_time = end_time[successor_index];
            if (finish_time > earliest_start) {
                earliest_start = finish_time;
            }
        }

        // Calculate end time and update resource availability
        int allocated_resources[MAX_RESOURCES] = {0};
        for (int r = 0; r < MAX_RESOURCES; ++r) {
            allocated_resources[r] = resource_avail[r];
        }

        for (int t = earliest_start; t < earliest_start + task_duration; ++t) {
            for (int r = 0; r < MAX_RESOURCES; ++r) {
                allocated_resources[r] -= tasks[task_index].resource_req[r];
                if (allocated_resources[r] < 0) {
                    allocated_resources[r] = 0;
                }
            }
        }

        start_time[task_index] = earliest_start;
        end_time[task_index] = earliest_start + task_duration;
    }

    // Find the makespan
    int makespan = 0;
    for (int i = 0; i < num_tasks; ++i) {
        if (end_time[i] > makespan) {
            makespan = end_time[i];
        }
    }

    return makespan;
}

// Generate a neighboring solution (schedule)
void generate_neighbor(int current_schedule[], int neighbor_schedule[]) {
    memcpy(neighbor_schedule, current_schedule, sizeof(int) * MAX_TASKS);
    int idx1 = rand() % num_tasks;
    int idx2 = rand() % num_tasks;
    int temp = neighbor_schedule[idx1];
    neighbor_schedule[idx1] = neighbor_schedule[idx2];
    neighbor_schedule[idx2] = temp;
}

// Calculate acceptance probability for simulated annealing
double acceptance_probability(int energy, int new_energy, double temperature) {
    if (new_energy < energy) {
        return 1.0;
    }
    return exp((energy - new_energy) / temperature);
}

// Simulated annealing algorithm
void simulated_annealing() {
    int current_schedule[MAX_TASKS];
    int best_schedule[MAX_TASKS];
    int current_makespan, best_makespan;
    double temperature = INITIAL_TEMPERATURE;

    // Initialize current schedule randomly
    for (int i = 0; i < num_tasks; ++i) {
        current_schedule[i] = i;
    }
    best_makespan = calculate_makespan(current_schedule);
    memcpy(best_schedule, current_schedule, sizeof(current_schedule));

    // Simulated annealing loop
    while (temperature > MIN_TEMPERATURE) {
        for (int i = 0; i < MAX_ITERATIONS; ++i) {
            int neighbor_schedule[MAX_TASKS];
            generate_neighbor(current_schedule, neighbor_schedule);
            int neighbor_makespan = calculate_makespan(neighbor_schedule);
            double prob = acceptance_probability(current_makespan, neighbor_makespan, temperature);
            if (prob > (double) rand() / RAND_MAX) {
                memcpy(current_schedule, neighbor_schedule, sizeof(neighbor_schedule));
                current_makespan = neighbor_makespan;
                if (current_makespan < best_makespan) {
                    best_makespan = current_makespan;
                    memcpy(best_schedule, current_schedule, sizeof(current_schedule));
                }
            }
        }
        temperature *= COOLING_RATE;
    }

    // Print the best schedule found
    printf("Best Schedule (Makespan: %d):\n", best_makespan);
    print_schedule(best_schedule);
}

// Helper function to print the schedule
void print_schedule(int schedule[]) {
    for (int i = 0; i < num_tasks; ++i) {
        printf("Task %d ➜ Position %d\n", i, schedule[i]);
    }
}
