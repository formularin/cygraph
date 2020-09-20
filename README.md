# Cygraph

*A graph theory library implemented in Cython*

## Installation

First, try:

```bash
pip install cygraph
```

if that doesn't work, try:

```bash
pip install git+https://github.com/lol-cubes/cygraph
```

## Usage

### Python

Here is an example Python script that utilizes some of the features of cygraph.

```python
import cygraph as cg
import cygraph.algorithms as alg

G = cg.graph(vertices=list(range(4)))
G.add_edges({(1, 2), (2, 3), (3, 1)})
children = G.get_children(1)
print(f'{children=}')
components = alg.get_number_connected_components(G)
print(f'{components=}')
```

It should output:

```
children={2, 3}
components=2
```

### Cython

Here is the above script but with static typing in Cython.

```
#!python
#cython: language_level=3
import cygraph as cg  # cg.graph function.
cimport cygraph.graph_ as gp  # StaticGraph and DynamicGraph types.
cimport cygraph.algorithms as alg

cdef gp.DynamicGraph G = cg.graph(vertices=list(range(4)))
G.add_edges({(1, 2), (2, 3), (3, 1)})
cdef set children = G.get_children(1)
print(f'children={str(children)}')
cdef int components = alg.get_number_connected_components(G)
print(f'components={components}')
```

For more information on the Python and Cython APIs, see [the documentation](https://lol-cubes.github.io/cygraph)

---
**NOTE**

The current documentation is not up-to-date.

---

## Contribution

I am currently not accepting any pull requests. This project is still in its very early stages and I have many things in mind to do with it. Once I get those things done, I will add contribution guidelines.