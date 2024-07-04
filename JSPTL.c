// Job-Shop Scheduling Problem with Time Lags (JSPTL)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_JOBS 10      // Maximum number of jobs
#define MAX_MACHINES 10  // Maximum number of machines
#define MAX_OPERATIONS 10  // Maximum number of operations per job
#define MAX_ITER 10000   // Maximum iterations for simulated annealing
#define INITIAL_TEMP 100.0  // Initial temperature
#define COOLING_RATE 0.95   // Cooling rate for simulated annealing

// Structure for an operation
typedef struct {
    int machine;   // Machine index
    int processing_time;  // Processing time
} Operation;

// Structure for a job
typedef struct {
    int num_operations;   // Number of operations for this job
    Operation operations[MAX_OPERATIONS];  // Operations of the job
} Job;

// Function to calculate the total completion time (makespan) of a schedule
int calculate_makespan(Job jobs[], int n_jobs) {
    int machine_end_times[MAX_MACHINES] = {0};  // End times of machines
    int job_end_times[MAX_JOBS][MAX_OPERATIONS] = {{0}};  // End times of jobs' operations
    int total_makespan = 0;

    // Calculate completion times for each job's operations
    for (int i = 0; i < n_jobs; i++) {
        int job_makespan = 0;
        for (int j = 0; j < jobs[i].num_operations; j++) {
            int machine = jobs[i].operations[j].machine;
            int processing_time = jobs[i].operations[j].processing_time;

            // Calculate start time of operation j of job i
            int start_time = machine_end_times[machine];

            // Account for time lags if applicable
            if (j > 0) {
                start_time = fmax(start_time, job_end_times[i][j-1]);
            }

            // Calculate end time of operation j of job i
            int end_time = start_time + processing_time;

            // Update job's operation end time
            job_end_times[i][j] = end_time;

            // Update machine's end time
            machine_end_times[machine] = end_time;

            // Update job's total makespan
            job_makespan = fmax(job_makespan, end_time);
        }
        // Update total makespan
        total_makespan += job_makespan;
    }

    return total_makespan;
}

// Function to generate a random initial solution
void generate_initial_solution(Job jobs[], int n_jobs) {
    // Example initialization (you can modify this based on your needs)
    for (int i = 0; i < n_jobs; i++) {
        jobs[i].num_operations = 2;  // Example: each job has 2 operations
        for (int j = 0; j < jobs[i].num_operations; j++) {
            jobs[i].operations[j].machine = j % 3;  // Example: alternate machines M0, M1, M2
            jobs[i].operations[j].processing_time = rand() % 10 + 1;  // Random processing time (1 to 10)
        }
    }
}

// Function to generate a neighbor solution (swap two operations)
void generate_neighbor_solution(Job jobs[], int n_jobs) {
    // Randomly select a job and two different operations within that job
    int job_idx = rand() % n_jobs;
    if (jobs[job_idx].num_operations < 2) return;  // Job should have at least two operations

    int op1_idx = rand() % jobs[job_idx].num_operations;
    int op2_idx = rand() % jobs[job_idx].num_operations;
    while (op2_idx == op1_idx) {
        op2_idx = rand() % jobs[job_idx].num_operations;
    }

    // Swap the machines of the selected operations
    int temp_machine = jobs[job_idx].operations[op1_idx].machine;
    jobs[job_idx].operations[op1_idx].machine = jobs[job_idx].operations[op2_idx].machine;
    jobs[job_idx].operations[op2_idx].machine = temp_machine;
}

// Simulated annealing algorithm
void simulated_annealing(Job jobs[], int n_jobs) {
    Job current_solution[MAX_JOBS];
    Job best_solution[MAX_JOBS];

    // Initialize current solution
    generate_initial_solution(current_solution, n_jobs);
    // Initialize best solution found so far
    // Copy current solution to best solution
    for (int i = 0; i < n_jobs; i++) {
        best_solution[i] = current_solution[i];
    }

    double temperature = INITIAL_TEMP;
    int iter = 0;

    while (temperature > 0.1 && iter < MAX_ITER) {
        // Generate a neighbor solution
        generate_neighbor_solution(current_solution, n_jobs);

        // Calculate makespan for current and neighbor solutions
        int current_makespan = calculate_makespan(current_solution, n_jobs);
        int best_makespan = calculate_makespan(best_solution, n_jobs);

        // Decide whether to accept the neighbor solution
        if (current_makespan < best_makespan) {
            // Accept the neighbor solution
            // Update best solution
            for (int i = 0; i < n_jobs; i++) {
                best_solution[i] = current_solution[i];
            }
        } else {
            // Accept with a probability based on temperature and difference in makespan
            double rand_num = (double)rand() / RAND_MAX;
            double acceptance_prob = exp(-(current_makespan - best_makespan) / temperature);
            if (rand_num < acceptance_prob) {
                // Accept the neighbor solution
                // Update best solution
                for (int i = 0; i < n_jobs; i++) {
                    best_solution[i] = current_solution[i];
                }
            } else {
                // Revert current solution back to previous (no action needed)
            }
        }

        // Decrease temperature
        temperature *= COOLING_RATE;

        iter++;
    }

    // Output the best solution found
    printf("Best Makespan = %d\n", calculate_makespan(best_solution, n_jobs));
    printf("Best Schedule:\n");
    for (int i = 0; i < n_jobs; i++) {
        printf("Job %d: ", i);
        for (int j = 0; j < best_solution[i].num_operations; j++) {
            printf("(M%d, %d) ", best_solution[i].operations[j].machine, best_solution[i].operations[j].processing_time);
        }
        printf("\n");
    }
}

// Main function
int main() {
    srand(time(NULL));  // Seed random number generator

    int n_jobs = 5;  // Number of jobs
    Job jobs[MAX_JOBS];  // Array to store jobs

    // Example initialization of jobs (you can modify this based on your needs)
    for (int i = 0; i < n_jobs; i++) {
        jobs[i].num_operations = 3;  // Example: each job has 3 operations
        for (int j = 0; j < jobs[i].num_operations; j++) {
            jobs[i].operations[j].machine = j % 3;  // Example: alternate machines M0, M1, M2
            jobs[i].operations[j].processing_time = rand() % 10 + 1;  // Random processing time (1 to 10)
        }
    }

    // Apply simulated annealing to solve the JSPTL problem
    simulated_annealing(jobs, n_jobs);

    return 0;
}
