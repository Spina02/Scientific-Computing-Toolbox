# Scientific Computing Toolbox

## Homework 

[Repository GitHub](https://github.com/christianfaccio/Scientific_Computing_Toolbox.git
)


This library has been created by [Giacomo Amerio](mailto:giacomoamerio24@gmail.com), [Christian Faccio](mailto:christianfaccio@outlook.it) and [Andrea Spinelli](mailto:andreaspinelli2002@gmail.com). It is a toolbox written in C++ and Python that contains three modules:

1) **Statistics Module**
    
    covers descriptive statistics and correlation analysis on a given dataset. For this module it's being used templates for functions' simplicity and different types of data input. 
2) **Interpolation Module**

    covers four types of interpolation on a given set of points (Linear, Polynomial with Lagrange and Newton method, Cubic Spline). For this module it's being used templates for different data input (int and double). 
3) **ODE Module**

    solves given differential equations in three ways (Forward Euler, Runge-Kutta-4, Explicit-Midpoint). This module includes templates and std::variant for different types of values.

All modules use inheritance for the natural form of hierarchy that exists in the various areas.

#### Individual contributions

***############################################## 
TODO -> ADD PYHTON SUTFF ##############################################***

- Giacomo Amerio developed the Statistics Module, and the general utilities of parsing and importing data.
- Christian Faccio developed the Interpolation Module. 
- Andrea Spinelli developed the ODE Module and the makefile structure.

All contributed to the debugging and optimisation phase, mostly Andrea who stimulated the whole group to look for the stars and make a great project.

---

## Project Structure
```bash
.
├── data/
├── demos/
│   ├── notebooks/
├── extern/                   # Pybind11 module
├── include/
│   ├── Interpolation_Module/ 
│   ├── ODE_Module/
│   ├── Statistics_Module/
│   ├── ScientificToolbox.hpp
│   ├── Utilities.hpp
├── main/
├── scientific_toolbox/
├── src/
│   ├── Interpolation_Module/ # Here you can find the Interpolation Module CMakeLists.txt
│   ├── ODE_Module/           # Here you can find the ODE Module CMakeLists.txt
│   ├── Statistics_Module/    # Here you can find the Statistics Module CMakeLists.txt
│   └── python_bindings/
├── tests/
├── CMakeLists.txt
├── MIT-License.txt
├── README.md
├── requirements.txt
└── setup.py
```
---

## Installation

You can use the following command in your terminal to clone this repository.
```bash
git clone --recurse-submodules https://github.com/christianfaccio/Scientific_Computing_Toolbox.git
```
>[!warning]
> make sure to initialise the pybind11 submodule during the  cloning with the command above. If you already cloned it without `--recurse-submodules` flag, you can still initialize the submodule using the following command:
>```bash
>git submodule update --init --recursive
>```

#### External Libraries
To build the C++ library **only** you just need to have ***Eigen*** , ***GSL*** and ***muParser*** libraries.

You can install them using your package manager:

- **Debian/Ubuntu**:

    ```bash
    sudo apt-get install libeigen3-dev libgsl-dev libmuparser-dev
    ```
- **Arch-Linux**: 
    ```bash
    sudo pacman -S eigen gsl muparser
    ```

- **macOS**: 
    ```bash
    brew install eigen gsl muparser
    ```
---

## Building

We decided to create a single command to compile the entire toolbox. You can simply run the following command right after cloning the repository (make sure you initialised the  submodules before).
```bash
make 
``` 

With this command alone one can use the C++ part of the library properly 

In order to use the python interface, you can choose between installing the toolbox locally (in your computer) or using a virtual environment:

- **Install locally**:
    ```bash
    pip install -r requirements.txt
    ```

- **Use a python virtual environment**:
    ```bash
    make venv
    ```
    This command creates a virtual environment with every requirement needed to run the `scientific_toolbox` package (specified in requirements.txt).

    You can activate the venv using `source ./venv/bin/activate` and deactivate it using `deactivate`.

> [!tip]
> The commands `make py-run` and `make py-test` do not require the venv to be explicitely activate, indeed they call directly the python executable there defined (`./venv/bin/python`).

## Testing

After the installation you can test the if package runs properly launching C++ and python tests as follows:

- C++ tests

    ```bash
    # Test all modules
    make test

    # Test individual Modules
    make test interpolation
    make test ode
    make test statistics
    ```

- Python tests:

    ```bash
    # Test all modules
    make py-test

    # Test individual Modules
    make py-test interpolation
    make py-test ode
    make py-test statistics
    ```

## Running

Our project contains different standalone executables which takes a

- C++
    ```bash 
    # Run all Modules
    make run

    # Run individual Modules
    make run interpolation
    make run ode
    make run statistics
    ```
- Python:

    ```bash
    # Test all modules
    make py-run

    # Test individual Modules
    make py-run interpolation
    make py-run ode
    make py-run statistics
    ```

## Cleaning all Build Files
```bash
make clean
```

### Cleaning individual Modules
```bash
make clean interpolation
make clean ode
make clean statistics
```

> [!NOTE]
> Both Interpolation and ODE modules include comprehensive test analysis sections that demonstrate error metrics, convergence rates, and other performance indicators.

## Module Specific example usage

### Interpolation Module
```bash
./build/Interpolation_Module/bin/Interpolation_Module_main [input_file] [value_to_interpolate]

# Follow the interactive prompts to specify input files and interpolation parameters
```

### ODE Module
```bash
./build/ODE_Module/bin/ODE_Module_main

# This file contains a demostration of usage of the ODE module. Results are saved to output/ODE_Module/
```

> [!TIP]
> It is possible to enable DEBUG prints in tweaking the flag in `include/ODE_Module/utils.cpp`.

### Statistics Module
```bash
./build/Statistics_Module/bin/Statistics_Module_main [input_file] [output_file] [target_columns]

# Example
./build/Statistics_Module/bin/Statistics_Module_main Food_and_Nutrition__.csv stats.txt "Weight,Height"
```