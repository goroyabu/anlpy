#/// @file setup.py
#/// @detail Automatically generated by make_anlpy_project.sh 1.0.0
import os
import re
# import platform
import subprocess
import sys

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='.'):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    # def run(self):
    #     try:
    #         out = subprocess.check_output(['cmake', '--version'])
    #     except OSError:
    #         raise RuntimeError("CMake must be installed to build the following extensions: " +
    #                            ", ".join(e.name for e in self.extensions))

    #     if platform.system() == "Windows":
    #         cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
    #         if cmake_version < '3.1.0':
    #             raise RuntimeError("CMake >= 3.1.0 is required on Windows")

    #     for ext in self.extensions:
    #         self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # required for auto-detection & inclusion of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        debug = int(os.environ.get("DEBUG", 0)) if self.debug is None else self.debug
        cfg = 'Debug' if debug else 'Release'

        # CMake lets you override the generator - we need to check this.
        # Can be set with Conda-Build, for example.
        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

        # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
        # EXAMPLE_VERSION_INFO shows you how to pass a value into the C++ code
        # from Python.
        cmake_args = [
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(extdir),
            "-DPYTHON_EXECUTABLE={}".format(sys.executable),
            "-DCMAKE_BUILD_TYPE={}".format(cfg),  # not used on MSVC, but no harm
        ]
        build_args = []
        # Adding CMake arguments set as environment variable
        # (needed e.g. to build for ARM OSx on conda-forge)
        if "CMAKE_ARGS" in os.environ:
            cmake_args += [item for item in os.environ["CMAKE_ARGS"].split(" ") if item]

        # In this example, we pass in the version to C++. You might not need to.
        # cmake_args += [
        #     "-DEXAMPLE_VERSION_INFO={}".format(self.distribution.get_version())
        # ]

        if self.compiler.compiler_type != "msvc":
            # Using Ninja-build since it a) is available as a wheel and b)
            # multithreads automatically. MSVC would require all variables be
            # exported for Ninja to pick it up, which is a little tricky to do.
            # Users can override the generator with CMAKE_GENERATOR in CMake
            # 3.15+.
            if not cmake_generator:
                try:
                    import ninja  # noqa: F401

                    cmake_args += ["-GNinja"]
                except ImportError:
                    pass

        else:

            # Single config generators are handled "normally"
            single_config = any(x in cmake_generator for x in {"NMake", "Ninja"})

            # CMake allows an arch-in-generator style for backward compatibility
            contains_arch = any(x in cmake_generator for x in {"ARM", "Win64"})

            # Specify the arch if using MSVC generator, but only if it doesn't
            # contain a backward-compatibility arch spec already in the
            # generator name.
            if not single_config and not contains_arch:
                cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]

            # Multi-config generators have a different way to specify configs
            if not single_config:
                cmake_args += [
                    "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(cfg.upper(), extdir)
                ]
                build_args += ["--config", cfg]

        if sys.platform.startswith("darwin"):
            # Cross-compile support for macOS - respect ARCHFLAGS if set
            archs = re.findall(r"-arch (\S+)", os.environ.get("ARCHFLAGS", ""))
            if archs:
                cmake_args += ["-DCMAKE_OSX_ARCHITECTURES={}".format(";".join(archs))]

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators.
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            # self.parallel is a Python 3 only way to set parallel jobs by hand
            # using -j in the build_ext call, not supported by pip or PyPA-build.
            if hasattr(self, "parallel") and self.parallel:
                # CMake 3.12+ only.
                build_args += ["-j{}".format(self.parallel)]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args, cwd=self.build_temp
        )

        # if platform.system() == "Windows":
        #     cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
        #     if sys.maxsize > 2**32:
        #         cmake_args += ['-A', 'x64']
        #     build_args += ['--', '/m']
        # else:
        #     cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        #     build_args += ['--', '-j2']

        # env = os.environ.copy()
        # env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''), self.distribution.get_version())
        # if not os.path.exists(self.build_temp):
        #     os.makedirs(self.build_temp)
        # subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        # #subprocess.check_call(['cmake', '-G', 'Ninja', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        # subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)
        # # subprocess.check_call(['cmake', '--install', '.'], cwd=self.build_temp)

setup(
    name='ccana',
    version='20211025b',
    author='Goro Yabu',
    author_email='',
    description='',
    long_description='',
    ext_modules=[CMakeExtension('ccana')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
)

