// Cyclic Scheduling Problem (CSP) 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_TASKS 20
#define MAX_DAYS 7
#define MAX_ITERATIONS 10000
#define INITIAL_TEMPERATURE 100.0
#define COOLING_RATE 0.99
#define EPSILON 0.001

// Function prototypes
void generate_initial_solution(int solution[MAX_DAYS][MAX_TASKS], int num_tasks);
int calculate_cost(int solution[MAX_DAYS][MAX_TASKS], int num_tasks);
void copy_solution(int dest[MAX_DAYS][MAX_TASKS], int src[MAX_DAYS][MAX_TASKS], int num_tasks);
void print_solution(int solution[MAX_DAYS][MAX_TASKS], int num_tasks);
double acceptance_probability(int cost, int new_cost, double temperature);
void simulated_annealing(int solution[MAX_DAYS][MAX_TASKS], int num_tasks);

int main() {
    int solution[MAX_DAYS][MAX_TASKS];
    int num_tasks;

    // Number of tasks to schedule (user input or random generation)
    printf("Enter the number of tasks to schedule (up to %d): ", MAX_TASKS);
    scanf("%d", &num_tasks);

    if (num_tasks <= 0 || num_tasks > MAX_TASKS) {
        printf("Invalid number of tasks. Exiting...\n");
        return 1;
    }

    // Seed for random number generation
    srand(time(NULL));

    // Generate initial solution randomly
    generate_initial_solution(solution, num_tasks);

    // Print initial solution
    printf("Initial Solution:\n");
    print_solution(solution, num_tasks);
    printf("Cost = %d\n", calculate_cost(solution, num_tasks));

    // Apply simulated annealing
    simulated_annealing(solution, num_tasks);

    // Print final solution
    printf("\nOptimal Solution:\n");
    print_solution(solution, num_tasks);
    printf("Cost = %d\n", calculate_cost(solution, num_tasks));

    return 0;
}

// Generate initial solution randomly
void generate_initial_solution(int solution[MAX_DAYS][MAX_TASKS], int num_tasks) {
    for (int day = 0; day < MAX_DAYS; day++) {
        for (int task = 0; task < num_tasks; task++) {
            solution[day][task] = rand() % 2; // 0 or 1 representing whether task is scheduled
        }
    }
}

// Calculate cost (fitness) of a solution (lower is better)
int calculate_cost(int solution[MAX_DAYS][MAX_TASKS], int num_tasks) {
    int cost = 0;
    // Simplified cost function: penalize non-zero entries in the matrix
    for (int day = 0; day < MAX_DAYS; day++) {
        for (int task = 0; task < num_tasks; task++) {
            if (solution[day][task] != 0) {
                cost++;
            }
        }
    }
    return cost;
}

// Copy solution from source to destination
void copy_solution(int dest[MAX_DAYS][MAX_TASKS], int src[MAX_DAYS][MAX_TASKS], int num_tasks) {
    for (int day = 0; day < MAX_DAYS; day++) {
        for (int task = 0; task < num_tasks; task++) {
            dest[day][task] = src[day][task];
        }
    }
}

// Print the solution matrix
void print_solution(int solution[MAX_DAYS][MAX_TASKS], int num_tasks) {
    for (int day = 0; day < MAX_DAYS; day++) {
        printf("Day %d: ", day + 1);
        for (int task = 0; task < num_tasks; task++) {
            printf("%d ", solution[day][task]);
        }
        printf("\n");
    }
}

// Calculate acceptance probability
double acceptance_probability(int cost, int new_cost, double temperature) {
    if (new_cost < cost) {
        return 1.0; // Accept the new solution
    }
    return exp((cost - new_cost) / temperature);
}

// Simulated annealing algorithm
void simulated_annealing(int solution[MAX_DAYS][MAX_TASKS], int num_tasks) {
    int current_solution[MAX_DAYS][MAX_TASKS];
    copy_solution(current_solution, solution, num_tasks);

    int best_solution[MAX_DAYS][MAX_TASKS];
    copy_solution(best_solution, solution, num_tasks);

    int current_cost = calculate_cost(current_solution, num_tasks);
    int best_cost = current_cost;

    double temperature = INITIAL_TEMPERATURE;

    for (int iter = 0; iter < MAX_ITERATIONS && temperature > EPSILON; iter++) {
        // Generate a neighbor solution (flip a random task on a random day)
        int day = rand() % MAX_DAYS;
        int task = rand() % num_tasks;

        // Flip the task (0 to 1 or 1 to 0)
        current_solution[day][task] = 1 - current_solution[day][task];

        // Calculate the cost of the new solution
        int new_cost = calculate_cost(current_solution, num_tasks);

        // Decide whether to accept the new solution
        double probability = acceptance_probability(current_cost, new_cost, temperature);

        if (probability > (double) rand() / RAND_MAX) {
            // Accept the new solution
            current_cost = new_cost;
            if (current_cost < best_cost) {
                best_cost = current_cost;
                copy_solution(best_solution, current_solution, num_tasks);
            }
        } else {
            // Revert back to the previous solution
            current_solution[day][task] = 1 - current_solution[day][task]; // Flip back
        }

        // Cool down the temperature
        temperature *= COOLING_RATE;
    }

    // Copy the best solution found
    copy_solution(solution, best_solution, num_tasks);
}
