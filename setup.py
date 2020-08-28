import sys
from setuptools import setup, find_packages

import numpy as np
from Cython.Build import cythonize


with open('README.md', 'r') as f:
    long_description = f.read()


setup(
    name='cygraph',
    version='0.1.1',
    author='Arin Khare',
    author_email='arinmkhare@gmail.com',
    description='A graph theory library written in Cython',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/lol-cubes/cygraph',
    packages=find_packages(),
    ext_modules=cythonize(
        ['cygraph/*.pyx', 'cygraph/algorithms/*.pyx',
            'cygraph/graph_/*.pyx']
    ),
    include_dirs=[np.get_include()]
)