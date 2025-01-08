#!/bin/bash

# Exit on errors
set -e

# Get the directory of the current script
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Define the virtual environment directory
VENV_DIR="$SCRIPT_DIR/../venv"
REQUIREMENTS_FILE="$SCRIPT_DIR/requirements.txt"
PYTHON_TEST_SCRIPT="$SCRIPT_DIR/../tests/Statistics_Test.py"

# Check if Python is installed
if ! command -v python3 &> /dev/null; then
    echo "Python3 is not installed. Please install it before proceeding."
    exit 1
fi

# Create the virtual environment
if [ ! -d "$VENV_DIR" ]; then
    echo "Creating virtual environment in $VENV_DIR..."
    python3 -m venv "$VENV_DIR"
else
    echo "Virtual environment already exists at $VENV_DIR. Skipping creation."
fi

# Activate the virtual environment
source "$VENV_DIR/bin/activate"

# Install dependencies
if [ -f "$REQUIREMENTS_FILE" ]; then
    echo "Installing dependencies from $REQUIREMENTS_FILE..."
    pip install --upgrade pip
    pip install -r "$REQUIREMENTS_FILE"
else
    echo "Requirements file not found at $REQUIREMENTS_FILE. Skipping dependency installation."
fi