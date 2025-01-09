import os
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import time
from scipy.interpolate import interp1d, lagrange as scipy_lagrange, CubicSpline

class InterpolationAnalysis:
    def __init__(self):
        script_dir = os.path.dirname(__file__)
        build_dir = os.path.join(script_dir, '../build/src/Interpolation_Module/')
        sys.path.append(build_dir)
        import interpolation
        self.interpolation = interpolation

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

    def sparse_points_generator(self, min, max, n):
        sparse_x = np.linspace(min, max, n)
        sparse_y = [np.sin(x_i) for x_i in sparse_x]
        sparse_data = pd.DataFrame({'x': sparse_x, 'y': sparse_y})
        return self.interpolation.df_to_set_of_points(sparse_data)

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

    def run_tests(self):
        self.interpolation.InterpolationTester().run_tests()

    def interpolate_and_plot(self, data, filename_prefix, sparse_data=None):
        
        if sparse_data is not None:
            sparse_points = self.interpolation.df_to_set_of_points(sparse_data)
            points = self.interpolation.df_to_set_of_points(data)

            interpolators = {
                'Linear': self.interpolation.LinearInterpolation(sparse_points),
                'Lagrange': self.interpolation.Lagrange(sparse_points),
                'Newton': self.interpolation.Newton(sparse_points),
                'Spline': self.interpolation.SplineInterpolation(sparse_points),
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
            plt.savefig(f'../output/{filename_prefix}_interpolations_from_function.png')
        
        else:
            points = self.interpolation.df_to_set_of_points(data)

            interpolators = {
                'Linear': self.interpolation.LinearInterpolation(points),
                'Lagrange': self.interpolation.Lagrange(points),
                'Newton': self.interpolation.Newton(points),
                'Spline': self.interpolation.SplineInterpolation(points),
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
            plt.savefig(f'../output/{filename_prefix}_interpolations.png')

    def analyze_accuracy(self, points, sparse_points):
        analysis = self.interpolation.AnalysisInterpolation()
        methods = ["linear", "lagrange", "newton", "cubic_spline"]
        accuracies = {}
        for method in methods:
            mae = analysis.AccuracyAnalysis(points, sparse_points, method)
            accuracies[method] = mae
        return accuracies

    def analyze_efficiency(self, points, sparse_points):
        analysis = self.interpolation.AnalysisInterpolation()
        methods = ["linear", "lagrange", "newton", "cubic_spline"]
        efficiencies = {}
        for method in methods:
            efficiencies[method] = analysis.EfficiencyAnalysis(points, sparse_points, method)
        return efficiencies
    
    def plot_order_of_convergence(self, data, sparse_data, n_points_vector):
        points = self.interpolation.df_to_set_of_points(data)

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
        axs[0, 0].plot(data['x'], data['y'], 'ro', label='Data points')
        axs[0, 1].plot(data['x'], data['y'], 'ro', label='Data points')
        axs[1, 0].plot(data['x'], data['y'], 'ro', label='Data points')
        axs[1, 1].plot(data['x'], data['y'], 'ro', label='Data points')

        for i in range(len(n_points_vector)):
            sparse_points = self.sparse_points_generator(0, 10, n_points_vector[i])
            vec_sparse_points.append(sparse_points)
            if i != 0:
                linear_ooc.append(self.interpolation.AnalysisInterpolation().OrderConvergenceAnalysis(points, vec_sparse_points[i-1], vec_sparse_points[i], "linear"))
                lagrangge_ooc.append(self.interpolation.AnalysisInterpolation().OrderConvergenceAnalysis(points, vec_sparse_points[i-1], vec_sparse_points[i], "lagrange"))
                newton_ooc.append(self.interpolation.AnalysisInterpolation().OrderConvergenceAnalysis(points, vec_sparse_points[i-1], vec_sparse_points[i], "newton"))
                spline_ooc.append(self.interpolation.AnalysisInterpolation().OrderConvergenceAnalysis(points, vec_sparse_points[i-1], vec_sparse_points[i], "cubic_spline"))
            
            linear_interpolation = self.interpolation.LinearInterpolation(sparse_points)
            lagrange_interpolation = self.interpolation.Lagrange(sparse_points)
            newton_interpolation = self.interpolation.Newton(sparse_points)
            spline_interpolation = self.interpolation.SplineInterpolation(sparse_points)

            y_linear = [linear_interpolation.interpolate(x_i) for x_i in x]
            y_lagrange = [lagrange_interpolation.interpolate(x_i) for x_i in x]
            y_newton = [newton_interpolation.interpolate(x_i) for x_i in x]
            y_spline = [spline_interpolation.interpolate(x_i) for x_i in x]
            
            axs[0, 0].plot(x, y_linear, color=n_colors[i % len(n_colors)], label=f'n = {n_points_vector[i]}')
            axs[0, 1].plot(x, y_lagrange, color=n_colors[i % len(n_colors)], label=f'n = {n_points_vector[i]}')
            axs[1, 0].plot(x, y_newton, color=n_colors[i % len(n_colors)], label=f'n = {n_points_vector[i]}')
            axs[1, 1].plot(x, y_spline, color=n_colors[i % len(n_colors)], label=f'n = {n_points_vector[i]}')

        # Adding subtitles
        axs[0, 0].set_title('Linear Interpolation')
        axs[0, 1].set_title('Lagrange Interpolation')
        axs[1, 0].set_title('Newton Interpolation')
        axs[1, 1].set_title('Spline Interpolation')

        # Add the external legend
        fig.legend(handles=external_legend_elements, loc='upper left', bbox_to_anchor=(0.1, 1), ncol=3, frameon=False, fontsize='small')

        # Adjust layout
        plt.tight_layout(rect=[0, 0, 1, 0.95])  # Leave space for the legend
        plt.savefig('../output/interpolations_with_different_npoints.png')

        # Plotting OOC
        fig, axs = plt.subplots(2, 2, figsize=(10, 8))
        fig.suptitle('Order of Convergence')
        external_legend_elements = [
            plt.Line2D([0], [0], color = 'blue',marker = 'o', linestyle='-', label = 'Error')
        ]        
        for ax, (name, ooc) in zip(axs.flat, [("Linear", linear_ooc), ("Lagrange", lagrangge_ooc), ("Newton", newton_ooc), ("Spline", spline_ooc)]):
            ax.plot(n_points_vector[1:], ooc, 'bo-', label='Error')
            ax.set_title(name)
        fig.legend(handles=external_legend_elements, loc='upper left', bbox_to_anchor=(0, 1), frameon=False)
        plt.tight_layout()
        plt.savefig('../output/order_of_convergence.png')

        # Printing ooc
        print("Linear OOC: ", linear_ooc)
        print("Lagrange OOC: ", lagrangge_ooc)
        print("Newton OOC: ", newton_ooc)
        print("Spline OOC: ", spline_ooc)

        
            

    def time_comparison(self, data):
        points = self.interpolation.df_to_set_of_points(data)

        time_linear = []
        time_scipy_linear = []
        start = time.time()
        self.interpolation.LinearInterpolation(points)
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
        self.interpolation.Lagrange(points)
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
        self.interpolation.SplineInterpolation(points)
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
    points = analyzer.interpolation.df_to_set_of_points(function_data)
    sparse_points = analyzer.interpolation.df_to_set_of_points(function_sparse_data)
    accuracies = analyzer.analyze_accuracy(points, sparse_points)
    efficiencies = analyzer.analyze_efficiency(points, sparse_points)
    for method, mae in accuracies.items():
        print(f"MAE for {method}: {mae}")

    # Oder of convergence analysis
    n_points_vector = [4, 8, 16, 32, 64]
    analyzer.plot_order_of_convergence(function_data, function_sparse_data, n_points_vector)

    # Time comparison
    analyzer.time_comparison(random_data)

