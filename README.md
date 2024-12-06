# Scientific Computing Toolbox
Homework 2 - Advanced Programming Course (UniTs)

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