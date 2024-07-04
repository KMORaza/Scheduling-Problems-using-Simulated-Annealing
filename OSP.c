// Open-Shop Scheduling Problem (OSP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_JOBS 10  // Maximum number of jobs
#define MAX_MACHINES 10  // Maximum number of machines
// Function to calculate the makespan of a given solution
int calculate_makespan(int **schedule, int n, int m, int *processing_times) {
    int *finish_time = (int *)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++) {
        finish_time[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            finish_time[j] = fmax(finish_time[j], schedule[i][j]) + processing_times[i * m + j];
        }
    }
    int makespan = 0;
    for (int i = 0; i < m; i++) {
        if (finish_time[i] > makespan) {
            makespan = finish_time[i];
        }
    }
    free(finish_time);
    return makespan;
}
// Function to generate a random initial solution
void generate_initial_solution(int **schedule, int n, int m, int *processing_times) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            schedule[i][j] = rand() % (n * m);  // Random initialization
        }
    }
}
// Function to copy a schedule from source to destination
void copy_schedule(int **source, int **destination, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            destination[i][j] = source[i][j];
        }
    }
}
// Function to perform simulated annealing
void simulated_annealing(int **schedule, int n, int m, int *processing_times, double initial_temperature, double cooling_rate) {
    int **current_solution = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        current_solution[i] = (int *)malloc(m * sizeof(int));
    }
    int **best_solution = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        best_solution[i] = (int *)malloc(m * sizeof(int));
    }
    generate_initial_solution(schedule, n, m, processing_times);
    copy_schedule(schedule, current_solution, n, m);
    copy_schedule(schedule, best_solution, n, m);
    int current_makespan = calculate_makespan(current_solution, n, m, processing_times);
    int best_makespan = current_makespan;
    double temperature = initial_temperature;
    srand(time(NULL));
    while (temperature > 1.0) {
        for (int i = 0; i < 100; i++) {  // Number of iterations at each temperature
            int job = rand() % n;
            int machine1 = rand() % m;
            int machine2 = rand() % m;
            int old_start_time = current_solution[job][machine1];
current_solution[job][machine1] = current_solution[job][machine2];
current_solution[job][machine2] = old_start_time;
            int new_makespan = calculate_makespan(current_solution, n, m, processing_times);
            int delta_makespan = new_makespan - current_makespan;
            if (delta_makespan < 0 || exp(-delta_makespan / temperature) > ((double)rand() / RAND_MAX)) {
current_makespan = new_makespan;
                if (current_makespan < best_makespan) {
best_makespan = current_makespan;
copy_schedule(current_solution, best_solution, n, m);
                }
            } else {
                // Revert back to the previous solution
current_solution[job][machine2] = current_solution[job][machine1];    current_solution[job][machine1] = old_start_time;
            }
        }
        temperature *= cooling_rate;  // Cooling the temperature
    }
    // Copy the best solution found to the output schedule
copy_schedule(best_solution, schedule, n, m);
    // Free dynamically allocated memory
    for (int i = 0; i < n; i++) {
free(current_solution[i]);
        free(best_solution[i]);
    }
    free(current_solution);
    free(best_solution);
}
int main() {
    // Problem parameters
    int n = 5;  // Number of jobs
    int m = 3;  // Number of machines
    double initial_temperature = 100.0;
    double cooling_rate = 0.95;
    // Processing times (example)
    int processing_times[] = {
        3, 2, 5,
        1, 4, 2,
        4, 3, 6,
        2, 5, 1,
        5, 2, 3
    };
    // Allocate memory for the schedule
    int **schedule = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        schedule[i] = (int *)malloc(m * sizeof(int));
    }
simulated_annealing(schedule, n, m, processing_times, initial_temperature, cooling_rate);
    // Print the optimal schedule found
    printf("\nOptimal Schedule:\n");
    for (int i = 0; i < n; i++) {
        printf("Job %d:", i + 1);
        for (int j = 0; j < m; j++) {
            printf(" %d", schedule[i][j]);
        }
        printf("\n");
    }
    // Free dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(schedule[i]);
    }
    free(schedule);
    return 0;
}