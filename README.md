# Scientific Computing Toolbox

## Homework 

Repository GitHub: https://github.com/christianfaccio/Scientific_Computing_Toolbox.git

This library has been created by Giacomo Amerio (giacomoamerio24@gmail.com), Christian Faccio (christianfaccio@outlook.it) and Andrea Spinelli (andreaspinelli2002@gmail.com). It is a toolbox written in C++ that contains three modules:

1) Statistics Module - template only, covers descriptive statistics and correlation analysis on a given dataset. For this module it's being used templates for functions' simplicity and different types of data input. 
2) Interpolation Module - covers four types of interpolation on a given set of points (Linear, Polynomial with Lagrange and Newton method, Cubic Spline). For this module it's being used templates for different data input (int and double). 
3) ODE Module - solves given differential equations in three ways (Forward Euler, Runge-Kutta-4, Explicit-Midpoint). This module includes templates and std::variant for different types of values.

All modules use inheritance for the natural form of hierarchy that exists in the various areas.

Individual contributions:
Giacomo Amerio developed the Statistics Module, and the general utilities of parsing and importing data. Christian Faccio developed the Interpolation Module. Andrea Spinelli developed the ODE Module and the makefile structure. All contributed to the debugging and optimisation phase, mostly Andrea who stimulated the whole group to look for the stars and make a great project.




## Project Structure
```bash
.
├── data/
├── include/
│   ├── Interpolation_Module/ 
│   ├── ODE_Module/
│   ├── Statistics_Module/
│   ├── Utilities/
│   ├── Interpolation_Module.hpp
│   ├── ODE_Module.hpp
│   ├── ScientificToolbox.hpp
│   └── Statistics_Module.hpp
├── main/
│   ├── Interpolation_Module_main.cpp
│   ├── ODE_Module_main.cpp
│   └── Statistics_Module_main.cpp
├── src/
│   ├── Interpolation_Module/ # Here you can find the Interpolation Module Makefile
│   ├── ODE_Module/           # Here you can find the ODE Module Makefile
│   └── Statistics_Module/    # Here you can find the Statistics Module Makefile
├── tests/
│   ├── Interpolation_Module_test.cpp
│   ├── ODE_Module_test.cpp
│   └── Statistics_Module_test.cpp
├── Makefile
├── MIT-License.txt
└── README.md
```



## Dependencies

### External Libraries
The project requires ***Eigen*** , ***GSL*** , ***muParser*** . You can install it using your package manager:

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

If any of this libraries is installed in a non-standard location, you can specify the paths:
```bash
MUPARSER_INCLUDE=/path/to/include MUPARSER_LIB=/path/to/lib make

GSL_INCLUDE=/path/to/include 
GSL_LIB=/path/to/lib make

EIGEN_INCLUDE=/path/to/include make
```


## Building

### Build all Modules
```bash
make 
``` 

### Build individual Modules
```bash
make interpolation
make ode
make statistics
```

> [!NOTE]
> Both test and run commands will automatically compile the code if needed.

## Testing

### Test all Modules
```bash
make test
```

### Test individual Modules
```bash
make test interpolation
make test ode
make test statistics
```

## Running

### Run all Modules
```bash 
make run
```

### Run individual Modules
```bash
make run interpolation
make run ode
make run statistics
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