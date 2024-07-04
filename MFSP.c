// Multistage Flowshop Scheduling Problem (MFSP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_STAGES 5
#define MAX_JOBS 10

// Define the processing times matrix
int processing_times[MAX_JOBS][MAX_STAGES] = {
    {3, 2, 6, 5, 8},
    {2, 5, 3, 7, 4},
    {4, 2, 3, 6, 3},
    {6, 1, 2, 5, 4},
    {5, 3, 6, 2, 5},
    {2, 3, 4, 3, 7},
    {3, 2, 5, 4, 6},
    {4, 3, 2, 5, 4},
    {5, 4, 3, 2, 6},
    {3, 6, 4, 5, 2}
};

// Function to calculate total completion time for a given sequence
int calculate_completion_time(int sequence[]) {
    int completion_times[MAX_JOBS][MAX_STAGES];

    // Initialize first job's completion times
    completion_times[0][0] = processing_times[sequence[0]][0];
    for (int j = 1; j < MAX_STAGES; ++j) {
        completion_times[0][j] = completion_times[0][j-1] + processing_times[sequence[0]][j];
    }

    // Calculate completion times for subsequent jobs
    for (int i = 1; i < MAX_JOBS; ++i) {
        completion_times[i][0] = completion_times[i-1][0] + processing_times[sequence[i]][0];
        for (int j = 1; j < MAX_STAGES; ++j) {
            completion_times[i][j] = fmax(completion_times[i][j-1], completion_times[i-1][j]) + processing_times[sequence[i]][j];
        }
    }

    // Total completion time is the last completion time in the matrix
    return completion_times[MAX_JOBS-1][MAX_STAGES-1];
}

// Simulated Annealing Algorithm
void simulated_annealing(int current_sequence[], double initial_temperature, double cooling_rate) {
    int best_sequence[MAX_JOBS];
    int current_cost, new_cost, best_cost;

    // Initialize current sequence and costs
    for (int i = 0; i < MAX_JOBS; ++i) {
        best_sequence[i] = current_sequence[i];
    }
    current_cost = calculate_completion_time(current_sequence);
    best_cost = current_cost;

    // Simulated Annealing loop
    double temperature = initial_temperature;
    while (temperature > 1e-10) {
        // Generate a neighboring solution by swapping two jobs
        int index1 = rand() % MAX_JOBS;
        int index2 = rand() % MAX_JOBS;
        while (index1 == index2) {
            index2 = rand() % MAX_JOBS;
        }
        int temp = current_sequence[index1];
        current_sequence[index1] = current_sequence[index2];
        current_sequence[index2] = temp;

        // Calculate the cost of the new sequence
        new_cost = calculate_completion_time(current_sequence);

        // Decide if we should accept the new sequence
        if (new_cost < current_cost) {
            current_cost = new_cost;
            if (current_cost < best_cost) {
                best_cost = current_cost;
                for (int i = 0; i < MAX_JOBS; ++i) {
                    best_sequence[i] = current_sequence[i];
                }
            }
        } else {
            double probability = exp(-(new_cost - current_cost) / temperature);
            if (rand() / (double)RAND_MAX < probability) {
                current_cost = new_cost;
            } else {
                // Revert back to the previous sequence
                temp = current_sequence[index1];
                current_sequence[index1] = current_sequence[index2];
                current_sequence[index2] = temp;
            }
        }

        // Cool down the temperature
        temperature *= cooling_rate;
    }

    // Print the best sequence found
    printf("Best Sequence: ");
    for (int i = 0; i < MAX_JOBS; ++i) {
        printf("%d ", best_sequence[i]);
    }
    printf("\nBest Completion Time = %d\n", best_cost);
}

int main() {
    srand(time(NULL));

    // Initialize a random initial sequence
    int initial_sequence[MAX_JOBS];
    for (int i = 0; i < MAX_JOBS; ++i) {
        initial_sequence[i] = i;
    }

    // Set initial temperature and cooling rate
    double initial_temperature = 1000.0;
    double cooling_rate = 0.99;

    // Run simulated annealing
    simulated_annealing(initial_sequence, initial_temperature, cooling_rate);

    return 0;
}
