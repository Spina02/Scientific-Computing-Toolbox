# Scientific Computing Toolbox
Homework 2 - Advanced Programming Course (UniTs)

how to run statistics module: 

```bash

g++ -std=c++17 -I./include -I./include/Stats_Module/getpot-c++/GetPot StatMod_main.cpp -o StatMod_main

./StatMod_main -i ../data/detailed_meals_macros_CLEANED.csv -o output.txt -c Ages
```
NOTE: the -c flag is optional, if not specified the program will compute the statistics for all the columns in the input file.

NOTE: in order to run the program you need to have the GetPot library (only linux compatible https://getpot.sourceforge.net/) and the Eigen library (https://gitlab.com/libeigen/eigen/-/tree/master/Eigen?ref_type=heads) located in the include/Stats_Module/ folder. 

## Dependencies

### muParser
The project requires ***muParser***. You can install it using your package manager:

- **Debian/Ubuntu**:

    ```bash
    sudo apt-get install libmuparser-dev
    ```
- **Arch-Linux**: 
    ```bash
    sudo pacman -S muparser
    ```

- **macOS**: 
    ```bash
    brew install muparser
    ```

If muParser is installed in a non-standard location, you can specify the paths:
```bash
MUPARSER_INCLUDE=/path/to/include MUPARSER_LIB=/path/to/lib make