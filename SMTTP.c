// Single Machine Total Tardiness Problem (SMTTP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_JOBS 20    // Maximum number of jobs
#define MAX_ITER 10000 // Maximum number of iterations for SA
// Structure to represent a job
typedef struct {
    int processing_time;
    int due_date;
} Job;
// Function to calculate total tardiness of a sequence of jobs
int calculate_total_tardiness(Job *jobs, int *sequence, int num_jobs) {
    int current_time = 0;
    int total_tardiness = 0;
    for (int i = 0; i < num_jobs; i++) {
        int job_index = sequence[i];
        current_time += jobs[job_index].processing_time;
        int tardiness = fmax(0, current_time - jobs[job_index].due_date);
        total_tardiness += tardiness;
    }
    return total_tardiness;
}
// Function to generate a random initial solution (random permutation of jobs)
void generate_random_solution(int *sequence, int num_jobs) {
    for (int i = 0; i < num_jobs; i++) {
        sequence[i] = i;
    }
    for (int i = num_jobs - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = sequence[i];
        sequence[i] = sequence[j];
        sequence[j] = temp;
    }
}
// Simulated Annealing function to minimize total tardiness
void simulated_annealing(Job *jobs, int *best_sequence, int num_jobs) {
    int current_sequence[MAX_JOBS];
    int current_tardiness, best_tardiness;
    double temperature = 100.0;
    double cooling_rate = 0.99;
    int iteration = 0;
    // Generate a random initial solution
generate_random_solution(current_sequence, num_jobs);
    current_tardiness = calculate_total_tardiness(jobs, current_sequence, num_jobs);
    best_tardiness = current_tardiness;
    for (int i = 0; i < num_jobs; i++) {
        best_sequence[i] = current_sequence[i];
    }
    // Perform simulated annealing
    while (temperature > 1.0 && iteration < MAX_ITER) {
        int new_sequence[MAX_JOBS];
        for (int i = 0; i < num_jobs; i++) {
            new_sequence[i] = current_sequence[i];
        }
        // Perform a small change to get a new neighboring solution
        int rand_pos1 = rand() % num_jobs;
        int rand_pos2 = rand() % num_jobs;
        int temp = new_sequence[rand_pos1];
        new_sequence[rand_pos1] = new_sequence[rand_pos2];
        new_sequence[rand_pos2] = temp;
        // Calculate the tardiness of the new solution
        int new_tardiness = calculate_total_tardiness(jobs, new_sequence, num_jobs);
        // Decide whether to accept the new solution
        if (new_tardiness < current_tardiness || exp(-(new_tardiness - current_tardiness) / temperature) > ((double)rand() / RAND_MAX)) {
            for (int i = 0; i < num_jobs; i++) {
current_sequence[i] = new_sequence[i];
            }
current_tardiness = new_tardiness;
            // Update the best solution found so far
            if (current_tardiness < best_tardiness) {
                best_tardiness = current_tardiness;
                for (int i = 0; i < num_jobs; i++) {
                    best_sequence[i] = current_sequence[i];
                }
            }
        }
        // Cool the temperature
        temperature *= cooling_rate;
        iteration++;
    }
    printf("Best sequence found:\n");
    for (int i = 0; i < num_jobs; i++) {
        printf("%d ", best_sequence[i]);
    }
    printf("\nTotal tardiness = %d\n", best_tardiness);
}
int main() {
    srand(time(NULL));
    // Example data: processing times and due dates for jobs
    Job jobs[MAX_JOBS] = {
        {3, 10},
        {5, 15},
        {2, 7},
        {7, 20},
        {4, 11}
    };
    int num_jobs = 5;
    int best_sequence[MAX_JOBS];
    // Solve the problem using simulated annealing
    simulated_annealing(jobs, best_sequence, num_jobs);
    return 0;
}
