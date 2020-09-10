#!python
#cython: language_level=3
"""Functions involving articulation points.
"""

from cygraph.graph_ cimport Graph, StaticGraph, DynamicGraph


cdef set get_articulation_points(Graph graph):
    """Finds the articulation points in a graph using the lowpoint
    theorem.

    Implementation taken from here:
    https://github.com/networkx/networkx/blob/master/networkx/algorithms/components/biconnected.py

    Parameters
    ----------
    graph: cygraph.Graph
        An undirected graph.
    
    Returns
    -------
    set
        The articulation points in `graph`.

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    
    Examples
    --------
    >>> G = cg.graph(vertices=list(range(4)))
    >>> G.add_edge(0, 1)
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> alg.get_articulation_points(G)
    {1, 2}
    """
    if graph.directed:
        raise NotImplementedError("Cannot find the articulation points "
            "of an undirected graph.")
    
    cdef set visited = set()
    cdef set articulation_points = set()
    cdef int n_articulation_points = 0

    # Local loop variables.
    cdef object start, grandparent, parent, child
    cdef set children
    cdef dict discovery_times, lowpoints
    cdef int root_children
    cdef list stack
    for start in graph.vertices:
        # Each iteration that is not skipped is checking a new component.
        if start in visited:
            continue
        
        discovery_times = {start: 0}  # time of first discovery of vertex during search
        lowpoints = {start: 0}
        root_children = 0
        visited.add(start)
        stack = [(start, start, graph.get_children(start))]
        while stack:
            grandparent, parent, children = stack[-1]
            try:
                child = children.pop()
                if grandparent == child:
                    continue
                if child in visited:
                    if discovery_times[child] <= discovery_times[parent]:
                        # back edge
                        lowpoints[parent] = \
                            min(lowpoints[parent], discovery_times[child])
                else:
                    lowpoints[child] = discovery_times[child] = \
                        len(discovery_times)
                    visited.add(child)
                    stack.append((parent, child, graph.get_children(child)))
            except KeyError:  # Iterated through all children.
                stack.pop()
                if len(stack) > 1:
                    if lowpoints[parent] >= discovery_times[grandparent]:
                        articulation_points.add(grandparent)
                    lowpoints[grandparent] = min(lowpoints[parent],
                                                 lowpoints[grandparent])
                elif stack:  # length 1 so grandparent is root
                    root_children += 1
        # root is articulation point if it has more than 1 child
        if root_children > 1:
            articulation_points.add(start)

    return articulation_points


def py_get_articulation_points(graph):
    """Finds the articulation points in a graph using the lowpoint
    theorem.

    Parameters
    ----------
    graph: cygraph.Graph
        An undirected graph.
    
    Returns
    -------
    set
        The articulation points in `graph`.

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    
    Examples
    --------
    >>> G = cg.graph(vertices=list(range(4)))
    >>> G.add_edge(0, 1)
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> alg.get_articulation_points(G)
    {1, 2}
    """
    return get_articulation_points(graph)