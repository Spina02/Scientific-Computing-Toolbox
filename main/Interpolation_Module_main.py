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
    interpolator.run()
