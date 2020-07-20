"""A graph theory library for both python and cython.
"""

from .graph import StaticGraph, DynamicGraph


def create_graph(static=False, directed=False, vertices=[]):
    """
    Creates an instance of a Graph object.

    Args:
        static: Optional; Setting this to true makes overall operations
            much faster, but it makes adding nodes much slower,
            especially on large graphs.
        directed: Optional; Whether or not the graph has directed edges.
        vertices: Optional; A collection of initial vertices for the graph.
    """

    args = [directed, vertices]
    if static:
        return StaticGraph(*args)
    else:
        return DynamicGraph(*args)