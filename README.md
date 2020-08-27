# cygraph

*A graph theory library implemented in Cython*

## Installation


### With pip

```
pip install cygraph
```

### From source

```
git clone 'https://github.com/lol-cubes/cygraph.git'
cd cygraph
python3 setup.py install
```

## Usage

Here is a basic example of some features in cygraph.
```python
>>> import cygraph as cg
>>> import cygraph.algorithms as alg
>>> G = cg.graph(vertices=list(range(4)))
>>> G.add_edge(1, 2)
>>> G.add_edge(2, 3)
>>> G.add_edge(3, 1)
>>> G.get_children(1)
{2, 3}
>>> alg.get_number_components(G)
2
```

For more information on the Python and Cython APIs, see [the documentation](https://lol-cubes.github.io/cygraph)

## Contribution

I am currently not accepting any pull requests. This project is still in its very early stages and I have many things in mind to do with it. Once I get those things done, I will add contribution guidelines.