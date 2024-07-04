// Job-Shop Scheduling Problem with Sequence Dependent Setup Times (JSPSDST)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_JOBS 10
#define MAX_MACHINES 10

// Example problem constants
const int num_jobs = 5;
const int num_machines = 5;
int processing_times[MAX_JOBS][MAX_MACHINES] = {
    {5, 7, 3, 6, 4},
    {6, 4, 7, 3, 5},
    {4, 5, 6, 3, 7},
    {3, 6, 5, 4, 7},
    {5, 4, 3, 6, 7}
};
int setup_times[MAX_MACHINES][MAX_MACHINES] = {
    {0, 3, 2, 4, 3},
    {3, 0, 1, 2, 4},
    {2, 1, 0, 3, 2},
    {4, 2, 3, 0, 1},
    {3, 4, 2, 1, 0}
};

// Function prototypes
void generate_initial_solution(int current_solution[]);
int calculate_makespan(int solution[]);
void copy_solution(int dest[], int src[]);
void simulated_annealing(int current_solution[], int best_solution[]);

int main() {
    srand(time(NULL));

    int current_solution[MAX_JOBS * MAX_MACHINES];
    int best_solution[MAX_JOBS * MAX_MACHINES];

    generate_initial_solution(current_solution);
    copy_solution(best_solution, current_solution);

    simulated_annealing(current_solution, best_solution);

    printf("Best Solution Sequence:\n");
    for (int i = 0; i < num_jobs * num_machines; ++i) {
        printf("%d ", best_solution[i]);
    }
    printf("\n");

    printf("Best Makespan: %d\n", calculate_makespan(best_solution));

    return 0;
}

void generate_initial_solution(int current_solution[]) {
    // Generate a random initial solution (random permutation of machine orders)
    int machines[MAX_MACHINES];
    for (int i = 0; i < num_machines; ++i) {
        machines[i] = i;
    }

    for (int i = 0; i < num_jobs; ++i) {
        // Randomly shuffle machine order for each job
        for (int j = 0; j < num_machines; ++j) {
            int k = rand() % num_machines;
            int temp = machines[j];
            machines[j] = machines[k];
            machines[k] = temp;
        }

        // Copy shuffled machine order into current solution
        for (int j = 0; j < num_machines; ++j) {
            current_solution[i * num_machines + j] = machines[j];
        }
    }
}

int calculate_makespan(int solution[]) {
    int completion_times[MAX_JOBS][MAX_MACHINES] = {0};
    int makespan = 0;

    for (int i = 0; i < num_jobs; ++i) {
        for (int j = 0; j < num_machines; ++j) {
            int machine = solution[i * num_machines + j];
            int processing_time = processing_times[i][machine];
            int setup_time = (j == 0) ? 0 : setup_times[solution[i * num_machines + (j - 1)]][machine];
            completion_times[i][machine] = (j == 0) ? processing_time : completion_times[i][solution[i * num_machines + (j - 1)]] + setup_time + processing_time;

            if (completion_times[i][machine] > makespan) {
                makespan = completion_times[i][machine];
            }
        }
    }

    return makespan;
}

void copy_solution(int dest[], int src[]) {
    for (int i = 0; i < num_jobs * num_machines; ++i) {
        dest[i] = src[i];
    }
}

double acceptance_probability(int current_cost, int new_cost, double temperature) {
    if (new_cost < current_cost) {
        return 1.0;
    }
    return exp((current_cost - new_cost) / temperature);
}

void simulated_annealing(int current_solution[], int best_solution[]) {
    double temperature = 1000.0;
    double cooling_rate = 0.99;
    int current_cost = calculate_makespan(current_solution);
    int best_cost = current_cost;
    int iteration = 0;

    while (temperature > 1.0) {
        int neighbor_solution[MAX_JOBS * MAX_MACHINES];
        copy_solution(neighbor_solution, current_solution);

        // Generate a neighboring solution (swap two random positions)
        int pos1 = rand() % (num_jobs * num_machines);
        int pos2 = rand() % (num_jobs * num_machines);
        int temp = neighbor_solution[pos1];
        neighbor_solution[pos1] = neighbor_solution[pos2];
        neighbor_solution[pos2] = temp;

        int neighbor_cost = calculate_makespan(neighbor_solution);

        // Decide if we should accept the neighbor
        if (acceptance_probability(current_cost, neighbor_cost, temperature) > ((double)rand() / RAND_MAX)) {
            copy_solution(current_solution, neighbor_solution);
            current_cost = neighbor_cost;
        }

        // Update the best solution found so far
        if (current_cost < best_cost) {
            best_cost = current_cost;
            copy_solution(best_solution, current_solution);
        }

        // Cool the temperature
        temperature *= cooling_rate;
        iteration++;
    }
}
