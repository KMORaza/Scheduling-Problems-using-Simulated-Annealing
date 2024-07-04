// Time-Indexed Scheduling Problem (TISP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NUM_ACTIVITIES 5
#define TIME_HORIZON 24 // Assuming a 24-hour time horizon

// Activity structure
typedef struct {
    int start_time;
    int end_time;
} Activity;

// Function prototypes
void initialize_solution(Activity solution[]);
double evaluate_solution(Activity solution[]);
void generate_neighbor(Activity solution[], Activity neighbor[]);
void copy_solution(Activity source[], Activity destination[]);
void print_solution(Activity solution[]);
double acceptance_probability(double current_cost, double new_cost, double temperature);

int main() {
    srand(time(NULL)); // Initialize random seed

    Activity current_solution[NUM_ACTIVITIES];
    Activity neighbor_solution[NUM_ACTIVITIES];

    double current_cost, new_cost;
    double temperature = 1000.0; // Initial temperature
    double cooling_rate = 0.95; // Cooling rate

    initialize_solution(current_solution);
    current_cost = evaluate_solution(current_solution);

    while (temperature > 1.0) {
        generate_neighbor(current_solution, neighbor_solution);
        new_cost = evaluate_solution(neighbor_solution);

        if (new_cost < current_cost) {
            copy_solution(neighbor_solution, current_solution);
            current_cost = new_cost;
        } else {
            double probability = acceptance_probability(current_cost, new_cost, temperature);
            if (rand() / (double)RAND_MAX < probability) {
                copy_solution(neighbor_solution, current_solution);
                current_cost = new_cost;
            }
        }

        temperature *= cooling_rate;
    }

    printf("Solution:\n");
    print_solution(current_solution);
    printf("Cost = %.2f\n", current_cost);

    return 0;
}

// Initialize the solution with random start and end times
void initialize_solution(Activity solution[]) {
    for (int i = 0; i < NUM_ACTIVITIES; i++) {
        solution[i].start_time = rand() % (TIME_HORIZON + 1);
        solution[i].end_time = solution[i].start_time + (rand() % (TIME_HORIZON - solution[i].start_time + 1));
    }
}

// Evaluate the cost of the solution (fitness function)
double evaluate_solution(Activity solution[]) {
    double cost = 0.0;
    // Example cost function: total duration of all activities
    for (int i = 0; i < NUM_ACTIVITIES; i++) {
        cost += solution[i].end_time - solution[i].start_time;
    }
    return cost;
}

// Generate a neighboring solution
void generate_neighbor(Activity solution[], Activity neighbor[]) {
    copy_solution(solution, neighbor);
    // Perturb the neighbor solution (e.g., swap start and end times of a random activity)
    int index = rand() % NUM_ACTIVITIES;
    neighbor[index].start_time = rand() % (TIME_HORIZON + 1);
    neighbor[index].end_time = neighbor[index].start_time + (rand() % (TIME_HORIZON - neighbor[index].start_time + 1));
}

// Copy one solution to another
void copy_solution(Activity source[], Activity destination[]) {
    for (int i = 0; i < NUM_ACTIVITIES; i++) {
        destination[i].start_time = source[i].start_time;
        destination[i].end_time = source[i].end_time;
    }
}

// Print the solution
void print_solution(Activity solution[]) {
    for (int i = 0; i < NUM_ACTIVITIES; i++) {
        printf("Activity %d: Start=%d, End=%d\n", i + 1, solution[i].start_time, solution[i].end_time);
    }
}

// Calculate acceptance probability for simulated annealing
double acceptance_probability(double current_cost, double new_cost, double temperature) {
    if (new_cost < current_cost) {
        return 1.0;
    }
    return exp((current_cost - new_cost) / temperature);
}
