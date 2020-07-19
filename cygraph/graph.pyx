"""Implementation of StaticGraph and DynamicGraph classes.
"""

import numpy as np


DTYPE = np.float64


cdef class StaticGraph:
    """
    A class representing a graph data structure.

    This is a directed graph class, although it will function as an
    undirected graph by creating directed edges both ways between two
    vertices. This class contains only basic functionality; algorithms
    are implemented externally.

    Adding vertices to a graph is relatively slow, expecially for
    already large graphs. If you are going to be adding lots of
    vertices to your graph, consider using cygraph.DynamicGraph.

    Args:
        bint directed: Optional; Whether or not the graph contains directed edges.
        list vertices: Optional; A list of vertices (can be any hashable type)
    
    Attributes:
        directed: Whether or not the graph contains directed edges.
        vertices: A list of the vertices in this graph.
        edges: A list of tuples contianing the two vertices of each edge.
    """

    # __adjacency_matrix_view[u][v] -> weight of edge between u and v.
    # None means there is no edge.
    cdef object __adjacency_matrix
    cdef double[:,:] __adjacency_matrix_view
    cdef dict __vertex_map  # Maps vertex names to numbers.
    cdef list __reverse_vertex_map  # Maps numbers to vertex names.

    cdef readonly bint directed
    cdef readonly list vertices

    def __cinit__(self, bint directed=False, list vertices=[]):

        self.directed = directed

        # Map vertex names to numbers and vice versa.
        self.__vertex_map = {}
        self.__reverse_vertex_map = []
        cdef int i
        cdef object v
        for i, v in enumerate(<list?>vertices):
            self.__vertex_map[v] = i
            self.__reverse_vertex_map.append(v)
        
        self.vertices = vertices[:]

        # Create adjacency matrix.
        cdef int size = len(vertices)
        self.__adjacency_matrix = np.empty((size, size), dtype=DTYPE)
        self.__adjacency_matrix.fill(None)
        self.__adjacency_matrix_view = self.__adjacency_matrix

    cpdef int __get_vertex_int(self, vertex) except -1:
        """
        Returns the int corresponding to a vertex.

        Args:
            vertex: A vertex in the graph.

        Returns:
            The int corresponding to the inputted vertex.

        Raises:
            ValueError: vertex is not in graph.
        """
        try:
            return <int>(self.__vertex_map[vertex])
        except KeyError:
            raise ValueError(f"{vertex} is not in graph.")

    cpdef void add_edge(self, v1, v2, double weight=1.0) except *:
        """
        Adds edge to graph between two vertices with a weight.

        Args:
            v1: One of the edge's vertices.
            v2: One of the edge's vertices.
            double weight: Optional; The weight of the edge.

        Raises:
            ValueError: At least one of the inputted vertices is not in the graph.
        """
        cdef int u = self.__get_vertex_int(v1)
        cdef int v = self.__get_vertex_int(v2)

        self.__adjacency_matrix_view[u][v] = weight
        if not self.directed:
            self.__adjacency_matrix_view[v][u] = weight

    cpdef void add_vertex(self, v) except *:
        """
        Adds vertex to the graph.

        Args:
            v: A vertex of any hashable type.
        
        Raises:
            TypeError: vertex type is not hashable.
        """
        # Map vertex name to number.
        cdef int vertex_number = len(<dict>self.__vertex_map)
        self.__vertex_map[v] = vertex_number
        self.__reverse_vertex_map.append(v)
        self.vertices.append(v)

        # Add new row.
        new_row = np.empty(vertex_number)
        new_row.fill(None)
        np.append(self.__adjacency_matrix, np.array([new_row]))

        # Add new column.
        new_column = np.empty(vertex_number + 1)
        new_column.fill(None)
        np.append(self.__adjacency_matrix, np.array([new_column]), axis=1)

    cpdef set get_children(self, vertex):
        """
        Returns the names of all the child vertices of a given vertex.
        Equivalent to neighbors if all edges are undirected.

        Args:
            vertex: A vertex in the graph.

        Returns:
            A set of the child vertices of the inputted vertex.

        Raises:
            ValueError: The inputted vertex is not in the graph.
        """
        cdef set children = set()
        cdef int u, v

        v = self.__get_vertex_int(vertex)

        for u in range(<int>len(self.vertices)):
            if self.__adjacency_matrix_view[v][u] is not None:
                children.add(self.__reverse_vertex_map[u])

        return children

    cpdef set get_descendants(self, vertex):
        """
        Returns the names of all the descendant vertices of a given vertex.

        Args:
            vertex: A vertex in the graph.
        
        Returns:
            A set of the descendant vertices of the input vertex.
        
        Raises:
            ValueError: The inputted vertex is not in the graph.
        """
        cdef set descendants = set()
        cdef object child

        for child in self.get_children(vertex):
            descendants = descendants.union(self.get_children(child))
        return descendants
        

    @property
    def edges(self):
        """
        A list of tuples contianing the two vertices of each edge.
        """
        cdef int u, v, n_vertices
        cdef set edges = set()
        cdef set edge

        n_vertices = len(self.vertices)

        if self.directed:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    if self.__adjacency_matrix_view[u][v] is not None:
                        edges.add((u, v))
        else:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    if self.__adjacency_matrix_view[u][v] is not None:
                        edge = {u, v}
                        if edge not in edges:
                            edges.add(edge)
            edges = {tuple(edge) for edge in edges}

        return edges


