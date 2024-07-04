// Sequential Ordering Problem (SOP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_ITER 10000
#define COOLING_FACTOR 0.95
#define INITIAL_TEMP 100.0
#define EPSILON 1e-5
#define N 5  // Number of elements in the sequence
// Objective function: Sum of pairwise squared differences
double objective_function(int *sequence, int n) {
    double sum = 0.0;
    for (int i = 1; i < n; ++i) {
        sum += pow(sequence[i] - sequence[i - 1], 2);
    }
    return sum;
}
// Simulated annealing algorithm
void simulated_annealing(int *sequence, int n) {
    double current_energy, new_energy, temperature, acceptance_prob;
    int temp_sequence[n];
    int current_sequence[n];
    int best_sequence[n];
    double best_energy = INFINITY;
    // Initialize current sequence randomly
    for (int i = 0; i < n; ++i) {
        sequence[i] = i + 1;
    }
    // Use current time as seed for random generator
    srand(time(NULL));
    // Start with initial temperature
    temperature = INITIAL_TEMP;
    // Main loop of simulated annealing
    for (int iter = 0; iter < MAX_ITER && temperature > EPSILON; ++iter) {
        // Copy current sequence to temp_sequence
        for (int i = 0; i < n; ++i) {
            temp_sequence[i] = sequence[i];
        }
        // Perform a random swap of two elements
        int index1 = rand() % n;
        int index2 = rand() % n;
        int temp = temp_sequence[index1];
        temp_sequence[index1] = temp_sequence[index2];
        temp_sequence[index2] = temp;
        // Calculate energies
        current_energy = objective_function(sequence, n);
        new_energy = objective_function(temp_sequence, n);
        // Decide whether to accept the new sequence
        if (new_energy < current_energy) {
            for (int i = 0; i < n; ++i) {
                sequence[i] = temp_sequence[i];
            }
            current_energy = new_energy;
        } else {
            acceptance_prob = exp(-(new_energy - current_energy) / temperature);
            double random_value = (double)rand() / RAND_MAX;
            if (random_value < acceptance_prob) {
                for (int i = 0; i < n; ++i) {
                    sequence[i] = temp_sequence[i];
                }
                current_energy = new_energy;
            }
        }
        // Update the best sequence found so far
        if (current_energy < best_energy) {
            best_energy = current_energy;
            for (int i = 0; i < n; ++i) {
                best_sequence[i] = sequence[i];
            }
        }
        // Cool the temperature
        temperature *= COOLING_FACTOR;
    }
    // Print the best sequence found
    printf("Best sequence found: ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", best_sequence[i]);
    }
    printf("\n");
    printf("Objective function = %lf\n", best_energy);
}
int main() {
    int sequence[N];
simulated_annealing(sequence, N);
    return 0;
}
