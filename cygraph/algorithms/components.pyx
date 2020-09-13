#!python
#cython: language_level=3
"""Functions for identifying graph components.
"""

from collections import deque

from cygraph.graph_ cimport Graph, StaticGraph, DynamicGraph


# Global variables required for Tarjan's algorithm.
cdef int _index = 0
cdef list _stack = []
cdef dict _indices = {}
cdef dict _lowlinks = {}


cdef void _dfs(Graph graph, object root, set visited) except *:
    """Finds all of the vertices in a component of a graph using a
    depth-first search.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    root
        The root vertex of the search.
    visited: set
        The set of already visited vertices.
    """
    cdef object v, w
    cdef set next_vertices
    cdef list stack = [root]

    while stack:
        v = stack.pop()
        if v not in visited:
            visited.add(v)
            for w in graph.get_children(v):
                if w not in visited:
                    stack.append(w)


cdef set _strongconnect(Graph graph, object v):
    """The strongconnect function for Tarjan's strongly connected
    component algorithm.

    Parameters
    ----------
    graph: cygraph.Graph
        A directed graph.
    v
        A vertex to use as the root node of this component.
    
    Returns
    -------
    set
        The set of vertices in the same strongly connected component as
        `v`, including v.
    """

    global _index, _stack, _indices, _lowlinks

    _indices[v] = _index
    _lowlinks[v] = _index
    _index += 1
    _stack.append(v)

    cdef object w
    for w in graph.get_children(v):
        if w not in _indices:
            # Child has not yet been visited; recurse on it.
            _strongconnect(graph, w)
            _lowlinks[v] = min(_lowlinks[v], _lowlinks[w])
        elif w in _stack:
            # w is in stack and hence in the current SCC.
            # If w is not on stack, the w was already visited in a
            # previous depth 0 call of strongconnect.
            _lowlinks[v] = min(_lowlinks[v], _indices[w])
    
    # If v is a root node, pop the stack and generate an SCC.
    cdef set comp = set()
    if _lowlinks[v] == _indices[v]:
        while True:
            w = _stack.pop()
            comp.add(w)
            if w == v:
                break
    
    return comp


cdef set _get_connected_components(Graph graph, bint vertices):
    """Gets the components of a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    vertices: bint
        Whether to calculate vertices, or the number of components.
    
    Returns
    -------
    set
        Either a set of frozensets containing the vertices in each
        component, or a set containing a single value that is the number
        of components in the graph; depending on the value of the
        `vertices` parameter.
    """
    if graph.directed:
        raise NotImplementedError("Cannot get the connected components "
            "of a directed graph.")

    cdef set components = set()
    cdef set visited = set()
    cdef int n_components = 0
    cdef object vertex
    cdef set component_vertices

    for vertex in graph.vertices:
        if vertex not in visited:
            component_vertices = set()
            _dfs(graph, vertex, component_vertices)
            visited |= component_vertices
            if vertices:
                components.add(frozenset(component_vertices))
            else:
                n_components += 1
    
    if vertices:
        return components
    else:
        return {n_components}


cdef set _get_strongly_connected_components(Graph graph, bint vertices):
    """Gets the strongly connected components of a graph using
    Tarjan's algorithm.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    vertices: bint
        Whether or not to keep track of which vertices are in which
        strongly connected component.

    Returns
    -------
    set
        The set of sets of vertices that are the strongly connnected
        components of `graph`, or a set with a single element that is
        the number of strongly connected components in `graph` if
        `vertices` is set to False.

    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    """
    if not graph.directed:
        raise NotImplementedError("Cannot get the strongly connected "
            "components of an undirected graph.")

    global _index, _stack, _indices, _lowlinks
    _index = 0
    _stack = []
    _indices = {}
    _lowlinks = {}

    cdef dict bookkeeping_dict
    cdef set components = set()
    cdef set visited = set()
    cdef set comp
    cdef object v, w
    cdef int n_components = 0
    for v in graph.vertices:
        if v not in _indices:
            comp = _strongconnect(graph, v)
            visited |= comp
            _stack = []
            for bookkeeping_dict in (_indices, _lowlinks):
                for w in dict(bookkeeping_dict):
                    if w not in visited:
                        del bookkeeping_dict[w]
            
            if vertices:
                components.add(frozenset(comp))
            else:
                n_components += 1

    if vertices:
        return components
    else:
        return {n_components}


cdef list get_connected_components(Graph graph, bint static):
    """Gets the connected components of a graph as connected graphs.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint
        Whether or not the graphs in the output should be static.

    Returns
    -------
    list
        A list of the graphs that are the connected components of
        `graph`, in no particular order.

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    
    Examples
    --------
    >>> G = cg.graph(vertices=list(range(4)))
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> alg.get_connected_components(G)
    {<DynamicGraph; vertices=[1, 2, 3]; edges={(1, 2, 1.0), (2, 3, 1.0)}>, <DynamicGraph; vertices=[0]; edges=set()>}

    Notes
    -----
    To get the number of connected components in a graph,
    cygraph.algorithms.get_number_connected_components(G) is faster than
    len(cygraph.algorithms.get_connected_components(G))
    """

    cdef set discovered_components = _get_connected_components(graph,
        vertices=True)

    # Get the induced subgraphs that are each of the components.
    cdef frozenset component
    cdef Graph component_graph
    cdef list component_graphs = []
    for component in discovered_components:
        if static:
            component_graph = StaticGraph(
                directed=graph.directed, vertices=list(component))
        else:
            component_graph = DynamicGraph(
                directed=graph.directed, vertices=list(component))

        for vertex in component:
            for neighbor in graph.get_children(vertex):
                if neighbor in component:
                    component_graph.add_edge(vertex, neighbor)

        component_graphs.append(component_graph)

    return component_graphs


