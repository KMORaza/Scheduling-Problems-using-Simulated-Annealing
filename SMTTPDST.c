// Single-Machine Total Tardiness Problem with Sequence Dependent Setup Times (SMTTPDST)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_JOBS 20   // Maximum number of jobs
#define MAX_ITER 1000 // Maximum number of iterations
#define INITIAL_TEMP 100.0
#define COOLING_RATE 0.99

// Structure to represent a job
typedef struct {
    int processing_time;
    int due_date;
} Job;

// Function prototypes
void generate_random_instance(Job jobs[], int n);
int calculate_total_tardiness(Job jobs[], int n, int sequence[]);
int evaluate_solution(Job jobs[], int n, int sequence[]);
void copy_sequence(int dest[], int src[], int n);
void simulated_annealing(Job jobs[], int n, int best_sequence[]);

int main() {
    srand(time(NULL));

    int n = 10; // Number of jobs (example: 10 jobs)
    Job jobs[MAX_JOBS];

    // Generate random instance of jobs
    generate_random_instance(jobs, n);

    // Array to store the best sequence found
    int best_sequence[MAX_JOBS];

    // Solve using simulated annealing
    simulated_annealing(jobs, n, best_sequence);

    // Output the best sequence found
    printf("\nBest job sequence:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", best_sequence[i]);
    }
    printf("\n");

    // Calculate and print the total tardiness of the best sequence found
    int total_tardiness = calculate_total_tardiness(jobs, n, best_sequence);
    printf("Total tardiness =  %d\n", total_tardiness);

    return 0;
}

// Function to generate a random instance of jobs
void generate_random_instance(Job jobs[], int n) {
    for (int i = 0; i < n; i++) {
        jobs[i].processing_time = rand() % 20 + 1; // Random processing time between 1 and 20
        jobs[i].due_date = rand() % 50 + 1;       // Random due date between 1 and 50
    }
}

// Function to calculate the total tardiness of a sequence of jobs
int calculate_total_tardiness(Job jobs[], int n, int sequence[]) {
    int completion_time = 0;
    int total_tardiness = 0;

    for (int i = 0; i < n; i++) {
        int job_index = sequence[i];
        completion_time += jobs[job_index].processing_time;
        int tardiness = fmax(completion_time - jobs[job_index].due_date, 0);
        total_tardiness += tardiness;
    }

    return total_tardiness;
}

// Function to evaluate the solution (negative of total tardiness for minimization)
int evaluate_solution(Job jobs[], int n, int sequence[]) {
    return -calculate_total_tardiness(jobs, n, sequence);
}

// Function to copy one sequence to another
void copy_sequence(int dest[], int src[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// Function to perform simulated annealing
void simulated_annealing(Job jobs[], int n, int best_sequence[]) {
    int current_sequence[MAX_JOBS];
    int current_total_tardiness, best_total_tardiness;
    int new_sequence[MAX_JOBS];
    int new_total_tardiness;
    double temperature = INITIAL_TEMP;

    // Initialize the current sequence randomly
    for (int i = 0; i < n; i++) {
        current_sequence[i] = i;
    }
    // Randomly shuffle the initial sequence
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() % (n - i);
        if (i != j) {
            int temp = current_sequence[i];
            current_sequence[i] = current_sequence[j];
            current_sequence[j] = temp;
        }
    }

    // Evaluate the initial solution
    current_total_tardiness = evaluate_solution(jobs, n, current_sequence);
    copy_sequence(best_sequence, current_sequence, n);
    best_total_tardiness = current_total_tardiness;

    // Simulated annealing main loop
    int iteration = 0;
    while (iteration < MAX_ITER && temperature > 0.1) {
        // Generate a new neighbor solution by swapping two jobs in the current sequence
        copy_sequence(new_sequence, current_sequence, n);
        int index1 = rand() % n;
        int index2 = rand() % n;
        while (index1 == index2) {
            index2 = rand() % n;
        }
        int temp = new_sequence[index1];
        new_sequence[index1] = new_sequence[index2];
        new_sequence[index2] = temp;

        // Evaluate the new solution
        new_total_tardiness = evaluate_solution(jobs, n, new_sequence);

        // Accept the new solution if it's better or according to the acceptance probability
        if (new_total_tardiness < current_total_tardiness ||
            exp((new_total_tardiness - current_total_tardiness) / temperature) > (rand() / (double)RAND_MAX)) {
            copy_sequence(current_sequence, new_sequence, n);
            current_total_tardiness = new_total_tardiness;

            // Update the best solution found so far
            if (current_total_tardiness < best_total_tardiness) {
                copy_sequence(best_sequence, current_sequence, n);
                best_total_tardiness = current_total_tardiness;
            }
        }

        // Decrease the temperature
        temperature *= COOLING_RATE;
        iteration++;
    }
}
