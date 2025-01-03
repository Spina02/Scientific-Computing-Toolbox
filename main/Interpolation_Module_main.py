
import sys
import os

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


# Add the build folder to sys.path
script_dir = os.path.dirname(__file__)  # Directory of the current script
build_dir = os.path.join(script_dir, '../build/src/Interpolation_Module/')  # Adjust the relative path
data_dir = os.path.join(script_dir, '../data/')
sys.path.append(build_dir)

# Now you can import your module
import interpolation

# Letting the user choose the path to data to interpolate, and if does not enter anything, the default data is used
data_path = input("Enter the path to the data to interpolate: ")
if data_path == "":
    data_path = os.path.join(data_dir, 'random_data.csv')
    print("No path was entered, using the default random data in data.csv")
data = pd.read_csv(data_path)

# Interpolating the data
points = interpolation.df_to_set_of_points(data)
spline = interpolation.SplineInterpolation(points)
value_to_interpolate = input(f"Enter the value to interpolate (in the range [{min(data['x']), max(data['x'])}]): ")
if value_to_interpolate == "":
    value_to_interpolate = 0.5
    print("No value was entered, using 0.5")
print(f"The interpolated value is: {spline.interpolate(float(value_to_interpolate))}")

# Plotting the data and the interpolation
x = np.linspace(min(data['x']), max(data['x']), 1000)
y = [spline.interpolate(i) for i in x]
y_chosen = spline.interpolate(float(value_to_interpolate))
plt.plot(x, y)
plt.scatter(data['x'], data['y'])
plt.scatter(float(value_to_interpolate), y_chosen, color='red', label=f"Interpolated value: {y_chosen}")
plt.legend()
plt.show()
