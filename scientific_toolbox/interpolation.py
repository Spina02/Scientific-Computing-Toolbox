# imports
import os
import time
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d, lagrange as scipy_lagrange, CubicSpline
from ._interpolation import *

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_dir = os.path.join(ROOT_DIR, 'data')
output_dir = os.path.join(ROOT_DIR, 'output/Interpolation_Module/')

class InterpolationAnalysis:
    def __init__(self):
        pass 

    @staticmethod
    def random_data_generator(min, max, num_points, seed=None):
        if seed is not None:
            np.random.seed(seed)
        x = np.linspace(min, max, num_points)
        y = np.random.rand(num_points)
        return pd.DataFrame({'x': x, 'y': y})

    @staticmethod
    def data_generator(min, max, num_points, function):
        x = np.linspace(min, max, num_points)
        y = function(x)
        return pd.DataFrame({'x': x, 'y': y})

    def sparse_points_generator(self, min, max, n, function=np.sin):
        """
        Generate sparse points using a specified function.
        
        Parameters:
        min (float): Minimum value of x.
        max (float): Maximum value of x.
        n (int): Number of points.
        function (callable): Function to compute y values. Default is np.sin.
        
        Returns:
        pd.DataFrame: Dataframe with columns x and y.
        """
        sparse_x = np.linspace(min, max, n)
        sparse_y = [function(x_i) for x_i in sparse_x]
        sparse_data = pd.DataFrame({'x': sparse_x, 'y': sparse_y})
        return df_to_set_of_points(sparse_data)

    @staticmethod
    def sample_values(values, num_samples):
        values = np.array(values)
        x = np.random.choice(values, num_samples - 2, replace=False)
        x = list(x)
        if values[0] not in x:
            x.append(values[0])
        if values[-1] not in x:
            x.append(values[-1])
        x.sort()
        y = [np.sin(x_i) for x_i in x]
        return pd.DataFrame({'x': x, 'y': y})

    def interpolate_and_plot(self, data, sparse_data=None):
        
        if sparse_data is not None:
            sparse_points = df_to_set_of_points(sparse_data)
            points = df_to_set_of_points(data)

            interpolators = {
                'Linear': LinearInterpolation(sparse_points),
                'Lagrange': Lagrange(sparse_points),
                'Newton': Newton(sparse_points),
                'Spline': SplineInterpolation(sparse_points),
            }

            x = np.linspace(sparse_data['x'].min(), sparse_data['x'].max(), 1000)
            interpolated_values = {name: [interp.interpolate(x_i) for x_i in x] for name, interp in interpolators.items()}

            # External legend elements
            external_legend_elements = [
                plt.Line2D([0], [0], color='red', marker='o', linestyle='None', label='True points'),
                plt.Line2D([0], [0], color='blue', marker='o', linestyle='None', label='Sampled points'),
                plt.Line2D([0], [0], color='green', linestyle='-', label='Interpolation')
            ]

            fig, axs = plt.subplots(2, 2, figsize=(10, 8))
            fig.suptitle('Interpolations')

            for ax, (name, y_values) in zip(axs.flat, interpolated_values.items()):
                ax.plot(data['x'], data['y'], 'ro', label='True points')
                ax.plot(sparse_data['x'], sparse_data['y'], 'bo', label='Sampled points')
                ax.plot(x, y_values, 'g-', label=f'{name} interpolation')
                ax.set_title(name)
            
            fig.legend(handles=external_legend_elements, loc='upper left', bbox_to_anchor=(0, 1), frameon=False)

            plt.tight_layout()
        
        else:
            points = df_to_set_of_points(data)

            interpolators = {
                'Linear': LinearInterpolation(points),
                'Lagrange': Lagrange(points),
                'Newton': Newton(points),
                'Spline': SplineInterpolation(points),
            }

            x = np.linspace(data['x'].min(), data['x'].max(), 1000)
            interpolated_values = {name: [interp.interpolate(x_i) for x_i in x] for name, interp in interpolators.items()}

            # External legend elements
            external_legend_elements = [
                plt.Line2D([0], [0], color='red', marker='o', linestyle='None', label='Data points'),
                plt.Line2D([0], [0], color='blue', label='Interpolation')
            ]


            fig, axs = plt.subplots(2, 2, figsize=(10, 8))
            fig.suptitle('Interpolations')
            
            for ax, (name, y_values) in zip(axs.flat, interpolated_values.items()):
                ax.plot(data['x'], data['y'], 'ro', label='Data points')
                ax.plot(x, y_values, label=f'{name} interpolation')
                ax.set_title(name)

            fig.legend(handles=external_legend_elements, loc='upper left', bbox_to_anchor=(0, 1), frameon=False)

            plt.tight_layout()
            os.makedirs('../output', exist_ok=True)

    def analyze_accuracy(self, data, sparse_data):
        points = df_to_set_of_points(data)
        sparse_points = df_to_set_of_points(sparse_data)
        analysis = AnalysisInterpolation()
        methods = ["linear", "lagrange", "newton", "cubic_spline"]
        accuracies = {}
        for method in methods:
            mae = analysis.AccuracyAnalysis(points, sparse_points, method)
            accuracies[method] = mae
        return accuracies

    def analyze_efficiency(self, data, sparse_data):
        points = df_to_set_of_points(data)
        sparse_points = df_to_set_of_points(sparse_data)
        analysis = AnalysisInterpolation()
        methods = ["linear", "lagrange", "newton", "cubic_spline"]
        efficiencies = {}
        for method in methods:
            efficiencies[method] = analysis.EfficiencyAnalysis(points, sparse_points, method)
        return efficiencies
    
    def plot_order_of_convergence(self, data, sparse_data, n_points_vector, function=np.sin):
        """
        Plot the order of convergence for different interpolation methods using specified sparse point generation function.
        
        Parameters:
        data (pd.DataFrame): Original data.
        sparse_data (pd.DataFrame): Sparse data.
        n_points_vector (list): Vector of point counts for sparse data generation.
        function (callable): Function to compute y values for sparse points. Default is np.sin.
        """
        points = df_to_set_of_points(data)

        x = np.linspace(sparse_data['x'].min(), sparse_data['x'].max(), 1000)

        vec_sparse_points = []
        linear_ooc = []
        lagrangge_ooc = []
        newton_ooc = []
        spline_ooc = []

        fig, axs = plt.subplots(2, 2, figsize=(10, 8))
        fig.suptitle('Interpolations')

        n_colors = ['green', 'orange', 'purple', 'brown', 'pink']  # Ensure the colors are sufficient

        # External legend elements
        external_legend_elements = [
            plt.Line2D([0], [0], color='red', marker='o', linestyle='None', label='True points'),
            plt.Line2D([0], [0], color=n_colors[0], linestyle='-', label='4 points'),
            plt.Line2D([0], [0], color=n_colors[1], linestyle='-', label='8 points'),
            plt.Line2D([0], [0], color=n_colors[2], linestyle='-', label='16 points'),
            plt.Line2D([0], [0], color=n_colors[3], linestyle='-', label='32 points'),
            plt.Line2D([0], [0], color=n_colors[4], linestyle='-', label='64 points')
        ]

        # Plotting original data points
        for ax in axs.flat:
            ax.plot(data['x'], data['y'], 'ro', label='Data points')

        for i, n_points in enumerate(n_points_vector):
            sparse_points = self.sparse_points_generator(0, 10, n_points, function=function)
            vec_sparse_points.append(sparse_points)
            if i != 0:
                linear_ooc.append(AnalysisInterpolation().OrderConvergenceAnalysis(points, vec_sparse_points[i-1], vec_sparse_points[i], "linear"))
                lagrangge_ooc.append(AnalysisInterpolation().OrderConvergenceAnalysis(points, vec_sparse_points[i-1], vec_sparse_points[i], "lagrange"))
                newton_ooc.append(AnalysisInterpolation().OrderConvergenceAnalysis(points, vec_sparse_points[i-1], vec_sparse_points[i], "newton"))
                spline_ooc.append(AnalysisInterpolation().OrderConvergenceAnalysis(points, vec_sparse_points[i-1], vec_sparse_points[i], "cubic_spline"))

            linear_interpolation = LinearInterpolation(sparse_points)
            lagrange_interpolation = Lagrange(sparse_points)
            newton_interpolation = Newton(sparse_points)
            spline_interpolation = SplineInterpolation(sparse_points)

            y_linear = [linear_interpolation.interpolate(x_i) for x_i in x]
            y_lagrange = [lagrange_interpolation.interpolate(x_i) for x_i in x]
            y_newton = [newton_interpolation.interpolate(x_i) for x_i in x]
            y_spline = [spline_interpolation.interpolate(x_i) for x_i in x]

            axs[0, 0].plot(x, y_linear, color=n_colors[i % len(n_colors)], label=f'n = {n_points}')
            axs[0, 1].plot(x, y_lagrange, color=n_colors[i % len(n_colors)], label=f'n = {n_points}')
            axs[1, 0].plot(x, y_newton, color=n_colors[i % len(n_colors)], label=f'n = {n_points}')
            axs[1, 1].plot(x, y_spline, color=n_colors[i % len(n_colors)], label=f'n = {n_points}')

        # Adding subtitles
        axs[0, 0].set_title('Linear Interpolation')
        axs[0, 1].set_title('Lagrange Interpolation')
        axs[1, 0].set_title('Newton Interpolation')
        axs[1, 1].set_title('Spline Interpolation')

        # Add the external legend
        fig.legend(handles=external_legend_elements, loc='upper left', bbox_to_anchor=(0.1, 1), ncol=3, frameon=False, fontsize='small')

        # Adjust layout
        plt.tight_layout(rect=[0, 0, 1, 0.95])  # Leave space for the legend

        # Plotting OOC
        fig, axs = plt.subplots(2, 2, figsize=(10, 8))
        fig.suptitle('Order of Convergence')
        external_legend_elements = [
            plt.Line2D([0], [0], color='blue', marker='o', linestyle='-', label='Error')
        ]        
        for ax, (name, ooc) in zip(axs.flat, [("Linear", linear_ooc), ("Lagrange", lagrangge_ooc), ("Newton", newton_ooc), ("Spline", spline_ooc)]):
            ax.plot(n_points_vector[1:], ooc, 'bo-', label='Error')
            ax.set_title(name)
        fig.legend(handles=external_legend_elements, loc='upper left', bbox_to_anchor=(0, 1), frameon=False)
        plt.tight_layout()

        # Printing ooc
        print("Linear OOC: ", linear_ooc)
        print("Lagrange OOC: ", lagrangge_ooc)
        print("Newton OOC: ", newton_ooc)
        print("Spline OOC: ", spline_ooc)

        
            

    def time_comparison(self, data):
        points = df_to_set_of_points(data)

        time_linear = []
        time_scipy_linear = []
        start = time.time()
        LinearInterpolation(points)
        end = time.time()
        time_linear.append(end - start)
        start = time.time()
        interp1d(data['x'], data['y'], kind='linear')
        end = time.time()
        time_scipy_linear.append(end - start)

        print("\nLinear Interpolation Time: " + str(time_linear) + "seconds")
        print("Scipy Linear Interpolation Time: " + str(time_scipy_linear) + "seconds")

        if time_linear[0] < time_scipy_linear[0]:
            print("Linear Interpolation is faster than Scipy Linear Interpolation.")

        time_lagrange = []
        time_scipy_lagrange = []
        start = time.time()
        Lagrange(points)
        end = time.time()
        time_lagrange.append(end - start)
        start = time.time()
        scipy_lagrange(data['x'], data['y'])
        end = time.time()
        time_scipy_lagrange.append(end - start)

        print("\nLagrange Interpolation Time: " + str(time_lagrange) + "seconds")
        print("Scipy Lagrange Interpolation Time: " + str(time_scipy_lagrange) + "seconds")

        if time_lagrange[0] < time_scipy_lagrange[0]:
            print("Lagrange Interpolation is faster than Scipy Lagrange Interpolation.")

        time_spline = []
        time_scipy_spline = []
        start = time.time()
        SplineInterpolation(points)
        end = time.time()
        time_spline.append(end - start)
        start = time.time()
        CubicSpline(data['x'], data['y'])
        end = time.time()
        time_scipy_spline.append(end - start)

        print("\nSpline Interpolation Time: " + str(time_spline) + "seconds")
        print("Scipy Spline Interpolation Time: " + str(time_scipy_spline) + "seconds")

        if time_spline[0] < time_scipy_spline[0]:
            print("Spline Interpolation is faster than Scipy Spline Interpolation.")

class Interpolator:
    def __init__(self, data_dir=data_dir):
        self.data_dir = data_dir
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

    def run(self):
        self.get_data()
        self.choose_interpolation_method()
        self.choose_value_to_interpolate()
        self.interpolate()
        self.plot()