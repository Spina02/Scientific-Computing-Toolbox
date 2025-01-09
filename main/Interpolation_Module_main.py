
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
data_file = input("Enter the path to the data to interpolate: ")
data_path = data_dir + data_file
if data_file == "":
    data_path = os.path.join(data_dir, 'random_data.csv')
    print("No path was entered, using the default random data in random_data.csv")
data = pd.read_csv(data_path)


# Interpolation method
method = input("Enter the interpolation method (\"linear\", \"lagrange\", \"newton\", \"spline\"): ")
if method == "":
    method ="linear"
    print("No method entered, using the linear one")

value_to_interpolate = input(f"Enter the value to interpolate (in the range [{min(data['x']), max(data['x'])}]): ")
if value_to_interpolate == "":
    value_to_interpolate = 0.5
print("No value was entered, using 0.5")
## assessing value_to_interpolate
if (float(value_to_interpolate) > max(data['x']) or float(value_to_interpolate) < min(data['x'])):
    raise(ValueError)

# Interpolating the data
points = interpolation.df_to_set_of_points(data)
if (method == "linear"):
    interp = interpolation.LinearInterpolation(points)
elif (method == "lagrange"):
    interp = interpolation.LagrangeInterpolation(points)
elif (method == "newton"):
    interp = interpolation.NewtonInterpolation(points)
else:
    interp = interpolation.SplineInterpolation(points)
    

print(f"The interpolated value is: {interp.interpolate(float(value_to_interpolate))}")




# Plotting the data and the interpolation
x = np.linspace(min(data['x']), max(data['x']), 1000)
y = [interp.interpolate(i) for i in x]
y_chosen = interp.interpolate(float(value_to_interpolate))
plt.plot(x, y)
plt.scatter(data['x'], data['y'])
plt.scatter(float(value_to_interpolate), y_chosen, color='red', label=f"Interpolated value: {y_chosen}")
plt.title(f"Interpolation using {method} method")
plt.legend()
plt.savefig("../output/plot_main.png")
