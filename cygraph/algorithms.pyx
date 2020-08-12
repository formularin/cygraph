"""
Various graph-theory related algorithm implementations using cygraph classes.

Currently includes:
 - Shortest Path (Dijkstra's algorithm)
 - Minimum spanning tree (Borůvka's algorithm)
 - Graph partition (Karger's algorithm)
 - Number of components (depth-first search)
"""

import random

from graph cimport Graph, StaticGraph, DynamicGraph


cdef set _dfs(Graph graph, object v):
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
    cdef object u
    for u in graph.get_children(v):
        if u not in discovered:
            discovered = discovered.union(_dfs(graph, u))
    return discovered


cpdef list find_shortest_path(Graph graph, object source, object target):
    """
    Takes a graph and finds the shortest path between two vertices in it.

    Args:
        cygraph.Graph graph: A graph.
        source: One of the vertices in the inputted graph.
        target: The other vertex in the inputted graph.

    Returns:
        The list of vertices that constitute the shortest path between
        source and target.

    Raises:
        ValueError: One of the vertices is not in the graph.
        ValueError: There is no path from source to target.
    """
    cdef dict distances, previous_nodes
    cdef set unprocessed_nodes = set()
    # Minimum distance from target to each node.
    distances = {}
    # Maps nodes to the node that comes before it in the path.
    previous_nodes = {}

    for vertex in graph.vertices:
        distances[vertex] = None
        previous_nodes[vertex] = None
        unprocessed_nodes.add(vertex)
    distances[source] = 0

    cdef object u ,v
    while unprocessed_nodes != set():
        try:
            u = min([v for v in distances
                    if v in unprocessed_nodes and distances[v] is not None])
        except ValueError:
            raise ValueError(f"There is no path in {graph!r} from"
                             f"{source} to {target}")

        unprocessed_nodes.remove(u)
        if u == target:
            break

        for v in graph.get_children(u):
            alternate_distance = distances[u] + graph.get_edge_weight(u, v)
            if ((distances[v] is not None and alternate_distance < distances[v])
                    or distances[v] is None):
                distances[v] = alternate_distance
                previous_nodes[v] = u

    cdef list sequence = []
    u = target
    if previous_nodes[u] is not None or u == source:
        while u is not None:
            sequence.insert(0, u)
            u = previous_nodes[u]

    return sequence


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
    Partitions a graph into two graphs. Does not change the inputted
    graph in any way.

    Args:
        cygraph.Graph graph: A graph.
        bint static: Optional; Whether or not the graphs in the output
            should be static. Defaults to False.

    Returns:
        A tuple of two graphs that are the result of a random partition
        of the inputted graph. Has an (n choose 2)^-1 probability of
        finding the minimum cut partition where n = |V| and the
        inputted graph G = (V, E). Also returns cutset of partition.

        Therefore, the output type is (Graph, Graph, set<(object, object)>)
    """
    if graph.directed:
        raise NotImplementedError(
            "cygraph.algorithms.partition_graph is not implemented for directed graphs."
        )
    if len(graph) < 2:
        raise ValueError(
            "Inputted graph has fewer than 2 vertices."
        )

    cdef DynamicGraph graph_copy = DynamicGraph(graph)
    cdef object u, v, uv, neighbor
    cdef set uv_neighbors
    cdef dict contracted_vertices = {v: {v} for v in graph}

    # Vertices in graph_copy will be pointers to lists, which contain contracted vertices.
    while len(graph_copy) > 2:
        # Choose a random edge to contract.
        u = random.choice(graph_copy.vertices)
        v = random.sample(graph_copy.get_children(u), 1)[0]

        while True:
            # Roundabout way of getting random int because
            # `int(1048576 * random.random())` is significantly
            # faster than `random.randint(0, 1048576)`
            uv = hex(int(1048576 * random.random()))[2:]
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
                    new_graph.add_edge(u, v, graph.get_edge_weight(u, v))
                elif i == 0:
                    # All of the cut edges will be encountered while
                    # filling in the first graph.
                    cutset.add((u, v, graph.get_edge_weight(u, v)))
    return (*new_graphs, cutset)


cpdef set get_components(Graph graph, bint static=False):
    """
    Gets the components of the inputted graph, where a component is a
    subgraph in which any two vertices are connected to each other by
    paths. This only works for undirected graphs. If you want to get the
    components of a directed graph, see
    cygraph.algorithms.get_strongly_connected_components

    Args:
        cygraph.Graph graph: A graph.
        bint static: Optional; Whether or not the graphs in the output
            should be static. Defaults to False.

    Returns:
        A set of cygraph.Graph objects that are the components
        of the inputted graph.

    Raises:
        ValueError: The inputted graph is directed.
    """
    if graph.directed:
        raise ValueError("get_components only applies to undirected "
                         "graphs. For directed graphs, see "
                         "get_strongly_connected_components.")

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


cpdef list get_strongly_connected_components(Graph graph, bint static=False):
    """
    Gets the strongly connected components of the inputted graph, where
    a strongly connected component is a subgraph in which every vertex
    is reachable by every other vertex. This only applies to directed
    graphs. If you want to get the components of an undirected graph,
    see cygraph.algorithms.get_components

    Args:
        cygraph.Graph graph: A graph.
        bint static: Optional; Whether or not the graphs in the output
            should be static. Defaults to False.

    Returns:
        A list of cygraph.Graph objects that are the components
        of the inputted graph.

    Raises:
        ValueError: The inputted graph is undirected.
    """