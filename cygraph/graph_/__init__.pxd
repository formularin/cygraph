# !python
# cython: language_level=3


from libcpp.pair import pair
from libcpp.unordered_set cimport unordered_set
from libcpp.vector cimport vector


cdef extern from "../../include/graph/adjacency_list.hpp" namespace "cygraph":

    cdef cppclass AdjacencyListGraph[Vertex, EdgeWeight]:
        """A graph class implemented using an adjacency list. Vertex type must be C++ hashable.
        """

        AdjacencyListGraph() except +
        """Default constructor.
        """

        AdjacencyListGraph(bint directed, const unordered_set[Vertex]& vertices) except +
        """Constructs an AdjacencyListGraph object.

        Parameters
        ----------
        directed: whether or not the graph is directed.
        vertices: a starting set of vertices for the graph.
        """

        EdgeWeight get_edge_weight(const Vertex& u, const Vertex& v) except +
        """Returns the weight of an edge (u, v).
        """

        void add_vertex(const Vertex& v) except +
        """Adds a vertex to the graph.

        Raises a ValueError if v is already in the graph.
        """

        void add_vertices(const unordered_set[Vertex]& vertices) except  +
        """Adds a set of vertices to the graph.

        Raises a ValueError if any of the vertices are already in the graph.
        """

        void remove_vertex(const Vertex& v) except +
        """Removes a vertex from the graph.

        Raises a ValueError if the vertex isn't in the graph.
        """

        void remove_vertices(const unordered_set[Vertex]& vertices) except +
        """Removes a set of vertices from the graph.

        Raises a ValueError if any of the vertices aren't in the graph.
        """

        void set_edge_weight(const Vertex& u, const Vertex& v, const EdgeWeight& weight) except +
        """Sets the weight of the edge (u, v).

        Raises a ValueError if one or more of the vertices are not in the graph.
        """

        void remove_edge(const Vertex& u, const Vertex& v) except +
        """Removes an edge from the graph.
        
        Raises a ValueError if attempting to remove an edge that doesn't exist, including if one or
        more of the vertices is not in the graph.
        """

        void remove_edges(const vector[pair[Vertex, Vertex]]& edges) except +
        """Removes a set of edges from the graph.

        Raises a ValueError if any of the edges don't exist, including if one or more of the
        vertices are not in the graph.
        """

        bint has_edge(const Vertex &u, const Vertex& v) except +
        """Returns whether or not a given edge is in the graph.
        
        If one or more of the vertices are not in the graph, false is returned and a warning is 
        printed.
        """

        bint has_vertex(const Vertex& v) except +
        """Returns whether or not a vertex is in the graph.
        """

        unordered_set[Vertex] get_children(const Vertex& v) except +
        """Returns the children of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_parents.
        """

        unordered_set[Vertex] get_parents(const Vertex& v) except +
        """Returns the parents of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_children.
        """

    cdef cppclass UndirectedAdjacencyListGraph[Vertex]:
        """A graph class implemented using an adjacency list, but optimized for undirected graphs
        (boolean edge weights).

        Implemented as a separate class from AdjacencyListGraph[Vertex, EdgeWeight] because Cython
        does not support C++ partial specialization as of version 0.29.21.
        """

        UndirectedAdjacencyListGraph() except +
        """Default constructor.
        """

        UndirectedAdjacencyListGraph(bint directed, const unordered_set[Vertex]& vertices) except +
        """Constructs and UndirectedAdjacencyListGraph.

        Parameters
        ----------
        directed: whether or not the graph is directed.
        vertices: a starting set of vertices for the graph.
        """

        bint get_edge_weight(const Vertex& v) except +
        """Returns whether or not an edge exists. An alias to has_edge except raises a ValueError
        if one or more of the vertices is not in the graph.
        """

        void add_vertex(const Vertex& v) except +
        """Adds a vertex to the graph.

        Raises a ValueError if v is already in the graph.
        """

        void add_vertices(const unordered_set[Vertex]& vertices) except +
        """Adds an array of vertices to the graph.

        Raises a ValueError if any of the inputted vertices are already in the graph.
        """

        void remove_vertex(const Vertex& v) except +
        """Removes a vertex from the graph.

        Raises a ValueError if attempting to remove a vertex that isn't in the graph.
        """

        void remove_vertices(const unordered_set[Vertex]& vertices) except +
        """Removes a set of vertices from the graph.

        Raises a ValueError if any of the vertices aren't in the graph.
        """

        void set_edge_weight(const Vertex& u, const Vertex& v, const bint& weight) except +
        """Adds or removes an edge between two vertices in the graph.
        """

        void add_edge(const Vertex& u, const Vertex& v) except +
        """Adds an edge between two vertices in the graph.
        """

        void add_edges(const vector[pair[Vertex, Vertex]]& edges) except +
        """Adds multiple edges to the graph.
        """

        void remove_edge(const Vertex& u, const Vertex& v) except +
        """Removes an edge from the graph.

        Raises a ValueError if attempting to remove an edge that doesn't exist, including if one or
        more of the vertices is not in the graph.
        """

        void remove_edges(const vector[pair[Vertex, Vertex]]& edges) except +
        """Removes a set of edges from the graph.

        Raises a ValueError if any of the edges don't exist, including if one or more of the
        vertices are not in the graph.
        """

        bint has_edge(const Vertex& u, const Vertex& v) except +
        """Returns whether or not a given edge is in the graph. If one or more of the vertices are
        not in the graph, false is returned and a warning is printed.
        """

        unordered_set[Vertex] get_children(const Vertex& v) except +
        """Returns the children of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_parents.
        """

        unordered_set[Vertex] get_parents(const Vertex& v) except +
        """Returns the parents of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_children.
        """


