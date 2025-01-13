import sys
import os

import numpy as np

from scientific_toolbox.ode import ODEAnalysis, load_tests_from_csv, ForwardEulerSolver, ExplicitMidpointSolver, RK4Solver, compute_error

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_dir = os.path.join(ROOT_DIR, 'data')
output_dir = os.path.join(ROOT_DIR, 'output/ODE_Module/Analysis')

def main():
    print(sys.executable)
    print(sys.path)

    # Letting the user choose the path to data containing the ODEs, and if does not enter anything, the default data is used
    data_path = input("Enter the path to the csv file containing the ODEs: ")
    if data_path == "":
        data_path = os.path.join(data_dir, 'ode_tests.csv')
        print(f"No path was entered, using the default odes in {data_path}")
        
    data = load_tests_from_csv(data_path)
    
    analyzer = ODEAnalysis()
    
    # plot solutions from different solvers
    analyzer.compare_solvers(data, save_path = os.path.join(output_dir, ""), show = False)

    # Solve a single ODE

    expr = ["0.1 * y1 - 0.02 * y1 * y2", "-0.3 * y2 + 0.01 * y1 * y2"];
    y0 = [50.0, 20.0]
    real_solution = [35.8857, 22.7193]
    
    euler = ForwardEulerSolver(expr, y0, 0, 1, 0.001)
    euler_solution = euler.solve()
    print(f"The euler solution of the ODE '{expr}' is: {euler_solution.get_result()}\n\texpected value: \t\t{real_solution}\n\terror: \t{compute_error(euler_solution.get_result(), real_solution)}")
    analyzer.plot_solution(euler_solution, title = "Predator-Prey model - Euler", save_path = os.path.join(output_dir, "pp_euler"), show = False)
    
    midpoint = ExplicitMidpointSolver(expr, y0, 0, 1, 0.001)
    midpoint_solution = midpoint.solve()
    print(f"The midpoint solution of the ODE '{expr}' is: {midpoint_solution.get_result()}\n\texpected value: \t\t{real_solution}\n\terror: \t{compute_error(midpoint_solution.get_result(), real_solution)}")
    analyzer.plot_solution(midpoint_solution, title = "Predator-Prey model - midpoint", save_path =  os.path.join(output_dir, "pp_midpoint"), show = False)
    
    rk4 = RK4Solver(expr, y0, 0, 1, 0.001)
    rk4_solution = rk4.solve()
    print(f"The RK4 solution of the ODE '{expr}' is: {rk4_solution.get_result()}\n\texpected value: \t\t{real_solution}\n\terror: \t{compute_error(rk4_solution.get_result(), real_solution)}")
    analyzer.plot_solution(rk4_solution, title = "Predator-Prey model - RK4", save_path = os.path.join(output_dir, "pp_rk4"), show = False)
 
    #Compare the C++ and Python solvers
    analyzer.compare_cpp_py([data[-1]])   
    
if __name__ == '__main__':
    main()