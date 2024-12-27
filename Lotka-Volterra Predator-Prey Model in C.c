#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Define the parameters for the model
#define ALPHA 0.1   // Growth rate of prey
#define BETA 0.02   // Rate at which predators kill prey
#define DELTA 0.01  // Rate at which predators reproduce by consuming prey
#define GAMMA 0.1   // Death rate of predators

#define MAX_STEPS 10000  // Maximum number of steps for the simulation
#define OUTPUT_INTERVAL 100 // Interval to output data to file

// Function to compute the derivatives (dx/dt and dy/dt)
void lotka_volterra(double x, double y, double *dxdt, double *dydt) {
    *dxdt = ALPHA * x - BETA * x * y;
    *dydt = DELTA * x * y - GAMMA * y;
}

// Function to perform the Euler's method integration
void euler_method(double x0, double y0, double h, int steps) {
    double x = x0;   // Initial prey population
    double y = y0;   // Initial predator population
    double t = 0.0;  // Time
    double dxdt, dydt;

    // Open a file to save the results
    FILE *file = fopen("predator_prey_simulation.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the header to the file
    fprintf(file, "Time, Prey Population, Predator Population\n");

    // Perform the integration using Euler's method
    for (int i = 0; i < steps; i++) {
        // Save the current values of t, x, and y at intervals
        if (i % OUTPUT_INTERVAL == 0) {
            fprintf(file, "%.2f, %.2f, %.2f\n", t, x, y);
        }

        // Compute the derivatives
        lotka_volterra(x, y, &dxdt, &dydt);

        // Update the populations using Euler's method
        x += h * dxdt;
        y += h * dydt;

        // Update the time
        t += h;
    }

    // Close the file
    fclose(file);
}

// Function to perform the Runge-Kutta method for comparison
void runge_kutta_method(double x0, double y0, double h, int steps) {
    double x = x0;   // Initial prey population
    double y = y0;   // Initial predator population
    double t = 0.0;  // Time
    double dxdt1, dydt1, dxdt2, dydt2, dxdt3, dydt3, dxdt4, dydt4;

    // Open a file to save the results
    FILE *file = fopen("predator_prey_runge_kutta.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the header to the file
    fprintf(file, "Time, Prey Population, Predator Population\n");

    // Perform the integration using Runge-Kutta method
    for (int i = 0; i < steps; i++) {
        // Compute the derivatives for each step
        lotka_volterra(x, y, &dxdt1, &dydt1);
        lotka_volterra(x + 0.5 * h * dxdt1, y + 0.5 * h * dydt1, &dxdt2, &dydt2);
        lotka_volterra(x + 0.5 * h * dxdt2, y + 0.5 * h * dydt2, &dxdt3, &dydt3);
        lotka_volterra(x + h * dxdt3, y + h * dydt3, &dxdt4, &dydt4);

        // Update the populations using Runge-Kutta method
        x += (h / 6.0) * (dxdt1 + 2 * dxdt2 + 2 * dxdt3 + dxdt4);
        y += (h / 6.0) * (dydt1 + 2 * dydt2 + 2 * dydt3 + dydt4);

        // Save the current values of t, x, and y at intervals
        if (i % OUTPUT_INTERVAL == 0) {
            fprintf(file, "%.2f, %.2f, %.2f\n", t, x, y);
        }

        // Update the time
        t += h;
    }

    // Close the file
    fclose(file);
}

// Function to perform the Modified Euler's method for comparison
void modified_euler_method(double x0, double y0, double h, int steps) {
    double x = x0;   // Initial prey population
    double y = y0;   // Initial predator population
    double t = 0.0;  // Time
    double dxdt1, dydt1, dxdt2, dydt2;

    // Open a file to save the results
    FILE *file = fopen("predator_prey_modified_euler.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the header to the file
    fprintf(file, "Time, Prey Population, Predator Population\n");

    // Perform the integration using Modified Euler's method
    for (int i = 0; i < steps; i++) {
        // Compute the derivatives for each step
        lotka_volterra(x, y, &dxdt1, &dydt1);
        lotka_volterra(x + h * dxdt1, y + h * dydt1, &dxdt2, &dydt2);

        // Update the populations using Modified Euler's method
        x += 0.5 * h * (dxdt1 + dxdt2);
        y += 0.5 * h * (dydt1 + dydt2);

        // Save the current values of t, x, and y at intervals
        if (i % OUTPUT_INTERVAL == 0) {
            fprintf(file, "%.2f, %.2f, %.2f\n", t, x, y);
        }

        // Update the time
        t += h;
    }

    // Close the file
    fclose(file);
}

// Function to perform the simulation for a given method and save data
void simulate_and_save(double x0, double y0, double h, int steps, const char *method) {
    if (strcmp(method, "Euler") == 0) {
        euler_method(x0, y0, h, steps);
    } else if (strcmp(method, "Runge-Kutta") == 0) {
        runge_kutta_method(x0, y0, h, steps);
    } else if (strcmp(method, "Modified Euler") == 0) {
        modified_euler_method(x0, y0, h, steps);
    } else {
        printf("Unknown method!\n");
    }
}

// Function to plot the results using a simple ASCII plot
void ascii_plot(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file for plotting.\n");
        return;
    }

    double t, prey, predators;
    printf("Time\tPrey\tPredators\n");
    while (fscanf(file, "%lf, %lf, %lf\n", &t, &prey, &predators) != EOF) {
        printf("%.2f\t%.2f\t%.2f\n", t, prey, predators);
    }

    fclose(file);
}

int main() {
    // Initial conditions for the populations
    double x0 = 40.0;  // Initial prey population
    double y0 = 9.0;   // Initial predator population

    // Step size and number of steps for the simulation
    double h = 0.1;    // Time step size
    int steps = MAX_STEPS;  // Number of steps

    // Perform the simulation with different methods
    simulate_and_save(x0, y0, h, steps, "Euler");
    simulate_and_save(x0, y0, h, steps, "Runge-Kutta");
    simulate_and_save(x0, y0, h, steps, "Modified Euler");

    // Print a message when the simulation is complete
    printf("Simulation complete. Results saved to respective CSV files.\n");

    // Optional: Display data using ASCII plot
    ascii_plot("predator_prey_simulation.csv");

    return 0;
}
