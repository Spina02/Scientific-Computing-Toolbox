import unittest
import sys
import os 
import time 
import numpy as np
import pandas as pd

from scipy.interpolate import interp1d, lagrange as scipy_lagrange, CubicSpline

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

import scientific_toolbox.interpolation as interp

class TestInterpolation(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        pass

    def setUp(self):
        pass

    def test_random_data_generator(self):
        analyzer = interp.InterpolationAnalysis()
        random_data = analyzer.random_data_generator(0, 10, 20, seed=17)
        self.assertEqual(random_data.shape, (20, 2))

    def test_data_generator(self):
        analyzer = interp.InterpolationAnalysis()
        data = analyzer.data_generator(0, 10, 20, np.sin)
        self.assertEqual(data.shape, (20, 2))
    
    def test_interpolate_and_plot(self):
        analyzer = interp.InterpolationAnalysis()
        random_data = analyzer.random_data_generator(0, 10, 20, seed=17)
        analyzer.interpolate_and_plot(random_data)
        self.assertTrue(True)

    def test_interpolation_on_function(self):
        analyzer = interp.InterpolationAnalysis()
        function_data = analyzer.data_generator(0, 10, 20, np.sin)
        function_sparse_data = analyzer.sample_values(function_data['x'], 5)
        analyzer.interpolate_and_plot(function_data, function_sparse_data)
        self.assertTrue(True)

    def test_accuracy(self):
        analyzer = interp.InterpolationAnalysis()
        function_data = analyzer.data_generator(0, 10, 20, np.sin)
        function_sparse_data = analyzer.sample_values(function_data['x'], 5)
        analyzer.analyze_accuracy(function_data, function_sparse_data)
        self.assertTrue(True)
    
    def test_efficiency(self):
        analyzer = interp.InterpolationAnalysis()
        function_data = analyzer.data_generator(0, 10, 20, np.sin)
        function_sparse_data = analyzer.sample_values(function_data['x'], 5)
        analyzer.analyze_efficiency(function_data, function_sparse_data)
        self.assertTrue(True)
    
    def test_order_of_convergence(self):
        analyzer = interp.InterpolationAnalysis()
        function_data = analyzer.data_generator(0, 10, 20, np.sin)
        function_sparse_data = analyzer.sample_values(function_data['x'], 5)
        n_points_vector = [4, 8, 16, 32, 64]
        analyzer.plot_order_of_convergence(function_data, function_sparse_data, n_points_vector)
        self.assertTrue(True)

    def test_time_comparison(self):
        analyzer = interp.InterpolationAnalysis()
        random_data = analyzer.random_data_generator(0, 10, 20, seed=17)
        analyzer.time_comparison(random_data)
        self.assertTrue(True)

    def test_interpolation(self):
        data = pd.read_csv(ROOT_DIR + '/data/random_data.csv')
        interpolator = interp.Interpolator()
        interpolator.get_data(data)
        interpolator.set_interpolation_method('linear')
        interpolator.set_value_to_interpolate(5.67)
        interpolator.interpolate()
        self.assertTrue(True)
    
    

if __name__ == '__main__':
    unittest.main()


