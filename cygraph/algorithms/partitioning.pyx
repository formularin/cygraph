#!python
#cython: language_level=3
"""Functions related to graph partitioning.
"""

from libc.stdlib cimport rand, srand

from cygraph.graph_ cimport Graph, StaticGraph, DynamicGraph


cdef extern from "time.h":
    ctypedef int time_t
    time_t time(time_t*)

srand(time(NULL))


cdef tuple partition_karger(Graph graph, bint static):
    """Partitions a graph into two graphs. Does not change the inputted
    graph in any way.

    Implemented from here:
    https://en.wikipedia.org/wiki/Karger%27s_algorithm

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint
        Whether or not the graphs in the output should be static.

    Returns
    -------
    tuple
        Two graphs that are the result of a random partition of `graph`,
        as well as the cutset of the partition.

    Raises
    ------
    NotImplementedError
        `graph` is directed.

    Examples
    --------
    >>> G = cg.graph(vertices=[1, 2])
    >>> G.add_edge(1, 2)
    >>> alg.partition_karger(G)
    (<DynamicGraph; vertices=[1]; edges=set()>, <DynamicGraph; vertices=[2]; edges=set()>, {(1, 2, 1.0)})
    """
    if graph.directed:
        raise NotImplementedError("Cannot perform Karger partition on "
            "directed graph.")
    if len(graph) < 2:
        raise ValueError(
            "Inputted graph has fewer than 2 vertices."
        )

    cdef DynamicGraph graph_copy = DynamicGraph(graph)
    cdef object u, v, uv, neighbor
    cdef set uv_neighbors
    cdef list neighbors
    cdef dict contracted_vertices = {v: {v} for v in graph}

    # Vertices in graph_copy will be pointers to lists, which contain
    # contracted vertices.
    while len(graph_copy) > 2:
        # Choose a random edge to contract.
        u = graph_copy.vertices[rand() % len(graph_copy.vertices)]
        neighbors = list(graph_copy.get_children(u))
        v = neighbors[rand() % len(neighbors)]

        while True:
            uv = hex(rand())[2:]
            if uv not in graph.vertices:
                break

        # Contract edge (u, v) to new vertex uv.
        uv_neighbors = graph_copy.get_children(u) | graph_copy.get_children(v)
        uv_neighbors.remove(u)
        uv_neighbors.remove(v)
        graph_copy.remove_vertex(u)
        graph_copy.remove_vertex(v)
        graph_copy.add_vertex(uv)
        for neighbor in uv_neighbors:
            graph_copy.add_edge(uv, neighbor)

        # Save which contracted vertices are now in uv.
        contracted_vertices[uv] = contracted_vertices[u] | contracted_vertices[v]
        del contracted_vertices[u]
        del contracted_vertices[v]

    # Generate new two graphs.
    cdef int i
    cdef tuple new_graphs
    if static:
        new_graphs = tuple([
            StaticGraph(
                directed=False,
                vertices=list(contracted_vertices[graph_copy.vertices[i]]))
            for i in range(2)
        ])
    else:
        new_graphs = tuple([
            DynamicGraph(
                directed=False,
                vertices=list(contracted_vertices[graph_copy.vertices[i]]))
            for i in range(2)
        ])
    # Add edges to graphs and determine cutset.
    cdef Graph new_graph
    cdef set cutset = set()
    for i, new_graph in enumerate(new_graphs):
        for u in new_graph.vertices:
            for v in graph.get_children(u):
                if v in new_graph.vertices:
                    if not new_graph.has_edge(u, v):
                        new_graph.add_edge(u, v, graph.get_edge_weight(u, v))
                elif i == 0:
                    # All of the cut edges will be encountered while
                    # filling in the first graph.
                    cutset.add((u, v, graph.get_edge_weight(u, v)))
    return (*new_graphs, cutset)


def py_partition_karger(graph, static=False):
    """Partitions a graph into two graphs. Does not change the inputted
    graph in any way.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint, optional
        Whether or not the graphs in the output should be static.

    Returns
    -------
    tuple
        Two graphs that are the result of a random partition of `graph`,
        as well as the cutset of the partition.

    Raises
    ------
    NotImplementedError
        `graph` is directed.

    Examples
    --------
    >>> G = cg.graph(vertices=[1, 2])
    >>> G.add_edge(1, 2)
    >>> alg.partition_karger(G)
    (<DynamicGraph; vertices=[1]; edges=set()>, <DynamicGraph; vertices=[2]; edges=set()>, {(1, 2, 1.0)})
    """
    return partition_karger(graph, static)
