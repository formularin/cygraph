"""A graph theory library implemented in Cython.

Examples
--------
>>> import cygraph as cg
>>> import cygraph.algorithms as alg
>>> G = cg.graph(vertices=list(range(4)))
>>> G.add_edge(1, 2)
>>> G.add_edge(2, 3)
>>> G.add_edge(3, 1)
>>> G.get_children(1)
{2, 3}
>>> alg.get_number_connected_components(G)
2
"""

import numpy as np

from cygraph.graph_ import Graph, DynamicGraph, StaticGraph


__version__ = '0.2.1'


def graph(directed=False, vertices=[], graph_=None, adjacency_matrix=None,
        adjacency_list=[], static=False):
    """Create an instance of a cygraph.Graph object.

    Parameters
    ----------
    directed: bint, optional
        Whether to create a directed graph (otherwise undirected).
    vertices: list, optional
        A list of the vertices in this graph.
    graph_: cygraph.Graph, optional
        A graph to create a copy of. This overrides all of the following
        parameters.
    adjacency_matrix: list or np.ndarray, optional
        An adjanceny matrix for creating a graph that has edges to start
        with. If static, must be np.ndarray, if not static, must be list.
    adjacency_list: list, optional
        An adjacency list for creating a graph that has edges to start
        with.
    static: bint, optional
        Whether to create a cygraph.StaticGraph or cygraph.DynamicGraph
        instance. Setting this to True will mean algorithms will mean
        all operations will run faster, but adding vertices after
        initialization will be slow.

    Returns
    -------
    cygraph.Graph
        A graph.
    """

    if not isinstance(adjacency_matrix, np.ndarray):
        if adjacency_matrix == [] or adjacency_matrix is None:
            if static:
                adjacency_matrix = None
            else:
                adjacency_matrix = []

    kwargs = {
        'graph': graph_,
        'directed': directed,
        'vertices': vertices,
        'adjacency_matrix': adjacency_matrix,
        'adjacency_list': adjacency_list
    }
    if static:
        return StaticGraph(**kwargs)
    else:
        return DynamicGraph(**kwargs)
