# Cygraph Examples

Various programs that use the Cygraph library to demonstrate its API in
both Python and Cython.

## Compiling Cython

(from the examples/cython directory)

```bash
python3 setup.py build_ext --inplace
```

## Running the examples

For the examples that are script-like, you can run the python code with the standard `python3 module.py` command syntax.

However, for running cython code, you will have to import the module from python (ether in an interactive shell, or in a python module.)

(from the project root directory)

```python
>>> from examples.cython import example_module
```