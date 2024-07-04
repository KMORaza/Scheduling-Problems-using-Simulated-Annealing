// Permutation Flow Shop Problem (PFSP)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_ITER 10000
#define INITIAL_TEMP 100.0
#define COOLING_RATE 0.95
// Define constants for the problem
#define JOBS 3
#define MACHINES 3
// Function prototypes
void generateRandomSchedule(int schedule[JOBS][MACHINES]);
int calculateMakespan(int schedule[JOBS][MACHINES]);
void copySchedule(int from[JOBS][MACHINES], int to[JOBS][MACHINES]);
void swapJobs(int schedule[JOBS][MACHINES], int job1, int job2);
double acceptanceProbability(int currentMakespan, int newMakespan, double temperature);
void simulatedAnnealing(int schedule[JOBS][MACHINES]);
int main() {
    srand(time(NULL));
    int schedule[JOBS][MACHINES];
    // Generate a random initial schedule
generateRandomSchedule(schedule);
    // Print the initial schedule
    printf("Initial Schedule:\n");
    for (int i = 0; i < JOBS; i++) {
        for (int j = 0; j < MACHINES; j++) {
            printf("%d ", schedule[i][j]);
        }
        printf("\n");
    }
    // Run simulated annealing to find the optimal schedule
simulatedAnnealing(schedule);
    // Print the final optimal schedule found
    printf("\nOptimal Schedule:\n");
    for (int i = 0; i < JOBS; i++) {
        for (int j = 0; j < MACHINES; j++) {
            printf("%d ", schedule[i][j]);
        }
        printf("\n");
    }
    // Print the makespan of the optimal schedule
    int optimalMakespan = calculateMakespan(schedule);
    printf("\nOptimal Makespan = %d\n", optimalMakespan);
    return 0;
}
// Function to generate a random initial schedule
void generateRandomSchedule(int schedule[JOBS][MACHINES]) {
    for (int i = 0; i < JOBS; i++) {
        for (int j = 0; j < MACHINES; j++) {
            schedule[i][j] = j + 1; // Random initial order
        }
        // Shuffle the jobs randomly
        for (int j = MACHINES - 1; j > 0; j--) {
            int k = rand() % (j + 1);
            int temp = schedule[i][j];
            schedule[i][j] = schedule[i][k];
            schedule[i][k] = temp;
        }
    }
}
// Function to calculate the makespan of a schedule
int calculateMakespan(int schedule[JOBS][MACHINES]) {
    int completionTimes[JOBS][MACHINES];
    // Initialize the completion times matrix
    completionTimes[0][0] = schedule[0][0];
    for (int j = 1; j < MACHINES; j++) {
        completionTimes[0][j] = completionTimes[0][j - 1] + schedule[0][j];
    }
    for (int i = 1; i < JOBS; i++) {
        completionTimes[i][0] = completionTimes[i - 1][0] + schedule[i][0];
    }
    // Fill the rest of the completion times matrix
    for (int i = 1; i < JOBS; i++) {
        for (int j = 1; j < MACHINES; j++) {
            completionTimes[i][j] = fmax(completionTimes[i - 1][j], completionTimes[i][j - 1]) + schedule[i][j];
        }
    }
    // Makespan is the last element in the completion times matrix
    return completionTimes[JOBS - 1][MACHINES - 1];
}
// Function to copy one schedule to another
void copySchedule(int from[JOBS][MACHINES], int to[JOBS][MACHINES]) {
    for (int i = 0; i < JOBS; i++) {
        for (int j = 0; j < MACHINES; j++) {
            to[i][j] = from[i][j];
        }
    }
}
// Function to swap two jobs in a schedule
void swapJobs(int schedule[JOBS][MACHINES], int job1, int job2) {
    for (int j = 0; j < MACHINES; j++) {
        int temp = schedule[job1][j];
        schedule[job1][j] = schedule[job2][j];
        schedule[job2][j] = temp;
    }
}
// Function to calculate acceptance probability in simulated annealing
double acceptanceProbability(int currentMakespan, int newMakespan, double temperature) {
    if (newMakespan < currentMakespan) {
        return 1.0; // Accept the new solution if it's better
    }
    return exp((currentMakespan - newMakespan) / temperature);
}
// Function implementing simulated annealing
void simulatedAnnealing(int schedule[JOBS][MACHINES]) {
    int currentMakespan = calculateMakespan(schedule);
    int bestSchedule[JOBS][MACHINES];
    copySchedule(schedule, bestSchedule);
    int bestMakespan = currentMakespan;
    double temperature = INITIAL_TEMP;
    for (int iter = 1; iter <= MAX_ITER; iter++) {
        // Generate a new neighboring solution by swapping two random jobs
        int job1 = rand() % JOBS;
        int job2 = rand() % JOBS;
        while (job1 == job2) {
            job2 = rand() % JOBS;
        }
        copySchedule(schedule, bestSchedule);
        swapJobs(schedule, job1, job2);
        // Calculate the makespan of the new solution
        int newMakespan = calculateMakespan(schedule);
        // Decide whether to accept the new solution
        if (acceptanceProbability(currentMakespan, newMakespan, temperature) > ((double)rand() / RAND_MAX)) {
            // Accept the new solution
            currentMakespan = newMakespan;
            if (currentMakespan < bestMakespan) {
                bestMakespan = currentMakespan;
                copySchedule(schedule, bestSchedule);
            }
        } else {
            // Revert back to the previous solution
copySchedule(bestSchedule, schedule);
        }
        // Cool down the temperature
        temperature *= COOLING_RATE;
    }
    // Copy the best schedule found back to the original schedule
    copySchedule(bestSchedule, schedule);
}
