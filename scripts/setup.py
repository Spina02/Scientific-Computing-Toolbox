from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import os 
import sys 
import glob 


# Get the root directory of the project
root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "../"))

# Ensure that the directory for extensions exists
package_dir = os.path.join(os.path.dirname(__file__), 'scientific_toolbox')
if not os.path.exists(package_dir):
    os.makedirs(package_dir)

class BuildExt(build_ext):
    """Custom build_ext to ensure pybind11 submodule is used."""
    def build_extensions(self):
        pybind_include = os.path.join(root_dir, "extern/pybind11/include")
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
    long_description=open(os.path.join(root_dir, "README.md")).read(),
    long_description_content_type='text/markdown',
    ext_modules=[
        Extension(
            'scientific_toolbox.interpolation',
            sources = [
                os.path.join(root_dir, "python_bindings/interpolation_bindings.cpp"),
                *glob.glob(os.path.join(root_dir, "src/Interpolation_Module/*.cpp")),
            ],
            include_dirs=[
                os.path.join(root_dir, "extern/pybind11/include"), 
                os.path.join(root_dir, "include")
                ],
            extra_compile_args=["-std=c++17"] + gsl_cflags,
            extra_link_args=gsl_libs,
            language='c++',
        ),
        Extension(
            'scientific_toolbox.ode',
            sources = [
                os.path.join(root_dir, "python_bindings/ode_bindings.cpp"),
                *glob.glob(os.path.join(root_dir, "src/ODE_Module/*.cpp")),
                ],
            include_dirs=[os.path.join(root_dir, "extern/pybind11/include"), os.path.join(root_dir, "include")],
            language='c++',
            extra_compile_args=["-std=c++17"] + eigen_cflags + muparser_cflags,
            extra_link_args=eigen_libs + muparser_libs,
        ),
        Extension(
            'scientific_toolbox.statistics',
            sources = [
                os.path.join(root_dir, "python_bindings/statistics_bindings.cpp"),
                *glob.glob(os.path.join(root_dir, "src/Statistics_Module/*.cpp")),
                ],
            include_dirs=[
                os.path.join(root_dir, "extern/pybind11/include"), 
                os.path.join(root_dir, "include")
                ],
            language='c++',
            extra_compile_args=["-std=c++17"] + eigen_cflags,
            extra_link_args=eigen_libs,
        )
    ],
    cmdclass={'build_ext': BuildExt},
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
)
