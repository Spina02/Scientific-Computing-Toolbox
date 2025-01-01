
import sys
import os

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


# Add the build folder to sys.path
script_dir = os.path.dirname(__file__)  # Directory of the current script
build_dir = os.path.join(script_dir, '../build/src/Interpolation_Module/')  # Adjust the relative path
sys.path.append(build_dir)

# Now you can import your module
import interpolation_bindings as ib

# Function to generate random data
def random_data_generator(min, max, num_points, seed=None):
    x = np.linspace(min, max, num_points)
    y = np.random.rand(num_points)
    return pd.DataFrame({'x': x, 'y': y})

# Function to generate the data
def data_generator(min, max, num_points, function):
    x = np.linspace(min, max, num_points)
    y = function(x)
    return pd.DataFrame({'x': x, 'y': y})

# Testing interpolations
ib.InterpolationTester().run_tests()

# Generating some random data and interpolating it
random_data = random_data_generator(0, 10, 20, seed = 17)
random_points = ib.df_to_set_of_points(random_data)
plt.plot(random_data['x'], random_data['y'], 'ro', label='Data points')
plt.show()

# Interpolating the data
linear_interpolation = ib.LinearInterpolation(random_points)
lagrange_interpolation = ib.Lagrange(random_points)
newton_interpolation = ib.Newton(random_points)
spline_interpolation = ib.SplineInterpolation(random_points)


x = np.linspace(0, 10, 1000)
y_linear = [linear_interpolation.interpolate(x_i) for x_i in x]
y_lagrange = [lagrange_interpolation.interpolate(x_i) for x_i in x]
y_newton = [newton_interpolation.interpolate(x_i) for x_i in x]
y_spline = [spline_interpolation.interpolate(x_i) for x_i in x]

print("Plotting interpolations for 1000 random points in the interval [0, 10]...")

# Showing the four graphs in one divided in 2x2
fig, axs = plt.subplots(2, 2)
fig.suptitle('Interpolations')
axs[0, 0].plot(random_data['x'], random_data['y'], 'ro', label='Data points')
axs[0, 0].plot(x, y_linear, label='Linear interpolation')
axs[0, 0].legend()

axs[0, 1].plot(random_data['x'], random_data['y'], 'ro', label='Data points')
axs[0, 1].plot(x, y_lagrange, label='Lagrange interpolation')
axs[0, 1].legend()

axs[1, 0].plot(random_data['x'], random_data['y'], 'ro', label='Data points')
axs[1, 0].plot(x, y_newton, label='Newton interpolation')
axs[1, 0].legend()

axs[1, 1].plot(random_data['x'], random_data['y'], 'ro', label='Data points')
axs[1, 1].plot(x, y_spline, label='Spline interpolation')
axs[1, 1].legend()

plt.show()




