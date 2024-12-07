# ODE Module Implementation Plan

This document outlines the steps to implement the ODE module as specified in `homework_02.md`.

## Objective

Implement a module for solving Ordinary Differential Equations (ODEs) of the form:

$$
\frac{\mathrm{d}\mathbf{y}}{\mathrm{d}t} = \mathbf{f}(t, \mathbf{y}),
$$

using explicit Runge-Kutta methods: Forward Euler, RK4, and (**bonus**) the explicit midpoint method.

The module should handle both scalar ($N=1$) and vector problems and export the solution to a CSV file with appropriate headers.

## TODO

1. **Define Necessary Data Structures**

   - [ ] Represent the state variable vector $\mathbf{y}$ using `std::vector<double>`.
   - [ ] Define the type for the function $\mathbf{f}(t, \mathbf{y})$ using `std::function`.
   - [ ] Handle both scalar and vector cases.

2. **Declare Classes and Functions in Header `ODE_Module.hpp`**

   - [ ] Create an abstract class `ODESolver` with:
     - Constructor accepting the time interval, step size $h$, the function $\mathbf{f}$, and initial conditions $\mathbf{y}_0$.
     - Pure virtual method `solve()` to perform the integration.w
   - [ ] Derive concrete classes for each method:
     - `ForwardEulerSolver`
     - `RK4Solver`
     - (**Bonus**) `ExplicitMidpointSolver`

3. **Implement Integration Methods in `ODE_Module.cpp`**

   - [ ] Implement the `solve()` method for each derived class:
     - **Forward Euler Method**:
       $$
       \mathbf{y}_{n+1} = \mathbf{y}_n + h \cdot \mathbf{f}(t_n, \mathbf{y}_n)
       $$
     - **Explicit Midpoint Method**:
       $$
       \begin{align*}
       \mathbf{k}_1 &= \mathbf{f}(t_n, \mathbf{y}_n) \\
       \mathbf{k}_2 &= \mathbf{f}\left(t_n + \frac{h}{2}, \mathbf{y}_n + \frac{h}{2} \mathbf{k}_1\right) \\
       \mathbf{y}_{n+1} &= \mathbf{y}_n + h \cdot \mathbf{k}_2
       \end{align*}
       $$
     - **RK4 Method**:
       $$
       \begin{align*}
       \mathbf{k}_1 &= \mathbf{f}(t_n, \mathbf{y}_n) \\
       \mathbf{k}_2 &= \mathbf{f}\left(t_n + \frac{h}{2}, \mathbf{y}_n + \frac{h}{2} \mathbf{k}_1\right) \\
       \mathbf{k}_3 &= \mathbf{f}\left(t_n + \frac{h}{2}, \mathbf{y}_n + \frac{h}{2} \mathbf{k}_2\right) \\
       \mathbf{k}_4 &= \mathbf{f}(t_n + h, \mathbf{y}_n + h \mathbf{k}_3) \\
       \mathbf{y}_{n+1} &= \mathbf{y}_n + \frac{h}{6} (\mathbf{k}_1 + 2\mathbf{k}_2 + 2\mathbf{k}_3 + \mathbf{k}_4)
       \end{align*}
       $$
   - [ ] Ensure implementations work for systems of arbitrary size $N$.

4. **Implement Result Export Function**

   - [ ] Create a function to export values of $t$ and $\mathbf{y}$ to a CSV file.
   - [ ] Include appropriate headers (`t, y1, y2, ..., yN`).
   - [ ] Note: Functions like this should be placed in `Utilities.hpp` and `Utilities.cpp`.

5. **Write Tests in `ODE_Module_main.cpp`**

   - [ ] Implement tests for scalar problems (e.g., radioactive decay equation).
   - [ ] Implement tests for ODE systems (e.g., Lotka-Volterra equations).
   - [ ] Compare numerical solutions with analytical solutions when available.
   - [ ] Evaluate accuracy and convergence order of the methods.

6. **Exception and Error Handling**

   - [ ] Add checks for invalid or inconsistent inputs.
   - [ ] Handle possible exceptions during integration (e.g., excessively large time steps).

7. **Documentation**

   - [ ] Comment code explaining design choices.
   - [ ] Provide usage examples in notes or comments.

8. **Optional Improvements (Bonus)**

   - [ ] Implement adaptive step size control.
   - [ ] Integrate third-party libraries (e.g., Boost ODEInt) if appropriate.

## General Notes

- Use modern C++ features (C++17 or later).
- Keep code modular and readable.
- Follow coding standards and best practices.