cdef class DynamicGraph:
    """
    A class representing a graph data structure.

    This is a directed graph class, although it will function as an
    undirected graph by creating directed edges both ways between two
    vertices. This class contains only basic functionality; algorithms
    are implemented externally.

    Adding vertices to a graph is faster than with a StaticGraph, but
    overall performance is comprimised.

    Args:
        bint directed: Optional; Whether or not the graph contains directed edges.
        list vertices: A list of the vertices in this graph.
    
    Attributes:
        directed: Whether or not the graph contains directed edges.
        vertices: A list of the vertices in this graph.
        edges: A list of tuples contianing the two vertices of each edge.
    """

    # __adjacency_matrix[u][v] -> weight of edge between u and v.
    # None means there is no edge.
    cdef list __adjacency_matrix
    cdef dict __vertex_map
    cdef list __reverse_vertex_map

    cdef readonly bint directed
    cdef readonly list vertices

    def __cinit__(self, bint directed=False, list vertices=[]):

        self.directed = directed

        # Map vertex names to numbers and vice versa.
        self.__vertex_map = {}
        self.__reverse_vertex_map = []
        cdef int i
        cdef object v
        for i, v in enumerate(<list?>vertices):
            self.__vertex_map[v] = i
            self.reverse_vertex_map.append(v)

        self.vertices = vertices[:]

        # Create adjacency matrix.
        cdef int size = len(vertices)
        self.__adjacency_matrix = []
        for i in range(size):
            self.__adjacency_matrix.append([])
            for _ in range(size):
                self.__adjacency_matrix[i].append(None)
    
    cpdef int __get_vertex_int(self, vertex) except -1:
        """
        Returns the int correspoding to a vertex.

        Args:
            vertex: A vertex in the graph.
        
        Returns:
            The int corresponding to the inputted vertex.

        Raises:
            ValueError: vertex is not in graph.
        """
        try:
            return <int>(self.vertex_map[vertex])
        except KeyError:
            raise ValueError(f"{vertex} is not in graph.")
    
    cpdef void add_edge(self, v1, v2, double weight=1.0) except *:
        """
        Adds edge to the graph between two vertices with a weight.

        Args:
            v1: One of the edges's vertices.
            v2: One of the edges's vertices.
            double weight: Optional; The weight of the edge.
        
        Raises:
            ValueError: At least one of the inputted vertices is not in the graph.
        """
        cdef int u = self.__get_vertex_int(v1)
        cdef int v = self.__get_vertex_int(v2)

        self.__adjacency_matrix[u][v] = weight
        if not self.directed:
            self.__adjacency_matrix[v][u] = weight
    
    cpdef void add_vertex(self, v) except *:
        """
        Adds vertex to the graph.

        Args:
            v: A vertex of any hashable type.
        
        Raises:
            TypeError: vertex type is not hashable.
        """
        # Map vertex name to number.
        cdef int vertex_number = len(<dict>self.__vertex_map)
        self.__vertex_map[v] = vertex_number
        self.__reverse_vertex_map.append(v)

        # Add new row.
        cdef list new_row = [None for _ in range(vertex_number + 1)]
        self.__adjacency_matrix.append(new_row)
        
        # Add new column.
        cdef int i
        for i in range(<int> (len(self.__adjacency_matrix) - 1)):
            self.__adjacency_matrix[i].append(None)
    
    cpdef set get_children(self, vertex):
        """
        Returns the names of all the child vertices of a given vertex.
        Equivalent to neighbors if all edges are undirected.

        Args:
            vertex: A vertex in the graph.

        Returns:
            A set of the child vertices of the inputted vertex.

        Raises:
            ValueError: The inputted vertex is not in the graph.
        """
        cdef set children = set()
        cdef int u, v

        v = self.__get_vertex_int(vertex)

        for u in range(<int>len(self.vertices)):
            if self.__adjacency_matrix[v][u] is not None:
                children.add(self.__reverse_vertex_map[u])

        return children

    cpdef set get_descendants(self, vertex):
        """
        Returns the names of all the descendant vertices of a given vertex.

        Args:
            vertex: A vertex in the graph.
        
        Returns:
            A set of the descendant vertices of the input vertex.
        
        Raises:
            ValueError: The inputted vertex is not in the graph.
        """
        cdef set descendants = set()
        cdef object child

        for child in self.get_children(vertex):
            descendants.add(child)
            descendants = descendants.union(self.get_descendants(child))
        return descendants

    @property
    def edges(self):
        """
        A set of tuples contianing the two vertices of each edge.
        """
        cdef int u, v, n_vertices
        cdef set edges = set()
        cdef set edge

        n_vertices = len(self.vertices)

        if self.directed:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    if self.__adjacency_matrix[u][v] is not None:
                        edges.add((u, v))
        else:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    if self.__adjacency_matrix[u][v] is not None:
                        edge = {u, v}
                        if edge not in edges:
                            edges.add(edge)
            edges = {tuple(edge) for edge in edges}

        return edges