cdef extern from "../../include/graph/adjacency_matrix.hpp" namespace cygraph:
    cdef cppclass AdjacencyMatrixGraph[Vertex, EdgeWeight]:
        """A graph class implemented using an adjacency matrix. Vertex type must be C++ hashable.
        """

        AdjacencyMatrixGraph() except +
        """Default constructor
        """

        AdjacencyMatrixGraph(bint directed, const unordered_set[Vertex]& vertices) except +
        """Constructs an AdjacencyMatrixGraph object.

        Parameters
        ----------
        directed: whether or not the graph is directed.
        vertices: a starting set of vertices for the graph.
        """

        vector[Vertex] get_vertices() except +
        """Returns the vertices in the graph.
        """

        EdgeWeight get_edge_weight(const Vertex& u, const Vertex& v) except +
        """Returns the weight of an edge (u, v).

        Raises a ValueError if edge does not exist.
        """

        void add_vertex(const Vertex& v) except +
        """Adds a vertex to the graph.

        Raises a ValueError if v is already in the graph.
        """

        void add_vertices(const unordered_set[Vertex]& vertices) except +
        """Adds a set of vertices to the graph.
        
        Raises a ValueError if any of the vertices are already in the graph.
        """

        void remove_vertex(const Vertex& v) except +
        """Removes a vertex from the graph.

        Raises a ValueError if the vertex is not in the graph.
        """

        void remove_vertices(const unordered_set[Vertex]& vertices) except +
        """Removes a set of vertices from the graph.

        Raises a ValueError if any of the vertices aren't in the graph.
        """

        void set_edge_weight(const Vertex& u, const Vertex& v, const EdgeWeight& weight) except +
        """Sets the weight of the edge (u, v).

        Raises a ValueError if one or more of the vertices are not in the graph.
        """

        void remove_edge(const Vertex& u, const Vertex& v) except +
        """Removes an edge from the graph.
        
        Raises a ValueError if attempting to remove an edge that doesn't exist, including if one or
        more of the vertices is not in the graph.
        """

        void remove_edges(const vector[pair[Vertex, Vertex]]& edges) except +
        """Removes a set of edges from the graph.

        Raises a ValueError if any of the edges don't exist, including if one or more of the
        vertices are not in the graph.
        """

        bint has_edge(const Vertex &u, const Vertex& v) except +
        """Returns whether or not a given edge is in the graph.

        If one or more of the vertices are not in the graph, false is returned and a warning is 
        printed.
        """

        bint has_vertex(const Vertex& v) except +
        """Returns whether or not a vertex is in the graph.
        """

        unordered_set[Vertex] get_children(const Vertex& v) except +
        """Returns the children of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_parents.
        """

        unordered_set[Vertex] get_parents(const Vertex& v) except +
        """Returns the parents of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_children.
        """