cdef int get_number_connected_components(Graph graph) except *:
    """Finds the number of connected components of a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        An undirected graph.

    Returns
    -------
    int
        The number of connected components in `graph`

    Raises
    ------
    NotImplementedError
        `graph` is directed.

    Examples
    --------
    >>> G = cg.graph(vertices=list(range(4)))
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> alg.get_number_connected_components(G)
    2
    """
    return _get_connected_components(graph, False).pop()


cdef list get_strongly_connected_components(Graph graph, bint static):
    """Gets the strongly connected components of a graph

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint
        Whether or not the graphs in the output should be static.

    Returns
    -------
    list
        A list of the graphs that are the strongly connected components
        of `graph`, in no particular order.

    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    
    Examples
    --------
    >>> G = cg.graph(directed=True, vertices=list(range(4)))
    >>> G.add_edge(0, 1)
    >>> G.add_edge(1, 0)
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> G.add_edge(3, 2)
    >>> alg.get_strongly_connected_components(G)
    {<DynamicGraph; vertices=[2, 3]; edges={(2, 3, 1.0), (3, 2, 1.0)}>, <DynamicGraph; vertices=[0, 1]; edges={(1, 0, 1.0), (0, 1, 1.0)}>}

    Notes
    -----
    To get the number of strongly connected components in a graph,
    cygraph.algorithms.get_number_strongly_connected_components(G) is
    faster than
    len(cygraph.algorithms.get_strongly_connected_components(G))
    """
    components = _get_strongly_connected_components(graph, True)

    cdef list graph_components = []
    cdef frozenset comp
    cdef Graph g
    for comp in components:
        if static:
            g = StaticGraph(directed=True, vertices=list(comp))
        else:
            g = DynamicGraph(directed=True, vertices=list(comp))

        for v in comp:
            for u in graph.get_children(v):
                if u in comp:
                    g.add_edge(v, u)
        
        graph_components.append(g)

    return graph_components


cdef int get_number_strongly_connected_components(Graph graph) except *:
    """Gets the number of strongly connected components in a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        A directed graph.
    
    Returns
    -------
    int
        The number of strongly connected components in `graph`.
    
    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    
    Examples
    --------
    >>> G = cg.graph(directed=True, vertices=list(range(4)))
    >>> G.add_edge(0, 1)
    >>> G.add_edge(1, 0)
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> G.add_edge(3, 2)
    >>> alg.get_number_strongly_connected_components(G)
    2
    """
    return _get_strongly_connected_components(graph, False).pop()


def py_get_connected_components(graph, static=False):
    """Gets the connected components of a graph as connected graphs.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint, optional
        Whether or not the graphs in the output should be static.

    Returns
    -------
    list
        A list of the graphs that are the connected components of
        `graph`, in no particular order.

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    
    Examples
    --------
    >>> G = cg.graph(vertices=list(range(4)))
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> alg.get_connected_components(G)
    {<DynamicGraph; vertices=[1, 2, 3]; edges={(1, 2, 1.0), (2, 3, 1.0)}>, <DynamicGraph; vertices=[0]; edges=set()>}
    """
    return get_connected_components(graph, static)


def py_get_number_connected_components(graph):
    """Finds the number of connected components of a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        An undirected graph.

    Returns
    ------
    The number of connected components in `graph`

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    
    Examples
    --------
    >>> G = cg.graph(vertices=list(range(4)))
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> alg.get_number_connected_components(G)
    2
    """
    return get_number_connected_components(graph)


def py_get_strongly_connected_components(graph, static=False):
    """Gets the strongly connected components of a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint, optional
        Whether or not the graphs in the output should be static.

    Returns
    -------
    list
        A list of the graphs that are the strongly connected components
        of `graph`, in no particular order.

    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    
    Examples
    --------
    >>> G = cg.graph(directed=True, vertices=list(range(4)))
    >>> G.add_edge(0, 1)
    >>> G.add_edge(1, 0)
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> G.add_edge(3, 2)
    >>> alg.get_strongly_connected_components(G)
    {<DynamicGraph; vertices=[2, 3]; edges={(2, 3, 1.0), (3, 2, 1.0)}>, <DynamicGraph; vertices=[0, 1]; edges={(1, 0, 1.0), (0, 1, 1.0)}>}
    """
    return get_strongly_connected_components(graph, static)


def py_get_number_strongly_connected_components(graph):
    """Gets the number of strongly connected components in a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        A directed graph.
    
    Returns
    -------
    int
        The number of strongly connected components in `graph`.

    Raises
    ------
    NotImplementedError
        `graph` is undirected.

    Examples
    --------
    >>> G = cg.graph(directed=True, vertices=list(range(4)))
    >>> G.add_edge(0, 1)
    >>> G.add_edge(1, 0)
    >>> G.add_edge(1, 2)
    >>> G.add_edge(2, 3)
    >>> G.add_edge(3, 2)
    >>> alg.get_number_strongly_connected_components(G)
    2
    """
    return get_number_strongly_connected_components(graph)