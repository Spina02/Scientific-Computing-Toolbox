import unittest
import sys
import os

import numpy as np

from scientific_toolbox.ode import ODETester

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA_DIR = os.path.join(ROOT_DIR, 'data')
OUTPUT_DIR = os.path.join(ROOT_DIR, 'output/ODE_Module/Analysis')

class TestStatisticsModule(unittest.TestCase):
    
    @classmethod
    def setUpClass(cls):
        pass

    def setUp(self):
        self.tester = ODETester(DATA_DIR + '/ode_tests.csv')
        
    def test_run_parser_tests(self):
        # Test the parser module
        assert(self.tester.run_parser_tests())
    
    def test_ode(self):
        # Test the ODE module
        assert(self.tester.run_ode_tests())
    
if __name__ == "__main__":
    unittest.main()