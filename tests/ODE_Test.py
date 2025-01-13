import sys
import os

import numpy as np

# Add the build folder to sys.path
script_dir = os.path.dirname(__file__)  # Directory of the current script
build_dir = os.path.join(script_dir, '../build/src/ODE_Module/')  # Adjust the relative path
data_dir = os.path.join(script_dir, '../data/')
src_dir = os.path.join(script_dir, '../src/ODE_Module')
output_dir = os.path.join(script_dir, '../output/ODE_Module/Analysis')
sys.path.append(build_dir)
sys.path.append(src_dir)

# Now you can import your module
import ODE
import analysis

def main():
    print(sys.executable)
    print(sys.path)

    # Letting the user choose the path to data containing the ODEs, and if does not enter anything, the default data is used
    data_path = input("Enter the path to the csv file containing the ODEs: ")
    if data_path == "":
        data_path = os.path.join(data_dir, 'ode_tests.csv')
        print(f"No path was entered, using the default ODEs in {data_path}")
        
    data = ODE.load_tests_from_csv(data_path)
    
    analyzer = analysis.ODEAnalysis()
    
    # plot solutions from different solvers
    analyzer.compare_solvers(data, save_path = os.path.join(output_dir, ""), show = False)

    # Solve a single ODE

    expr = ["0.1 * y1 - 0.02 * y1 * y2", "-0.3 * y2 + 0.01 * y1 * y2"];
    y0 = [50.0, 20.0]
    real_solution = [35.8857, 22.7193]
    
    euler = ODE.ForwardEulerSolver(expr, y0, 0, 1, 0.001)
    euler_solution = euler.solve()
    print(f"The euler solution of the ODE '{expr}' is: {euler_solution.get_result()}\n\texpected value: \t\t{real_solution}\n\terror: \t{ODE.compute_error(euler_solution.get_result(), real_solution)}")
    analyzer.plot_solution(euler_solution, title = "Predator-Prey model - Euler", save_path = os.path.join(output_dir, "pp_euler"), show = False)
    
    midpoint = ODE.ExplicitMidpointSolver(expr, y0, 0, 1, 0.001)
    midpoint_solution = midpoint.solve()
    print(f"The midpoint solution of the ODE '{expr}' is: {midpoint_solution.get_result()}\n\texpected value: \t\t{real_solution}\n\terror: \t{ODE.compute_error(midpoint_solution.get_result(), real_solution)}")
    analyzer.plot_solution(midpoint_solution, title = "Predator-Prey model - midpoint", save_path =  os.path.join(output_dir, "pp_midpoint"), show = False)
    
    rk4 = ODE.RK4Solver(expr, y0, 0, 1, 0.001)
    rk4_solution = rk4.solve()
    print(f"The RK4 solution of the ODE '{expr}' is: {rk4_solution.get_result()}\n\texpected value: \t\t{real_solution}\n\terror: \t{ODE.compute_error(rk4_solution.get_result(), real_solution)}")
    analyzer.plot_solution(rk4_solution, title = "Predator-Prey model - RK4", save_path = os.path.join(output_dir, "pp_rk4"), show = False)
 
    #Compare the C++ and Python solvers
    analyzer.compare_cpp_py([data[-1]])   
    
if __name__ == '__main__':
    main()