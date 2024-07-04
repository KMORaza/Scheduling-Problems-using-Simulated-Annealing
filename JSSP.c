// Job Selection and Sequencing Problem (JSSP) 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_JOBS 5
#define MAX_MACHINES 3
#define MAX_ITERATIONS 10000
#define INITIAL_TEMPERATURE 100.0
#define COOLING_RATE 0.95

// Define a job operation structure
typedef struct {
    int machine;
    int processing_time;
} Operation;

// Define a job structure
typedef struct {
    Operation operations[MAX_MACHINES];
    int num_operations;
} Job;

// Function prototypes
void initialize_jobs(Job jobs[], int num_jobs);
int calculate_completion_time(Job jobs[], int sequence[]);
void print_jobs(Job jobs[], int num_jobs);
int simulated_annealing(Job jobs[], int num_jobs, int best_sequence[]);

int main() {
    Job jobs[MAX_JOBS];
    int best_sequence[MAX_JOBS];
    int i;

    // Initialize random seed
    srand(time(NULL));

    // Initialize jobs with random processing times
    initialize_jobs(jobs, MAX_JOBS);

    // Print initial jobs data (for verification)
    printf("Initial jobs data:\n");
    print_jobs(jobs, MAX_JOBS);

    // Solve JSSP using Simulated Annealing
    int min_completion_time = simulated_annealing(jobs, MAX_JOBS, best_sequence);

    // Output the best sequence found
    printf("\nBest job sequence found:\n");
    for (i = 0; i < MAX_JOBS; i++) {
        printf("Job %d ", best_sequence[i]);
    }
    printf("\n");
    printf("Minimized completion time = %d\n", min_completion_time);

    return 0;
}

// Initialize jobs with random processing times
void initialize_jobs(Job jobs[], int num_jobs) {
    int i, j, k;
    for (i = 0; i < num_jobs; i++) {
        jobs[i].num_operations = rand() % MAX_MACHINES + 1; // Random number of operations per job

        // Initialize operations
        for (j = 0; j < jobs[i].num_operations; j++) {
            jobs[i].operations[j].machine = j % MAX_MACHINES; // Assign machines cyclically
            jobs[i].operations[j].processing_time = rand() % 10 + 1;  // Random processing time between 1 to 10 units
        }

        // Shuffle operations to simulate random sequences
        for (j = jobs[i].num_operations - 1; j > 0; j--) {
            k = rand() % (j + 1);
            Operation temp = jobs[i].operations[j];
            jobs[i].operations[j] = jobs[i].operations[k];
            jobs[i].operations[k] = temp;
        }
    }
}

// Print jobs data (for verification)
void print_jobs(Job jobs[], int num_jobs) {
    int i, j;
    for (i = 0; i < num_jobs; i++) {
        printf("Job %d (Operations: %d):\n", i, jobs[i].num_operations);
        for (j = 0; j < jobs[i].num_operations; j++) {
            printf("  Operation %d: Machine %d, Processing Time %d\n", j, jobs[i].operations[j].machine, jobs[i].operations[j].processing_time);
        }
    }
}

// Calculate total completion time for a given job sequence
int calculate_completion_time(Job jobs[], int sequence[]) {
    int completion_times[MAX_MACHINES] = {0};
    int job_start_time[MAX_JOBS] = {0};
    int machine_available_time[MAX_MACHINES] = {0};
    int i, j;
    int max_completion_time = 0;

    // Calculate completion time for each job in sequence
    for (i = 0; i < MAX_JOBS; i++) {
        int job_index = sequence[i];
        int current_job_completion_time = 0;

        for (j = 0; j < jobs[job_index].num_operations; j++) {
            int machine = jobs[job_index].operations[j].machine;
            int processing_time = jobs[job_index].operations[j].processing_time;

            // Calculate start time of the current operation
            int start_time = (machine_available_time[machine] > job_start_time[job_index]) ? machine_available_time[machine] : job_start_time[job_index];

            // Update machine available time
            machine_available_time[machine] = start_time + processing_time;

            // Update job start time for the next operation
            job_start_time[job_index] = start_time + processing_time;

            // Update completion time for the job
            if (start_time + processing_time > current_job_completion_time) {
                current_job_completion_time = start_time + processing_time;
            }
        }

        // Update completion times for machines
        if (current_job_completion_time > max_completion_time) {
            max_completion_time = current_job_completion_time;
        }
    }

    return max_completion_time;
}

// Simulated Annealing function to find the optimal job sequence
int simulated_annealing(Job jobs[], int num_jobs, int best_sequence[]) {
    int current_sequence[MAX_JOBS];
    int new_sequence[MAX_JOBS];
    int current_completion_time, new_completion_time;
    int best_completion_time;
    double temperature = INITIAL_TEMPERATURE;
    double acceptance_probability;
    int iteration, i, j;
    int delta;

    // Initialize current sequence randomly
    for (i = 0; i < num_jobs; i++) {
        current_sequence[i] = i;
    }
    // Shuffle the initial sequence
    for (i = num_jobs - 1; i > 0; i--) {
        j = rand() % (i + 1);
        int temp = current_sequence[i];
        current_sequence[i] = current_sequence[j];
        current_sequence[j] = temp;
    }

    // Calculate initial completion time
    current_completion_time = calculate_completion_time(jobs, current_sequence);
    best_completion_time = current_completion_time;
    // Copy the best sequence found so far
    for (i = 0; i < num_jobs; i++) {
        best_sequence[i] = current_sequence[i];
    }

    // Simulated Annealing loop
    for (iteration = 1; iteration <= MAX_ITERATIONS; iteration++) {
        // Generate a neighboring solution by swapping two jobs in the current sequence
        for (i = 0; i < num_jobs; i++) {
            new_sequence[i] = current_sequence[i];
        }
        // Randomly choose two indices to swap
        i = rand() % num_jobs;
        j = rand() % num_jobs;
        // Swap
        int temp = new_sequence[i];
        new_sequence[i] = new_sequence[j];
        new_sequence[j] = temp;

        // Calculate the new completion time
        new_completion_time = calculate_completion_time(jobs, new_sequence);

        // Calculate delta E (change in objective function)
        delta = new_completion_time - current_completion_time;

        // Decide whether to accept the new solution
        if (delta < 0) {
            // If new solution is better, accept it
            for (i = 0; i < num_jobs; i++) {
                current_sequence[i] = new_sequence[i];
            }
            current_completion_time = new_completion_time;

            // Update the best solution found so far
            if (current_completion_time < best_completion_time) {
                best_completion_time = current_completion_time;
                for (i = 0; i < num_jobs; i++) {
                    best_sequence[i] = current_sequence[i];
                }
            }
        } else {
            // If new solution is worse, accept it with a probability based on temperature
            acceptance_probability = exp(-delta / temperature);
            double rand_num = ((double) rand() / RAND_MAX);
            if (rand_num < acceptance_probability) {
                for (i = 0; i < num_jobs; i++) {
                    current_sequence[i] = new_sequence[i];
                }
                current_completion_time = new_completion_time;
            }
        }

        // Cool the temperature
        temperature *= COOLING_RATE;
    }

    return best_completion_time;
}
