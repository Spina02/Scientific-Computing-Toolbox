import os
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import time
from scipy.interpolate import interp1d, lagrange as scipy_lagrange, CubicSpline
from scientific_toolbox.interpolation import InterpolationAnalysis, df_to_set_of_points

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Usage
if __name__ == "__main__":
    analyzer = InterpolationAnalysis()
    analyzer.run_tests()

    # Example data and operations
    random_data = analyzer.random_data_generator(0, 10, 20, seed=17)
    analyzer.interpolate_and_plot(random_data, "random_data")

    # Using a function to generate data
    function_data = analyzer.data_generator(0, 10, 20, np.sin)
    function_sparse_data = analyzer.sample_values(function_data['x'], 5)
    analyzer.interpolate_and_plot(function_data, "function_data", function_sparse_data)

    # Accuracy and Efficiency Analysis
    points = df_to_set_of_points(function_data)
    sparse_points = df_to_set_of_points(function_sparse_data)
    accuracies = analyzer.analyze_accuracy(points, sparse_points)
    efficiencies = analyzer.analyze_efficiency(points, sparse_points)
    for method, mae in accuracies.items():
        print(f"MAE for {method}: {mae}")

    # Oder of convergence analysis
    n_points_vector = [4, 8, 16, 32, 64]
    analyzer.plot_order_of_convergence(function_data, function_sparse_data, n_points_vector)

    # Time comparison
    analyzer.time_comparison(random_data)

