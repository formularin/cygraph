"""A graph theory library for both python and cython.

Example usage:
    >>> import cygraph

    >>> graph = cygraph.create_graph(static=True, vertices=['a', 'b', 'c'])
    >>> graph.add_edge('a', 'b', weight=1.0)
    >>> graph.edges
    {('b', 'a')}
    >>> graph.get_children('a')
    {'b'}
"""

from .graph import StaticGraph, DynamicGraph


def create_graph(static=False, directed=False, vertices=[], graph=None):
    """
    Creates an instance of a Graph object.

    Args:
        static: Optional; Setting this to true makes overall operations
            much faster, but it makes adding vertices much slower,
            especially on large graphs.
        directed: Optional; Whether or not the graph has directed edges.
        vertices: Optional; A collection of initial vertices for the graph.
        graph: Optional; A `cygraph.graph.StaticGraph` or
            `cygraph.graph.DynamicGraph` object.
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