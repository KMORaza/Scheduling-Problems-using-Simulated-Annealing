
// Single Machine Total Weighted Tardiness Problem (SMTWTP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_JOBS 10 // Maximum number of jobs
#define MAX_ITER 10000 // Maximum number of iterations for SA
#define INITIAL_TEMP 100.0 // Initial temperature
#define COOLING_RATE 0.95 // Cooling rate

// Structure to hold job information
typedef struct {
    int processing_time;
    int weight;
} Job;

// Function prototypes
void initialize_jobs(Job jobs[], int n);
int calculate_total_tardiness(Job jobs[], int n, int order[]);
void swap(int *a, int *b);
double acceptance_probability(int current_tardiness, int new_tardiness, double temperature);
void simulated_annealing(Job jobs[], int n, int order[]);

int main() {
    int n = 5; // Number of jobs
    Job jobs[MAX_JOBS] = {
        {3, 4}, // processing_time = 3, weight = 4
        {7, 2}, // processing_time = 7, weight = 2
        {2, 5}, // processing_time = 2, weight = 5
        {5, 7}, // processing_time = 5, weight = 7
        {4, 3}  // processing_time = 4, weight = 3
    };
    int order[MAX_JOBS]; // Order of jobs (solution)

    // Solve using simulated annealing
    simulated_annealing(jobs, n, order);

    // Output the optimal order found
    printf("Optimal order of jobs to minimize total weighted tardiness:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", order[i]);
    }
    printf("\n");

    // Calculate and print the total weighted tardiness of the optimal order
    int total_tardiness = calculate_total_tardiness(jobs, n, order);
    printf("Total weighted tardiness: %d\n", total_tardiness);

    return 0;
}

// Function to calculate the total weighted tardiness for a given order of jobs
int calculate_total_tardiness(Job jobs[], int n, int order[]) {
    int current_time = 0;
    int total_tardiness = 0;
    for (int i = 0; i < n; i++) {
        int job_index = order[i];
        current_time += jobs[job_index].processing_time;
        int tardiness = fmax(0, current_time - jobs[job_index].weight);
        total_tardiness += tardiness;
    }
    return total_tardiness;
}

// Function to swap two integers
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to calculate acceptance probability in simulated annealing
double acceptance_probability(int current_tardiness, int new_tardiness, double temperature) {
    if (new_tardiness < current_tardiness) {
        return 1.0; // Accept the new solution if it's better
    }
    return exp((current_tardiness - new_tardiness) / temperature);
}

// Function implementing simulated annealing to solve the problem
void simulated_annealing(Job jobs[], int n, int order[]) {
    int current_order[MAX_JOBS]; // Current order of jobs
    int best_order[MAX_JOBS]; // Best order found so far
    int current_tardiness, best_tardiness;
    double temperature = INITIAL_TEMP;

    // Initialize the current order randomly
    for (int i = 0; i < n; i++) {
        current_order[i] = i;
    }

    // Initialize best order as current order
    for (int i = 0; i < n; i++) {
        best_order[i] = current_order[i];
    }

    // Calculate initial total tardiness
    current_tardiness = calculate_total_tardiness(jobs, n, current_order);
    best_tardiness = current_tardiness;

    // Simulated annealing loop
    for (int iter = 0; iter < MAX_ITER; iter++) {
        // Generate new solution by swapping two jobs
        int i = rand() % n;
        int j = rand() % n;
        swap(&current_order[i], &current_order[j]);

        // Calculate new total tardiness
        int new_tardiness = calculate_total_tardiness(jobs, n, current_order);

        // Decide whether to accept the new solution
        if (acceptance_probability(current_tardiness, new_tardiness, temperature) > (double)rand() / RAND_MAX) {
            // Accept the new solution
            current_tardiness = new_tardiness;
            // Update best solution found so far
            if (current_tardiness < best_tardiness) {
                best_tardiness = current_tardiness;
                for (int k = 0; k < n; k++) {
                    best_order[k] = current_order[k];
                }
            }
        } else {
            // Revert to the previous solution
            swap(&current_order[i], &current_order[j]);
        }

        // Cool down the temperature
        temperature *= COOLING_RATE;
    }

    // Set the best order found
    for (int i = 0; i < n; i++) {
        order[i] = best_order[i];
    }
}
