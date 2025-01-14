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
    print("\n\nmoved to notebook\n\n")
    # TODO