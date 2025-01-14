from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import subprocess
import os
import sys
import glob

class BuildExt(build_ext):
    """Custom build_ext to ensure pybind11 submodule is used."""
    def build_extensions(self):
        pybind_include = os.path.join("extern", "pybind11", "include")
        for ext in self.extensions:
            ext.include_dirs.append(pybind_include)
        super().build_extensions()


def get_pkg_config_flags(package_name, flag_type):
    """
    Use pkg-config to get flags for a given package.
    :param package_name: Name of the package (e.g., "gsl").
    :param flag_type: Type of flag ("--cflags" or "--libs").
    :return: A list of flags.
    """
    try:
        flags = subprocess.check_output(
            ["pkg-config", flag_type, package_name],
            universal_newlines=True,
        ).strip().split()
        return flags
    except subprocess.CalledProcessError:
        raise RuntimeError(f"pkg-config failed to find {package_name}")
    except FileNotFoundError:
        raise RuntimeError("pkg-config is not installed on your system")


# Get GSL compiler and linker flags
try:
    gsl_cflags = get_pkg_config_flags("gsl", "--cflags")
    gsl_libs = get_pkg_config_flags("gsl", "--libs")
except RuntimeError as e:
    print(e)
    sys.exit(1)

# Get eigen compiler and linker flags
try:
    eigen_cflags = get_pkg_config_flags("eigen3", "--cflags")
    eigen_libs = get_pkg_config_flags("eigen3", "--libs")
except RuntimeError as e:
    print(e)
    sys.exit(1)

# Get muparser compiler and linker flags
try:
    muparser_cflags = get_pkg_config_flags("muparser", "--cflags")
    muparser_libs = get_pkg_config_flags("muparser", "--libs")
except RuntimeError as e:
    print(e)
    sys.exit(1)

setup(
    name='scientific_toolbox',
    version='0.1.0',
    author='Giacomo Amerio, Christian Faccio, Andrea Spinelli',
    author_email='giacomoamerio24@gmail.com, christianfaccio@outlook.it, andreaspinelli2002@gmail.com',
    description='A scientific toolbox for data analysis',
    long_description=open("README.md").read(),
    long_description_content_type='text/markdown',
    
    # Trova automaticamente i package e i sotto-package
    packages=find_packages(
        include=['scientific_toolbox', 'scientific_toolbox.*']
    ),
    
    ext_modules=[
        Extension(
            'scientific_toolbox._interpolation',
            sources=[
                "src/python_bindings/interpolation_bindings.cpp",
                *glob.glob("src/Interpolation_Module/*.cpp"),

            ],
            include_dirs=[
                "extern/pybind11/include",
                "include/Interpolation_Module",
            ],
            extra_compile_args=["-std=c++17"] + gsl_cflags,
            extra_link_args=gsl_libs,
            language='c++',
        ),
        Extension(
            'scientific_toolbox._ode',
            sources=[
                "src/python_bindings/ode_bindings.cpp",
                *glob.glob("src/ODE_Module/*.cpp"),

            ],
            include_dirs=[
                "extern/pybind11/include",
                "include/ODE_Module",
            ],
            language='c++',
            extra_compile_args=["-std=c++17"] + eigen_cflags + muparser_cflags,
            extra_link_args=eigen_libs + muparser_libs,
        ),
        Extension(
            'scientific_toolbox._stats',
            sources=[
                "src/python_bindings/statistics_bindings.cpp",
                *glob.glob("src/Statistics_Module/*.cpp"),

            ],
            include_dirs=[
                "extern/pybind11/include",
                "include/Statistics_Module",
            ],
            language='c++',
            extra_compile_args=["-std=c++17"] + eigen_cflags,
            extra_link_args=eigen_libs,
        ),
        Extension(
            'scientific_toolbox.utilities',
            sources=[
                "src/python_bindings/utilities_bindings.cpp",

            ],
            include_dirs=[
                "extern/pybind11/include",
                "include",
            ],
            language='c++',
            extra_compile_args=["-std=c++17"],
        ),
    ],
    cmdclass={'build_ext': BuildExt},

    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
)