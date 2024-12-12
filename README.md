# Scientific Computing Toolbox

## Homework 


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
./build/Interpolation_Module/bin/Interpolation_Module_main

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
./build/Statistics_Module/bin/Statistics_Module_main data/Food_and_Nutrition__.csv output/stats.txt "Weight,Height"
```