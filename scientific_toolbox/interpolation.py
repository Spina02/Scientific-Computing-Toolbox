# imports
import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from ._interpolation import *

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_dir = os.path.join(ROOT_DIR, 'data')
output_dir = os.path.join(ROOT_DIR, 'output/Interpolation_Module/')

class Interpolator:
    def __init__(self, data_dir=data_dir, output_dir=output_dir):
        self.data_dir = data_dir
        self.output_dir = output_dir
        self.data = None
        self.method = "linear"
        self.interp = None


    def get_data(self):
        data_file = input("Enter the path to the data to interpolate: ")
        data_path = os.path.join(self.data_dir, data_file) if data_file else os.path.join(self.data_dir, 'random_data.csv')
        
        if not data_file:
            print("No path was entered, using the default random data in random_data.csv")
        
        self.data = pd.read_csv(data_path)

    def choose_interpolation_method(self):
        method = input("Enter the interpolation method (\"linear\", \"lagrange\", \"newton\", \"spline\"): ")
        if not method:
            self.method = "linear"
            print("No method entered, using the linear one")
        else:
            self.method = method.lower()

    def choose_value_to_interpolate(self):
        value_to_interpolate = input(f"Enter the value to interpolate (in the range [{min(self.data['x'])}, {max(self.data['x'])}]): ")
        if not value_to_interpolate:
            self.value_to_interpolate = (max(self.data['x']) - min(self.data['x'])) / 2
            print("No value was entered, using 0.5")
        else:
            self.value_to_interpolate = float(value_to_interpolate)
        
        # Assessing value_to_interpolate
        if self.value_to_interpolate < min(self.data['x']) or self.value_to_interpolate > max(self.data['x']):
            raise ValueError(f"Value to interpolate {self.value_to_interpolate} is out of bounds.")

    def interpolate(self):
        points = df_to_set_of_points(self.data)
        
        # Choosing the interpolation method based on user input
        if self.method == "linear":
            self.interp = LinearInterpolation(points)
        elif self.method == "lagrange":
            self.interp = LagrangeInterpolation(points)
        elif self.method == "newton":
            self.interp = NewtonInterpolation(points)
        elif self.method == "spline":
            self.interp = SplineInterpolation(points)
        else:
            raise ValueError("Invalid interpolation method.")

        interpolated_value = self.interp.interpolate(self.value_to_interpolate)
        print(f"The interpolated value at {self.value_to_interpolate} is: {interpolated_value}")
        return interpolated_value

    def plot(self):
        x = np.linspace(min(self.data['x']), max(self.data['x']), 1000)
        y = [self.interp.interpolate(i) for i in x]
        y_chosen = self.interp.interpolate(self.value_to_interpolate)
        
        plt.plot(x, y)
        plt.scatter(self.data['x'], self.data['y'])
        plt.scatter(self.value_to_interpolate, y_chosen, color='red', label=f"Interpolated value: {y_chosen}")
        plt.title(f"Interpolation using {self.method} method")
        plt.legend()
        plt.savefig(os.path.join(self.output_dir, 'plot_main.png'))
        print(f"Plot saved to {os.path.join(self.output_dir, 'plot_main.png')}")

    def run(self):
        self.get_data()
        self.choose_interpolation_method()
        self.choose_value_to_interpolate()
        self.interpolate()
        self.plot()