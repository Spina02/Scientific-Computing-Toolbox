import numpy as np
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import matplotlib.colors as mc
import colorsys
import os
import sys

script_dir = os.path.dirname(__file__)  # Directory of the current script
build_dir = os.path.join(script_dir, '../build/src/ODE_Module/')  # Adjust the relative path
sys.path.append(build_dir)

# Now you can import your module
import ODE

def plot_solution(sol, title=None):
    t_values = np.array(sol.get_times())
    y = sol.get_solution()
    num_eq = sol.get_size()
    
    # Gestione dimensioni dinamiche
    fig_width = max(15, 5 * num_eq)  # Almeno 15, ma aumenta con il numero di equazioni
    fig_height = 6  # Altezza fissa o leggermente adattata
    
    fig = plt.figure(figsize=(fig_width, fig_height))
    gridspec = GridSpec(1, num_eq, figure=fig)
    
    # Divide le soluzioni in base alle equazioni
    if num_eq == 1:
        y = [y]
    else:
        y = list(zip(*y))  # Trasponi per separare le equazioni
    
    # Creazione dei subplots
    for eq in range(num_eq):
        ax = fig.add_subplot(gridspec[0, eq])
        
        # Linea standard per la soluzione
        ax.plot(t_values, y[eq], label='Soluzione', linestyle='-', linewidth=2, color=lighten_color('blue', 0.7))
        
        ax.set_ylabel(f'$y_{eq+1}$')
        ax.set_xlabel('$t$')
        ax.set_title(f"Equazione {eq+1}:\n{sol.get_expr()[eq]}", fontsize=12)
        ax.legend()
        ax.grid(True, alpha=0.3)
    
    # Titolo generale
    if title:
        fig.suptitle(title, fontsize=16)
    
    # Regolazione layout
    fig.tight_layout(rect=[0, 0, 1, 0.95])  # Lascia spazio per il titolo generale
    plt.show()
    
def lighten_color(color, amount=0.5):
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
        
def set_window_position(fig, x=100, y=100, width=800, height=600):
    """
    Imposta la posizione e le dimensioni iniziali della finestra Matplotlib.
    """
    fig_manager = plt.get_current_fig_manager()
    try:
        if hasattr(fig_manager, 'window'):
            fig_manager.window.geometry(f"{width}x{height}+{x}+{y}")  # Tk backend
        elif hasattr(fig_manager, 'canvas'):
            fig_manager.window.setGeometry(x, y, width, height)  # Qt backend
    except:
        pass  # Fallback for other backends

def compare_solvers(data, solvers=None):
    # If no solvers are specified, get all available solver types
    if solvers is None:
        solvers = ODE.get_solver_types()
        
    for test_case in data:
        solutions = {}
        
        # Map solver names to their corresponding classes
        solvers_map = {
            "ForwardEulerSolver": ODE.ForwardEulerSolver,
            "ExplicitMidpointSolver": ODE.ExplicitMidpointSolver,
            "RK4Solver": ODE.RK4Solver
        }

        # Solve the test_case with each solver
        for solver_name in solvers:
            solver_cls = solvers_map.get(solver_name)
            if solver_cls is None:
                raise ValueError(f"Solver {solver_name} not recognized.")
            solver = solver_cls(test_case)
            solutions[solver_name] = solver.solve()
        
        # Number of equations in the system
        num_eq = solutions[next(iter(solutions))].get_size()
        
        # Create the figure and axes
        fig, axes = plt.subplots(
            nrows=num_eq,
            ncols=len(solvers),
            figsize=(9 * len(solvers), 3*num_eq)
        )
        
        # Imposta la posizione e le dimensioni iniziali della finestra
        set_window_position(fig, x=200, y=100, width=1200, height=800)
        
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
                this_color = lighten_color(base_col, amount=shade_amount)

                # Handle single- vs multi-dimensional output
                if num_eq == 1:
                    ax.plot(t_values, y_values, label=solver_name, color=this_color)
                else:
                    ax.plot(t_values, y_values[:, eq_idx], label=solver_name, color=this_color)

                ax.set_title(f"Eq {eq_idx+1}: {sol.get_expr()[eq_idx]},\n{solver_name}")
                ax.set_xlabel("t")
                ax.grid(True)
                
                # Add legend only in the top row
                if eq_idx == 0:
                    ax.legend(loc="upper left")
        
        plt.suptitle(f"Solutions for ODE: {test_case.expr}")
        plt.tight_layout()
        plt.show()
