"""
Various graph-theory related algorithm implementations using cygraph classes.

Currently includes:
 - Shortest Path (Dijkstra's algorithm)
 - Minimum spanning tree (Borůvka's algorithm)
 - Graph partition (Karger's algorithm)
 - Number of components (depth-first search)
"""

import random

from graph cimport Graph


cdef set _dfs(Graph graph, v):
    """
    Finds all of the vertices in a component of a graph using a
    depth-first search.

    Args:
        cygraph.Graph graph: A graph.
        v: The root vertex of the search.
    
    Returns:
        The set of vertices in the component of the inputted graph that
        contains the inputted root vertex.
    
    Raises:
        ValueError: vertex is not in graph.
    """
    cdef set discovered = {v}
    for u in graph.get_children(v):
        if u not in discovered:
            discovered = discovered.union(_dfs(graph, u))
    return discovered


cpdef list find_shortest_path(Graph graph, v1, v2):
    """
    Takes a graph and finds the shortest path between two vertices in it.
    
    Args:
        cygraph.Graph graph: A graph.
        v1: One of the vertices in the inputted graph.
        v2: The other vertex in the inputted graph.
    
    Returns:
        The list of vertices that constitute the shortest path

    Raises:
        ValueError: One of the vertices is not in the graph.
    """


cpdef Graph get_min_spanning_tree(Graph graph):
    """
    Finds the minimum spanning tree of a graph.

    Args:
        cygraph.Graph graph: A graph.
    
    Returns:
        The graph that is the minimum spanning tree of the inputted graph.
    """


cpdef tuple partition_graph(Graph graph, bint static=False):
    """
    Partitions a graph into two graphs.

    Args:
        cygraph.Graph graph: A graph.
        bint static: Optional; Whether or not the graphs in the output
            should be static. Defaults to False.
    
    Returns:
        A tuple of two graphs that are the result of a random partition
        of the inputted graph. Has an (n choose 2)^-1 probability of
        finding the minimum cut partition where n = |V| and the
        inputted graph G = (V, E).
    """


cpdef list get_components(Graph graph, bint static=False):
    """
    Gets the components of the inputted graph, where a component is a
    subgraph in which any two vertices are connected to each other by paths.

    Args:
        cygraph.Graph graph: A graph.
        bint static: Optional; Whether or not the graphs in the output
            should be static. Defaults to False.
    
    Returns:
        A list of cygraph.Graph objects that are the components
        of the inputted graph.
    """
    cdef set discovered_vertices, discovered_components, vertices, new_component
    cdef object vertex, neighbor

    vertices = set(graph.vertices)
    discovered_vertices = set()
    # Contains tuples which contain all the nodes in each component.
    discovered_components = set()
    while len(discovered_vertices) < len(vertices):
        vertex = random.sample(vertices, 1)[0]
        new_component = _dfs(graph, vertex)
        discovered_vertices = discovered_vertices.union(new_component)
        discovered_components.add(tuple(new_component))
    
    # Get the induced subgraphs that are each of the components.
    cdef tuple component
    cdef Graph component_graph
    cdef set component_graphs = set()
    for component in discovered_components:
        if static:
            component_graph = graph.StaticGraph(
                directed=graph.directed, vertices=list(vertices))
        else:
            component_graph = graph.DynamicGraph(
                directed=graph.directed, vertices=list(vertices))

        for vertex in component:
            for neighbor in graph.childen(vertex):
                if neighbor in component:
                    component_graph.add_edge(vertex, neighbor)
        
        component_graphs.add(component_graph)

    return component_graphs