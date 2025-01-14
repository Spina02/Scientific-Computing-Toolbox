import sys
import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 

try:
    from scientific_toolbox import interpolation
except ImportError as e:
    print("\nModule not found. Have you built the project?\nRun 'make' in the build directory, then 'pip install -e .'\n")
    print(e)
    sys.exit(1)

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_dir = os.path.join(ROOT_DIR, 'data')
output_dir = os.path.join(ROOT_DIR, 'output/Interpolation_Module/')
os.makedirs(output_dir, exist_ok=True)

# Usage
if __name__ == "__main__":
    interpolator = interpolation.Interpolator()
    data_file = input("Enter the name of the data file: ")
    if not os.path.exists(os.path.join(data_dir, data_file)):
        print("File not found.")
        sys.exit(1)
    if data_file == "":
        data_file = "random_data.csv"
    data = pd.read_csv(os.path.join(data_dir, data_file))
    interpolator.get_data(data)
    method = input("Enter the interpolation method (\'linear\', \'lagrange\', \'newton\', \'spline\'): ")
    if method == "":
        method = "linear"
    interpolator.set_interpolation_method(method)
    value = input("Enter the value to interpolate: ")
    if value == "":
        value = data['x'].mean()
    value = float(value)
    interpolator.set_value_to_interpolate(value)
    interpolator.interpolate()
    interpolator.plot()
