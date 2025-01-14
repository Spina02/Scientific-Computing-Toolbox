"""Scientific toolbox module for solving and analyzing Ordinary Differential Equations (ODEs).

This module provides:
- Multiple ODE solver implementations (Forward Euler, Explicit Midpoint, RK4)
- Visualization and analysis tools
- Performance comparison between C++ and Python implementations
- Test case management
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mc
import colorsys
import os
import sys
from scipy.integrate import solve_ivp
from ._ode import *
from scientific_toolbox.utilities import timer_decorator

from ._ode import (
    ODETestCase as _ODETestCase,
    ODESolution as _ODESolution,
    ODESolver as _ODESolver,
    ForwardEulerSolver as _ForwardEulerSolver,
    ExplicitMidpointSolver as _ExplicitMidpointSolver,
    RK4Solver as _RK4Solver,
    ODETester as _ODETester,
    load_tests_from_csv as _load_tests_from_csv,
    
)

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def load_tests_from_csv(file_path):
    __doc__ = _load_tests_from_csv.__doc__
    return _load_tests_from_csv(file_path)

class ODETestCase(_ODETestCase):
    __doc__ = _ODETestCase.__doc__

class ODESolution(_ODESolution):
    __doc__ = _ODESolution.__doc__

class ODESolver(_ODESolver):
    __doc__ = _ODESolver.__doc__

class ForwardEulerSolver(_ForwardEulerSolver):
    __doc__ = _ForwardEulerSolver.__doc__

class ExplicitMidpointSolver(_ExplicitMidpointSolver):
    __doc__ = _ExplicitMidpointSolver.__doc__

class RK4Solver(_RK4Solver):
    __doc__ = _RK4Solver.__doc__

class ODETester(_ODETester):
    __doc__ = _ODETester.__doc__

class ODEAnalysis:
    """
    Class for analyzing and visualizing ODE solutions.
    
    This class provides methods for:
    - Plotting ODE solutions: `plot_solution`
    - Comparing different ODE solvers: `compare_solvers`
    - Comparing C++ and Python solvers: `compare_cpp_py`
    """
    def __init__(self):
        pass

    def plot_solution(self, sol, title=None, save_path=None, show=True):
        """Plot ODE solution
        
        Args:
            sol: ODE solution to plot
            title: Optional title for the plot
            save_path: Optional path to save the plot
            show: Whether to display the plot with plt.show()
        """
        fig, ax = plt.subplots(figsize=(10, 6))
        
        t_values = np.array(sol.get_times())
        y_values = np.array(sol.get_solution())
        
        if y_values.ndim == 1:
            ax.plot(t_values, y_values, label='y(t)')
        else:
            for i in range(y_values.shape[1]):
                ax.plot(t_values, y_values[:, i], label=f'y{i+1}(t)')
        
        ax.set_xlabel('t')
        ax.set_ylabel('y')
        ax.grid(True)
        ax.legend()
        
        if title:
            ax.set_title(title)
            
        if save_path:
            os.makedirs(os.path.dirname(save_path), exist_ok=True)
            plt.savefig(save_path)
            
        if show:
            plt.show()
        else:
            plt.close()
        
    def _lighten_color(self, color, amount=0.5):
                """
                Lighten or darken the given color by a specified amount.
                """
                try:
                    c = mc.cnames[color]
                except:
                    c = color
                c = colorsys.rgb_to_hls(*mc.to_rgb(c))
                # Compute new luminance
                l_new = 1 - amount * (1 - c[1])
                return colorsys.hls_to_rgb(c[0], l_new, c[2])
            
    """def set_window_position(fig, x=100, y=100, width=800, height=600):

        fig_manager = plt.get_current_fig_manager()
        try:
            if hasattr(fig_manager, 'window'):
                fig_manager.window.geometry(f"{width}x{height}+{x}+{y}")  # Tk backend
            elif hasattr(fig_manager, 'canvas'):
                fig_manager.window.setGeometry(x, y, width, height)  # Qt backend
        except:
            pass  # Fallback for other backends"""
            
    def compare_solvers(self, test_case, solvers=None, save_path=None, show=True):
        """Compare different ODE solvers
        
        Args:
            test_case: Test case from data
            solvers: Optional list of solvers to compare
            save_path: Optional path to save the plot
            show: Whether to display the plot with plt.show()
        """
        if solvers is None:
            solvers = get_solver_types()
            
        solutions = {}
        
        # Map solver names to their corresponding classes
        solvers_map = {
            "ForwardEulerSolver": ForwardEulerSolver,
            "ExplicitMidpointSolver": ExplicitMidpointSolver,
            "RK4Solver": RK4Solver
        }
        
        # Create a wrapper function for timing
        @timer_decorator # -> (double, Solution)
        def solve_wrapper(solver):
            return solver.solve()

        # Solve the test_case with each solver
        for solver_name in solvers:
            solver_cls = solvers_map.get(solver_name)
            if solver_cls is None:
                raise ValueError(f"Solver {solver_name} not recognized.")
                
            solver = solver_cls(test_case)
            
            print(f"Solving {test_case.expr} with {solver_name}...")
            elapsed, solution = solve_wrapper(solver, solver_name)
            print(f"Elapsed time: {elapsed:.6f} s")
            solutions[solver_name] = solution
        
        # Number of equations in the system
        num_eq = solutions[next(iter(solutions))].get_size()
        
        # Create the figure and axes
        fig, axes = plt.subplots(
            nrows=num_eq,
            ncols=len(solvers),
            figsize=(7 * len(solvers), 3*num_eq)
        )
        
        # Ensure axes is 2D
        if num_eq == 1 and len(solvers) == 1:
            axes = np.array([[axes]])
        elif num_eq == 1:
            axes = axes[np.newaxis, :]
        elif len(solvers) == 1:
            axes = axes[:, np.newaxis]

        # Prepare base colors (one for each equation)
        base_colors = plt.cm.tab10(np.linspace(0, 1, num_eq))

        # Plot each equation for each solver
        for eq_idx in range(num_eq):
            base_col = base_colors[eq_idx]
            
            for col, (solver_name, sol) in enumerate(solutions.items()):
                ax = axes[eq_idx, col]
                t_values = np.array(sol.get_times())
                y_values = np.array(sol.get_solution())
                
                # Adjust shade based on the solver index
                if len(solvers) > 1:
                    shade_amount = 0.5 + 0.4 * (col / (len(solvers) - 1))
                else:
                    shade_amount = 0.5
                this_color = self._lighten_color(base_col, amount=shade_amount)

                # Handle single- vs multi-dimensional output
                if num_eq == 1:
                    ax.plot(t_values, y_values, label=solver_name, color=this_color)
                else:
                    ax.plot(t_values, y_values[:, eq_idx], label=solver_name, color=this_color)

                ax.set_title(f"Eq {eq_idx+1}: {sol.get_expr()[eq_idx]},\n{solver_name}")
                ax.set_xlabel("t")
                ax.grid(True)
                
                # Add legend only in the top row
                #if eq_idx == 0:
                #    ax.legend(loc="upper right")
        
        plt.suptitle(f"Solutions for ODE: {test_case.expr}")
        plt.tight_layout()
        
        if save_path:
            os.makedirs(save_path, exist_ok=True)
            plt.savefig(os.path.join(save_path, f"case_{eq_idx}.png"))
            
        if show:
            plt.show()
        else:
            plt.close()

    

    def compare_cpp_py(self, data):
        # Map solver names to scipy methods
        solvers_map = {
            "ForwardEulerSolver": {
                "cpp": ForwardEulerSolver,
                "py": "RK23",  # Scipy equivalent
            },
            "ExplicitMidpointSolver": {
                "cpp": ExplicitMidpointSolver,
                "py": "RK23",  # Midpoint simulated with RK23
            },
            "RK4Solver": {
                "cpp": RK4Solver,
                "py": "RK45",  # Scipy equivalent
            },
        }

        # Wrapper for C++ solvers
        @timer_decorator
        def cpp_solve_wrapper(solver, solver_name):
            return solver.solve().get_result()

        # Wrapper for Python solvers using scipy
        @timer_decorator
        def py_solve_wrapper(py_solver_method, test_case):

            # Assicura che y0 sia un array 1D
            y0 = np.atleast_1d(test_case.y0).astype(float)

            # Costruisci la funzione
            fun = parseExpression(test_case.expr)

            # Genera i punti di valutazione
            t_points = np.arange(test_case.t0, test_case.tf + test_case.h, test_case.h)

            # Risolvi con SciPy
            solution = solve_ivp(fun,
                                (test_case.t0, test_case.tf),
                                y0,
                                method=py_solver_method,
                                t_eval=t_points)
            
            sol = solution.y[:, -1]
            
            # if sol is a 1D array, return the value
            if len(sol) == 1:
                return sol[0]
            return sol

        # Iterate over test cases
        for test_case in data:
            for solver_name, solvers in solvers_map.items():
                # Get the C++ and Python solver classes
                cpp_solver_cls = solvers["cpp"]
                py_solver_method = solvers["py"]

                try:
                    print(f"Solving {test_case.expr} with {solver_name}...")
                    expected = test_case.get_expected_solution()
                    print(f"Expected solution: {expected}")
                    # Solve using the C++ solver
                    cpp_solver = cpp_solver_cls(test_case)
                    time, cpp_solution = cpp_solve_wrapper(cpp_solver, solver_name)
                    print(f"C++:\n\tresult:\t{cpp_solution},\n\terror:\t{compute_error(cpp_solution, expected)}\n\ttime:\t{time}")

                    # Solve using the Python solver
                    time, py_solution = py_solve_wrapper(py_solver_method, test_case)
                    print(f"Py:\n\tresult:\t{py_solution},\n\terror:\t{compute_error(py_solution, expected)}\n\ttime:\t{time}")

                    print()
                    
                except Exception as e:
                    print(f"Error : {e}")