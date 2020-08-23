"""Functions for identifying graph components.
"""

from ..graph cimport Graph, StaticGraph, DynamicGraph


cpdef void _dfs(Graph graph, object v, set visited,
        list vertex_stack=[[]], bint backwards=False):
    """Finds all of the vertices in a component of a graph using a
    depth-first search.

    Parameters
    graph: cygraph.Graph
        A graph.
    v
        The root vertex of the search.
    vertex_stack: list, optional
        A stack containing vertices as they are finished in the DFS.
    backwards: bint, optional
        Whether or not to use get_parents instead of get_children when
        finding vertex neighbors.

    Returns
    -------
    set
        Vertices in the component of `graph` that contains `v`.
    """
    cdef object u
    cdef set next_vertices

    if v not in visited:
        visited.add(v)
        if backwards:
            next_vertices = graph.get_parents(v)
        else:
            next_vertices = graph.get_children(v)
        for u in next_vertices:
            _dfs(graph, u, visited, vertex_stack, backwards)
        if vertex_stack != [[]]:
            vertex_stack.append(v)


cpdef set find_articulation_points(Graph graph):
    """Takes a graph and finds all of its articulation points, where an
    articulation point is any point that, when removed, causes the graph
    to increase in number of components.

    Implementation taken from here:
    https://github.com/networkx/networkx/blob/master/networkx/algorithms/components/biconnected.py

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    
    Returns
    -------
    set
        The articulation points in `graph`.
    
    Raises
    ------
    NotImplementedError
        `graph` is directed.
    """
    if graph.directed:
        raise NotImplementedError(
            "cygraph.algorithms.partition_graph is not implemented for "
            "directed graphs."
        )

    cdef set visited = set()
    cdef set articulation_points = set()

    # Local loop variables.
    cdef object start, grandparent, parent, child
    cdef set children
    cdef dict discovery, low
    cdef int root_children
    cdef list stack
    for start in graph.vertices:
        # Each iteration that is not skipped is checking a new component.
        if start in visited:
            continue
        
        discovery = {start: 0}  # time of first discovery of vertex during search
        low = {start: 0}
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
                    if discovery[child] <= discovery[parent]:  # back edge
                        low[parent] = min(low[parent], discovery[child])
                else:
                    low[child] = discovery[child] = len(discovery)
                    visited.add(child)
                    stack.append((parent, child, graph.get_children(child)))
            except KeyError:  # Iterated through all children.
                stack.pop()
                if len(stack) > 1:
                    if low[parent] >= discovery[grandparent]:
                        articulation_points.add(grandparent)
                    low[grandparent] = min(low[parent], low[grandparent])
                elif stack:  # length 1 so grandparent is root
                    root_children += 1
        # root is articulation point if it has more than 1 child
        if root_children > 1:
            articulation_points.add(start)
    
    return articulation_points


cpdef set get_components(Graph graph, bint static=False):
    """Gets the components of the inputted graph, where a component is a
    subgraph in which any two vertices are connected to each other by
    a path. This only works for undirected graphs. If you want to get
    the equivalent of a directed graph, see
    cygraph.algorithms.get_strongly_connected_components

    Parameters
    ----------
        cygraph.Graph graph: A graph.
        bint static: Optional; Whether or not the graphs in the output
            should be static. Defaults to False.

    Returns
    -------
    set
        Graphs that are the components of `graph`.

    Raises
    ------
    NotImplementedError
        `graph` is directed.
    """
    if graph.directed:
        raise NotImplementedError("get_components only applies to undirected "
            "graphs. For directed graphs, see get_strongly_connected_components.")

    cdef list vertices = graph.vertices[:]
    cdef set discovered_components, new_component
    cdef object vertex, neighbor

    # Contains tuples which contain all the vertices in each component.
    discovered_components = set()
    while vertices:
        vertex = vertices[0]
        new_component = set()
        _dfs(graph, vertex, new_component)
        for vertex in new_component:
            vertices.remove(vertex)
        discovered_components.add(tuple(new_component))

    # Get the induced subgraphs that are each of the components.
    cdef tuple component
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


cpdef set get_strongly_connected_components(Graph graph, bint static=False):
    """Gets the strongly connected components of the inputted graph,
    where a strongly connected component is a subgraph in which every
    vertex is reachable by every other vertex. This only applies to
    directed graphs. If you want to get the equivalent of an undirected
    graph, see cygraph.algorithms.get_components

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
    if not graph.directed:
        raise NotImplementedError("get_strongly_connected_components only"
            "applies to directed graphs. For undirected graphs, see "
            "get_components.")
 
    cdef list stack = []
    cdef set visited = set()
    cdef object v, u
    for v in graph.vertices:
        if v not in visited:
            _dfs(graph, v, visited, vertex_stack=stack)

    visited = set()
    cdef set new_vertices = set()
    cdef set components = set()
    while stack:
        v = stack.pop()
        _dfs(graph, v, new_vertices, backwards=True)

        for v in new_vertices:
            if v in stack:
                stack.remove(v)
        
        visited |= new_vertices
        components.add(tuple(new_vertices))
    
    cdef set graph_components = set()
    cdef tuple comp
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