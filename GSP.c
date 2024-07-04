// Group-Shop Scheduling Problem (GSP)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAX_GROUPS 5
#define MAX_JOBS 5
#define INITIAL_TEMP 100.0
#define COOLING_RATE 0.95
#define MIN_TEMP 1e-3
#define ITERATIONS_PER_TEMP 100

// Function prototypes
void generate_random_schedule(int schedule[MAX_GROUPS][MAX_JOBS]);
int calculate_schedule_cost(int schedule[MAX_GROUPS][MAX_JOBS]);
void copy_schedule(int dest[MAX_GROUPS][MAX_JOBS], int src[MAX_GROUPS][MAX_JOBS]);
void print_schedule(int schedule[MAX_GROUPS][MAX_JOBS]);
double acceptance_probability(int energy_diff, double temperature);
void simulated_annealing(int schedule[MAX_GROUPS][MAX_JOBS]);

int main() {
    int schedule[MAX_GROUPS][MAX_JOBS];

    // Generate an initial random schedule
    generate_random_schedule(schedule);

    // Print initial schedule
    printf("Initial Schedule:\n");
    print_schedule(schedule);

    // Solve using simulated annealing
    simulated_annealing(schedule);

    // Print final schedule
    printf("\nFinal Schedule:\n");
    print_schedule(schedule);

    return 0;
}

// Function to generate a random initial schedule
void generate_random_schedule(int schedule[MAX_GROUPS][MAX_JOBS]) {
    srand(time(NULL));
    for (int i = 0; i < MAX_GROUPS; i++) {
        for (int j = 0; j < MAX_JOBS; j++) {
            schedule[i][j] = rand() % 100; // Random job completion times (assuming some metric for cost)
        }
    }
}

// Function to calculate the cost (total completion time) of a schedule
int calculate_schedule_cost(int schedule[MAX_GROUPS][MAX_JOBS]) {
    int total_cost = 0;
    for (int i = 0; i < MAX_GROUPS; i++) {
        int group_cost = 0;
        for (int j = 0; j < MAX_JOBS; j++) {
            group_cost += schedule[i][j];
        }
        total_cost += group_cost;
    }
    return total_cost;
}

// Function to copy a schedule
void copy_schedule(int dest[MAX_GROUPS][MAX_JOBS], int src[MAX_GROUPS][MAX_JOBS]) {
    for (int i = 0; i < MAX_GROUPS; i++) {
        for (int j = 0; j < MAX_JOBS; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// Function to print a schedule
void print_schedule(int schedule[MAX_GROUPS][MAX_JOBS]) {
    for (int i = 0; i < MAX_GROUPS; i++) {
        printf("Group %d: ", i + 1);
        for (int j = 0; j < MAX_JOBS; j++) {
            printf("%d ", schedule[i][j]);
        }
        printf("\n");
    }
}

// Function to calculate acceptance probability
double acceptance_probability(int energy_diff, double temperature) {
    if (energy_diff < 0) {
        return 1.0;
    }
    return exp(-energy_diff / temperature);
}

// Simulated annealing function
void simulated_annealing(int schedule[MAX_GROUPS][MAX_JOBS]) {
    int current_schedule[MAX_GROUPS][MAX_JOBS];
    copy_schedule(current_schedule, schedule);

    int current_cost = calculate_schedule_cost(current_schedule);
    int best_cost = current_cost;
    int best_schedule[MAX_GROUPS][MAX_JOBS];
    copy_schedule(best_schedule, current_schedule);

    double temperature = INITIAL_TEMP;
    int iteration = 0;

    while (temperature > MIN_TEMP) {
        for (int i = 0; i < ITERATIONS_PER_TEMP; i++) {
            // Generate a neighboring solution (swap two random jobs in two random groups)
            int group1 = rand() % MAX_GROUPS;
            int group2 = rand() % MAX_GROUPS;
            int job1 = rand() % MAX_JOBS;
            int job2 = rand() % MAX_JOBS;

            // Swap jobs
            int temp = current_schedule[group1][job1];
            current_schedule[group1][job1] = current_schedule[group2][job2];
            current_schedule[group2][job2] = temp;

            // Calculate cost of the new schedule
            int new_cost = calculate_schedule_cost(current_schedule);

            // Calculate energy difference
            int cost_diff = new_cost - current_cost;

            // Decide whether to accept the new schedule
            if (acceptance_probability(cost_diff, temperature) > ((double) rand() / RAND_MAX)) {
                current_cost = new_cost;
                if (current_cost < best_cost) {
                    best_cost = current_cost;
                    copy_schedule(best_schedule, current_schedule);
                }
            } else {
                // Revert the swap
                temp = current_schedule[group1][job1];
                current_schedule[group1][job1] = current_schedule[group2][job2];
                current_schedule[group2][job2] = temp;
            }
        }
        // Cool the temperature
        temperature *= COOLING_RATE;
        iteration++;
    }

    // Copy the best schedule found back to the original schedule
    copy_schedule(schedule, best_schedule);
}
