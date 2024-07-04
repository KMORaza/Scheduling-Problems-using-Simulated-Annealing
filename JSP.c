// Job-Shop Scheduling Problem (JSP)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// Define structures
typedef struct {
    int machine;
    int duration;
} Operation;
typedef struct {
    int num_operations;
    Operation *operations;
} Job;
// Global variables
int num_jobs = 3;
int num_machines = 3;
Job jobs[3];
// Function prototypes
void initialize_data();
void initialize_schedule(int **schedule);
int calculate_makespan(int **schedule);
void copy_schedule(int **source, int **destination);
void free_memory(int **schedule);
void simulated_annealing();
// Function to initialize data (hardcoded for demonstration)
void initialize_data() {
    // Example data initialization
    jobs[0].num_operations = 2;
    jobs[0].operations = (Operation *)malloc(2 * sizeof(Operation));
jobs[0].operations[0].machine = 1;
jobs[0].operations[0].duration = 2;
jobs[0].operations[1].machine = 2;
jobs[0].operations[1].duration = 1;
    jobs[1].num_operations = 2;
    jobs[1].operations = (Operation *)malloc(2 * sizeof(Operation));
jobs[1].operations[0].machine = 2;
jobs[1].operations[0].duration = 2;
jobs[1].operations[1].machine = 1;
jobs[1].operations[1].duration = 3;
    jobs[2].num_operations = 2;
    jobs[2].operations = (Operation *)malloc(2 * sizeof(Operation));
jobs[2].operations[0].machine = 1;
jobs[2].operations[0].duration = 1;
jobs[2].operations[1].machine = 3;
jobs[2].operations[1].duration = 2;
}
// Function to initialize a schedule randomly
void initialize_schedule(int **schedule) {
    for (int i = 0; i < num_jobs; i++) {
        for (int j = 0; j < jobs[i].num_operations; j++) {
            schedule[i][j + 1] = rand() % num_machines + 1; // Randomly assign machines
        }
    }
}
// Function to calculate makespan of a schedule
int calculate_makespan(int **schedule) {
    int *machine_end_times = (int *)malloc(num_machines * sizeof(int));
    for (int i = 0; i < num_machines; i++) {
        machine_end_times[i] = 0;
    }
    for (int i = 0; i < num_jobs; i++) {
        int job_end_time = 0;
        for (int j = 0; j < jobs[i].num_operations; j++) {
            int current_machine = schedule[i][j + 1] - 1; // machine index
            int current_duration = jobs[i].operations[j].duration;
            int start_time = machine_end_times[current_machine];
            int end_time = start_time + current_duration;
            if (end_time > job_end_time) {
                job_end_time = end_time;
            }
    machine_end_times[current_machine] = end_time;
        }
    }

    int makespan = 0;
    for (int i = 0; i < num_machines; i++) {
        if (machine_end_times[i] > makespan) {
            makespan = machine_end_times[i];
        }
    }
    free(machine_end_times);
    return makespan;
}
// Function to copy one schedule to another
void copy_schedule(int **source, int **destination) {
    for (int i = 0; i < num_jobs; i++) {
        for (int j = 0; j <= jobs[i].num_operations; j++) {
            destination[i][j] = source[i][j];
        }
    }
}
// Function to free memory allocated for a schedule
void free_memory(int **schedule) {
    for (int i = 0; i < num_jobs; i++) {
        free(schedule[i]);
    }
    free(schedule);
}
// Simulated Annealing function
void simulated_annealing() {
    initialize_data();
    // Initialize the best schedule found
    int **best_schedule = (int **)malloc(num_jobs * sizeof(int *));
    for (int i = 0; i < num_jobs; i++) {
        best_schedule[i] = (int *)malloc((jobs[i].num_operations + 1) * sizeof(int));
    }
    int **current_schedule = (int **)malloc(num_jobs * sizeof(int *));
    for (int i = 0; i < num_jobs; i++) {
        current_schedule[i] = (int *)malloc((jobs[i].num_operations + 1) * sizeof(int));
    }
initialize_schedule(current_schedule);
copy_schedule(current_schedule, best_schedule);
    int current_makespan = calculate_makespan(current_schedule);
    int best_makespan = current_makespan;
    // Simulated Annealing parameters
    double temperature = 1000.0;
    double cooling_rate = 0.95;
    while (temperature > 1.0) {
        // Generate a neighboring solution
        int job1 = rand() % num_jobs;
        int job2 = rand() % num_jobs;
        while (job1 == job2) {
            job2 = rand() % num_jobs;
        }
        int op1 = rand() % jobs[job1].num_operations;
        int op2 = rand() % jobs[job2].num_operations;
        // Swap operations
        int temp = current_schedule[job1][op1 + 1];
        current_schedule[job1][op1 + 1] = current_schedule[job2][op2 + 1];
        current_schedule[job2][op2 + 1] = temp;
        // Calculate new makespan
        int new_makespan = calculate_makespan(current_schedule);
        // Calculate difference in makespan
        int delta_makespan = new_makespan - current_makespan;
        // Decide whether to move to the new solution
        if (delta_makespan < 0 || exp(-delta_makespan / temperature) > ((double)rand() / RAND_MAX)) {
            current_makespan = new_makespan;
            if (current_makespan < best_makespan) {
                best_makespan = current_makespan;
                copy_schedule(current_schedule, best_schedule);
            }
        } else {
            // Revert to the previous solution
        current_schedule[job2][op2 + 1] = current_schedule[job1][op1 + 1];
            current_schedule[job1][op1 + 1] = temp;
        }
        // Cooling process
        temperature *= cooling_rate;
    }
    // Output the best schedule found
    printf("Best MakespanMakespan = %d\n", best_makespan);
    printf("Best Schedule:\n");
    for (int i = 0; i < num_jobs; i++) {
        printf("Job %d: ", i + 1);
        for (int j = 0; j < jobs[i].num_operations; j++) {
            printf("(%d,%d) ", best_schedule[i][j + 1], jobs[i].operations[j].duration);
        }
        printf("\n");
    }
    // Free allocated memory
    free_memory(best_schedule);
free_memory(current_schedule);
    for (int i = 0; i < num_jobs; i++) {
    free(jobs[i].operations);
    }
}
// Main function
int main() {
    srand(time(NULL));
    simulated_annealing();
    return 0;
}