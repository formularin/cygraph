"""A graph theory library for both python and cython.

Example usage:
>>> import cygraph as cg
>>> graph = cg.create_graph(static=True, vertices=['a', 'b', 'c'])
>>> graph.add_edge('a', 'b', weight=1.0)
>>> graph.edges
{('b', 'a')}
>>> graph.get_children('a')
{'b'}
"""

from .graph import StaticGraph, DynamicGraph


def create_graph(static=False, directed=False, vertices=[], graph=None):
    """Creates an instance of a Graph object.

    Parameters
    ----------
    static: bool, optional
        Setting this to true makes overall operations much faster, but
        it makes adding vertices much slower, especially on large
        graphs.
    directed: bool, optional
        Whether or not the graph has directed edges.
    vertices: list, optional
        A collection of initial vertices for the graph.
    graph: cygraph.Graph, optional
        A `cygraph.StaticGraph` or `cygraph.DynamicGraph` object to
        create a copy of.
    """

    kwargs = {
        'directed': directed,
        'vertices': vertices,
        'graph': graph
    }
    if static:
        return StaticGraph(**kwargs)
    else:
        return DynamicGraph(**kwargs)