import numpy as np
import matplotlib.pyplot as plt

# Function for dy/dx = 1 + y^2
def f(x, y):
    return 1 + y * y

def adams_moulton_exact_steps(x0, y0, h, target_steps):
    x = [x0]
    y = [y0]
    
    # Compute first 3 steps using Runge-Kutta 4th Order method
    for i in range(3):
        k1 = h * f(x[-1], y[-1])
        k2 = h * f(x[-1] + h / 2, y[-1] + k1 / 2)
        k3 = h * f(x[-1] + h / 2, y[-1] + k2 / 2)
        k4 = h * f(x[-1] + h, y[-1] + k3)
        y_next = y[-1] + (k1 + 2 * k2 + 2 * k3 + k4) / 6
        y.append(y_next)
        x.append(x[-1] + h)

    # Adams-Bashforth Predictor and Adams-Moulton Corrector
    for i in range(3, target_steps):
        x_next = x[-1] + h
        # Predictor (Adams-Bashforth 4th Order)
        y_pred = y[-1] + h / 24 * (55 * f(x[-1], y[-1]) - 
                                   59 * f(x[-2], y[-2]) + 
                                   37 * f(x[-3], y[-3]) - 
                                   9 * f(x[-4], y[-4]))
        # Corrector (Adams-Moulton 4th Order)
        y_corr = y[-1] + h / 24 * (9 * f(x_next, y_pred) + 
                                   19 * f(x[-1], y[-1]) - 
                                   5 * f(x[-2], y[-2]) + 
                                   f(x[-3], y[-3]))
        y.append(y_corr)
        x.append(x_next)

    return x, y

# Inputs
x0, y0 = 0, 0  # Initial condition
h = 0.1  # Step size
target_steps = 3  # Number of steps to x = 0.3

# Compute the solution
x, y = adams_moulton_exact_steps(x0, y0, h, target_steps + 1)

# Print results
print("Results:")
for i in range(len(x)):
    print(f"x = {x[i]:.3f}, y = {y[i]:.6f}")

# Plot the results
plt.plot(x, y, 'o-', label="Adams-Moulton Solution")
plt.xlabel("x")
plt.ylabel("y")
plt.title("Solution of dy/dx = 1 + y^2 using Adams-Moulton Method")
plt.legend()
plt.grid()
plt.show()