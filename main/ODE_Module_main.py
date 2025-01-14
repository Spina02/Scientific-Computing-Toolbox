import os
import sys
import scientific_toolbox

# make shure you have the scientific_toolbox package installed with `pip install -e .`
try:
    import scientific_toolbox.ode as ode
except ImportError as e:
    print("\nModule not found. Have you built the project?\nRun 'make' in the build directory, then 'pip install -e .'\n")
    print(e)
    sys.exit(1)

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_dir = os.path.join(ROOT_DIR, 'data')
output_dir = os.path.join(ROOT_DIR, 'output/ODE_Module/Analysis')

def main():
    # Letting the user choose the path to data containing the ODEs, and if does not enter anything, the default data is used
    data_path = input("Enter the path to the csv file containing the ODEs: ")
    if data_path == "":
        data_path = os.path.join(data_dir, 'ode_tests.csv')
        print(f"No path was entered, using the default ODEs in {data_path}")
        
    data = ode.load_tests_from_csv(data_path)
    
    os.makedirs(output_dir, exist_ok=True)
    
    output_files = {
        "euler"     : os.path.join(output_dir, "euler_solution.csv"),
        "midpoint"  : os.path.join(output_dir, "midpoint_solution.csv"),
        "rk4"       : os.path.join(output_dir, "rk4_solution.csv")
    }
    
    plot_folders = {
        "euler"     : os.path.join(output_dir, "euler_plots"),
        "midpoint"  : os.path.join(output_dir, "midpoint_plots"),
        "rk4"       : os.path.join(output_dir, "rk4_plots")
    }
    
    analyzer = ode.ODEAnalysis()
    
    # Clear the output files
    for file in output_files.values():
        with open(file, "w") as f:
            f.write("")
    
    for i, case in enumerate(data):
        print(case)
        
        euler = ode.ForwardEulerSolver(case)
        euler_solution = euler.solve()
        print(f"The euler solution of the ODE is:\t{euler_solution.get_result()}")
        ode.save_to_csv(output_files["euler"], euler_solution, append = True)
        analyzer.plot_solution(euler_solution, title = f"{case.get_expr()} - Euler", save_path = os.path.join(plot_folders["euler"], f"case_{i}"), show = False)
        
        midpoint = ode.ExplicitMidpointSolver(case)
        midpoint_solution = midpoint.solve()
        print(f"The midpoint solution of the ODE is:\t{midpoint_solution.get_result()}")
        ode.save_to_csv(output_files["midpoint"], midpoint_solution, append = True)
        analyzer.plot_solution(midpoint_solution, title = f"{case.get_expr()} - Midpoint", save_path = os.path.join(plot_folders["midpoint"], f"case_{i}"), show = False)
        
        rk4 = ode.RK4Solver(case)
        rk4_solution = rk4.solve()
        print(f"The RK4 solution of the ODE is:\t\t{rk4_solution.get_result()}")       
        ode.save_to_csv(output_files["rk4"], rk4_solution, append = True)
        analyzer.plot_solution(rk4_solution, title = f"{case.get_expr()} - RK4", save_path = os.path.join(plot_folders["rk4"], f"case_{i}"), show = False)
        
        print()
    
if __name__ == '__main__':
    main()