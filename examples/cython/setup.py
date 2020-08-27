from setuptools import setup, find_packages

import numpy as np
from Cython.Build import cythonize

setup(
    ext_modules=cythonize(),
    include_dirs=[np.get_include()]
)