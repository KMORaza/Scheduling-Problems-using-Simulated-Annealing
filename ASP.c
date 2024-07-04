// Assembly Sequence Planning (ASP)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX_PARTS 10
#define MAX_OPERATIONS 100
#define MAX_TEMP 10000.0
#define MIN_TEMP 0.001
#define COOLING_FACTOR 0.95
#define MAX_ITERATIONS 1000

// Function prototypes
int calculate_cost(int operations[], int num_operations);
void generate_initial_solution(int operations[], int num_operations);
void generate_neighbour_solution(int operations[], int num_operations);
double acceptance_probability(int cost_current, int cost_neighbour, double temperature);

int main() {
    srand(time(NULL));

    // Example data: number of operations
    int num_operations = 6;
    int operations[MAX_OPERATIONS];

    // Generate initial random solution
    generate_initial_solution(operations, num_operations);

    // Simulated Annealing parameters
    double temperature = MAX_TEMP;
    int current_solution[MAX_OPERATIONS];
    int best_solution[MAX_OPERATIONS];
    int cost_current, cost_best, cost_neighbour;
    int iterations = 0;

    // Initial cost calculations
    cost_current = calculate_cost(operations, num_operations);
    cost_best = cost_current;
    memcpy(best_solution, operations, sizeof(int) * num_operations);

    while (temperature > MIN_TEMP && iterations < MAX_ITERATIONS) {
        generate_neighbour_solution(operations, num_operations);
        cost_neighbour = calculate_cost(operations, num_operations);

        // Decide if we should accept the neighbour
        if (acceptance_probability(cost_current, cost_neighbour, temperature) > ((double)rand() / RAND_MAX)) {
            // Accept the neighbour solution
            memcpy(current_solution, operations, sizeof(int) * num_operations);
            cost_current = cost_neighbour;

            // Check if this is the best solution found so far
            if (cost_current < cost_best) {
                cost_best = cost_current;
                memcpy(best_solution, current_solution, sizeof(int) * num_operations);
            }
        }

        // Cool down the temperature
        temperature *= COOLING_FACTOR;
        iterations++;
    }

    // Output the best solution found
    printf("Best solution found:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d ", best_solution[i]);
    }
    printf("\nCost = %d\n", cost_best);

    return 0;
}

// Calculate cost of a given sequence of operations
int calculate_cost(int operations[], int num_operations) {
    // Example cost function: sum of operations
    int cost = 0;
    for (int i = 0; i < num_operations; i++) {
        cost += operations[i];
    }
    return cost;
}

// Generate an initial random solution
void generate_initial_solution(int operations[], int num_operations) {
    // Example: randomly generate operations
    for (int i = 0; i < num_operations; i++) {
        operations[i] = rand() % MAX_PARTS + 1; // Example: operations are parts to be assembled
    }
}

// Generate a neighbour solution (swap two random operations)
void generate_neighbour_solution(int operations[], int num_operations) {
    // Example: swap two random operations
    int index1 = rand() % num_operations;
    int index2 = rand() % num_operations;
    int temp = operations[index1];
    operations[index1] = operations[index2];
    operations[index2] = temp;
}

// Calculate acceptance probability based on current and neighbour costs
double acceptance_probability(int cost_current, int cost_neighbour, double temperature) {
    // Example: using a simulated annealing acceptance probability function
    if (cost_neighbour < cost_current) {
        return 1.0;
    }
    return exp((cost_current - cost_neighbour) / temperature);
}
