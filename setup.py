from setuptools import setup

import numpy as np
from Cython.Build import cythonize


with open('README.md', 'r') as f:
    long_description = f.read()


setup(
    name='cygraph',
    version='0.2.1',
    author='Arin Khare',
    author_email='arinmkhare@gmail.com',
    description='A graph theory library written in Cython',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/lol-cubes/cygraph',
    packages=['cygraph', 'cygraph/algorithms', 'cygraph/graph_'],
    ext_modules=cythonize(
        ['cygraph/algorithms/*.pyx', 'cygraph/graph_/*.pyx']
    ),
    include_dirs=[np.get_include()],
    install_requires=['numpy>=1.19.0'],
    package_data={
        'cygraph': ['*.pyx', '*.pxd'],
        'cygraph/algorithms': ['*.pyx', '*.pxd'],
        'cygraph/graph_': ['*.pyx', '*.pxd']
    },
    include_package_data=True
)