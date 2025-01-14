import sys
import os

import numpy as np

from scientific_toolbox.ode import ODEAnalysis, load_tests_from_csv, ForwardEulerSolver, ExplicitMidpointSolver, RK4Solver, compute_error

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_dir = os.path.join(ROOT_DIR, 'data')
output_dir = os.path.join(ROOT_DIR, 'output/ODE_Module/Analysis')

def main():
    print("\n\nmoved to notebook\n\n")
    # TODO
    
if __name__ == '__main__':
    main()