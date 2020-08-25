#!python
#cython: language_level=3
"""Functions for identifying graph components.
"""

from ..graph_ cimport Graph, StaticGraph, DynamicGraph


cdef void _dfs(Graph graph, object v, set visited, list vertex_stack,
        bint backwards):
    """Finds all of the vertices in a component of a graph using a
    depth-first search.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    v
        The root vertex of the search.
    vertex_stack: list
        A stack containing vertices as they are finished in the DFS.
        Set to `[[]]` if you don't want this feature.
    backwards: bint
        Whether or not to use get_parents instead of get_children when
        finding vertex neighbors.

    Returns
    -------
    set
        Vertices in the component of `graph` that contains `v`.
    """
    # Default value of vertex_stack is [[]], because lists are not
    # hashable, therefore it can never actually be inputted by the user.

    cdef object vertex
    cdef set next_vertices
    cdef list stack = [v]
    
    while stack:
        vertex = stack.pop()
        if vertex not in visited:
            visited.add(vertex)
            if backwards:
                next_vertices = graph.get_parents(vertex)
            else:
                next_vertices = graph.get_children(vertex)
            stack += list(next_vertices)
            if vertex_stack != [[]]:
                vertex_stack.append(vertex)


cdef set _get_components(Graph graph, bint vertices):
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
            _dfs(graph, vertex, component_vertices, [[]], False)
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
    """Gets the strongly connected components of a graph

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    vertices: bint
        Whether or not to actually keep track of the set of vertices in
        each strongly connected component.

    Returns
    -------
    set
        The set of sets of vertices that are the strongly connnected
        components of `graph`, or a set containing a single value that
        is the number of strongly connected components in `graph`. Which
        one depends on the value of `vertices`.

    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    """
    if not graph.directed:
        raise NotImplementedError("Cannot get the strongly connected "
            "components of an undirected graph.")

    cdef list stack = []
    cdef set visited = set()
    cdef object v, u
    for v in graph.vertices:
        if v not in visited:
            _dfs(graph, v, visited, stack, False)

    visited = set()
    cdef set new_vertices = set()
    cdef set components = set()
    cdef int n_components = 0
    while stack:
        v = stack.pop()
        _dfs(graph, v, new_vertices, [[]], True)

        for v in new_vertices:
            if v in stack:
                stack.remove(v)
        
        visited |= new_vertices
        if vertices:
            components.add(frozenset(new_vertices))
        else:
            n_components += 1
    
    if vertices:
        return components
    else:
        return {n_components}


cdef set get_components(Graph graph, bint static):
    """Gets the connected components of a graph as connected graphs.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint
        Whether or not the graphs in the output should be static.

    Returns
    -------
    set
        Graphs that are the components of `graph`.

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    """

    cdef set discovered_components = _get_components(graph,
        vertices=True)

    # Get the induced subgraphs that are each of the components.
    cdef frozenset component
    cdef Graph component_graph
    cdef set component_graphs = set()
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

        component_graphs.add(component_graph)

    return component_graphs


cdef int get_number_components(Graph graph) except *:
    """Finds the number of connected components of a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        An undirected graph.

    Returns
    ------
    int
        The number of connected components in `graph`

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    """
    return _get_components(graph, False).pop()


cdef set get_strongly_connected_components(Graph graph, bint static):
    """Gets the strongly connected components of a graph

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint
        Whether or not the graphs in the output should be static.

    Returns
    -------
    set
        The set of graphs that are the strongly connnected components of
        `graph`

    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    """
    components = _get_strongly_connected_components(graph, True)

    cdef set graph_components = set()
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
    
    return graph_components


cdef int get_number_strongly_connected_components(Graph graph) except *:
    """Gets the number of strongly connected components of a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.

    Returns
    -------
    int
        The number of strongly connected components in `graph`

    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    """
    return _get_strongly_connected_components(graph, False).pop()


cpdef set py_get_components(Graph graph, bint static=False):
    """Gets the connected components of a graph as connected graphs.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint, optional
        Whether or not the graphs in the output should be static.

    Returns
    -------
    set
        Graphs that are the components of `graph`.

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    """
    return get_components(graph, static)


cpdef int py_get_number_components(Graph graph) except *:
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
    """
    return get_number_components(graph)


cpdef set py_get_strongly_connected_components(Graph graph, bint static=False):
    """Gets the strongly connected components of a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint, optional
        Whether or not the graphs in the output should be static.

    Returns
    -------
    set
        The set of graphs that are the strongly connnected components of
        `graph`

    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    """
    return get_strongly_connected_components(graph, static)


cpdef int py_get_number_strongly_connected_components(Graph graph) except *:
    """Gets the number of strongly connected components of a graph.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.

    Returns
    -------
    int
        The number of strongly connected components in `graph`

    Raises
    ------
    NotImplementedError
        `graph` is undirected.
    """
    return get_number_strongly_connected_components(graph)