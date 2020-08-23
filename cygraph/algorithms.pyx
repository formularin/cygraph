"""Various graph-theory related algorithm implementations using cygraph
classes.
"""

from libc.stdlib cimport rand, srand

from graph cimport Graph, StaticGraph, DynamicGraph


cdef extern from "time.h":
    ctypedef int time_t
    time_t time(time_t*)

srand(time(NULL))


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
        

cpdef list find_shortest_path(Graph graph, object source, object target):
    """Takes a graph and finds the shortest path between two vertices in
    it.

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    source
        One of the vertices in `graph`.
    target
        The other vertex in `graph`.

    Returns
    -------
    list
        The list of vertices that constitute the shortest path between
        source and target.
    """
    cdef dict distances, previous_vertices
    cdef set unprocessed_vertices = set()
    # Minimum distance from target to each vertex.
    distances = {}
    # Maps vertices to the vertex that comes before it in the path.
    previous_vertices = {}

    for vertex in graph.vertices:
        distances[vertex] = None
        previous_vertices[vertex] = None
        unprocessed_vertices.add(vertex)
    distances[source] = 0

    cdef object u ,v
    while unprocessed_vertices != set():
        try:
            u = min([v for v in distances
                    if v in unprocessed_vertices and distances[v] is not None])
        except ValueError:
            raise ValueError(f"There is no path in {graph!r} from"
                             f"{source} to {target}")

        unprocessed_vertices.remove(u)
        if u == target:
            break

        for v in graph.get_children(u):
            alternate_distance = distances[u] + graph.get_edge_weight(u, v)
            if ((distances[v] is not None and alternate_distance < distances[v])
                    or distances[v] is None):
                distances[v] = alternate_distance
                previous_vertices[v] = u

    cdef list sequence = []
    u = target
    if previous_vertices[u] is not None or u == source:
        while u is not None:
            sequence.insert(0, u)
            u = previous_vertices[u]

    return sequence


cpdef tuple partition_graph(Graph graph, bint static=False):
    """Partitions a graph into two graphs. Does not change the inputted
    graph in any way.

    Implemented from here:
    https://en.wikipedia.org/wiki/Karger%27s_algorithm

    Parameters
    ----------
    graph: cygraph.Graph
        A graph.
    static: bint, optional
        Whether or not the graphs in the output should be static.

    Returns
    -------
    tuple
        Two graphs that are the result of a random partition of `graph`.
    
    Raises
    ------
    NotImplementedError
        `graph` is directed.
    """
    if graph.directed:
        raise NotImplementedError(
            "cygraph.algorithms.partition_graph is not implemented for"
            "directed graphs."
        )
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
                    new_graph.add_edge(u, v, graph.get_edge_weight(u, v))
                elif i == 0:
                    # All of the cut edges will be encountered while
                    # filling in the first graph.
                    cutset.add((u, v, graph.get_edge_weight(u, v)))
    return (*new_graphs, cutset)


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