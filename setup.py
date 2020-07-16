import sys
from setuptools import setup, find_packages


if sys.argv[1:3] == ["build_ext", "--inplace"]:
    # Compile and install cython code.

    import numpy as np
    from Cython.Build import cythonize

    setup(
        ext_modules=cythonize("cygraph/*.pyx"),
        include_dirs=[np.get_include]
    )

else:
    # Install the package without compiling cython code.
    setup(
        name="cygraph",
        packages=find_packages(),
        install_requires=[
            "cython",
            "numpy"
        ],
    )
