"""
Various graph-theory related algorithm implementations using cygraph classes.

Currently includes:
 - Shortest Path (Dijkstra's algorithm)
 - Minimum spanning tree (Borůvka's algorithm)
 - Graph partition (Karger's algorithm)
 - Number of components (depth-first search)
"""

cimport graph


cpdef list find_shortest_path(graph.Graph graph, v1, v2):
    """
    Takes a graph and finds the shortest path between two vertices in it.
    
    Args:
        cygraph.graph.Graph graph: A graph.
        v1: One of the vertices in the inputted graph.
        v2: The other vertex in the inputted graph.
    
    Returns:
        The list of vertices that constitute the shortest path

    Raises:
        ValueError: One of the vertices is not in the graph.
    """


cpdef graph.Graph get_min_spanning_tree(graph.Graph graph):
    """
    Finds the minimum spanning tree of a graph.

    Args:
        cygraph.graph.Graph graph: A graph.
    
    Returns:
        The graph that is the minimum spanning tree of the inputted graph.
    """


cpdef tuple partition_graph(graph.Graph graph, bint static=False):
    """
    Partitions a graph into two graphs.

    Args:
        cygraph.graph.Graph graph: A graph.
        bint static: Optional; Whether or not the graphs in the output
            should be static. Defaults to False.
    
    Returns:
        A tuple of two graphs that are the result of a random partition
        of the inputted graph. Has an (n choose 2)^-1 probability of
        finding the minimum cut partition where n = |V| and the
        inputted graph G = (V, E).
    """


cpdef list get_components(graph.Graph graph, bint static=False):
    """
    Gets the components of the inputted graph, where a component is a
    subgraph in which any two vertices are connected to each other by paths.

    Args:
        cygraph.graph.Graph graph: A graph.
        bint static: Optional; Whether or not the graphs in the output
            should be static. Defaults to False.
    
    Returns:
        A list of cygraph.graph.Graph objects that are the components
        of the inputted graph.
    """