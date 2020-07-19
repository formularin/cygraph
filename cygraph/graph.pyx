"""Implementation of the Graph class
"""

import numpy as np


DTYPE = np.float64


cdef class Graph:
    """
    A class representing a graph data structure.

    This is a directed graph class, although it will function as an
    undirected graph by creating directed edges both ways between two
    nodes. This class contains only basic functionality; algorithms
    are implemented externally.

    Adding nodes to a graph is relatively slow, so when possible list
    all nodes the graph will ever have during instantiation.

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
        self.vertices = vertices[:]

        # Map node names to numbers and vice versa.
        self.__vertex_map = {}
        self.__reverse_vertex_map = []
        cdef int i
        cdef object v
        for i, v in enumerate(<list?>vertices):
            self.__vertex_map[v] = i
            self.__reverse_vertex_map.append(v)

        # Create adjacency matrix.
        cdef int size = len(vertices)
        self.__adjacency_matrix = np.empty((size, size), dtype=DTYPE)
        self.__adjacency_matrix.fill(None)
        self.__adjacency_matrix_view = self.__adjacency_matrix

    def __get_vertex_int(self, vertex):
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

    def add_edge(self, v1, v2, double weight=1.0):
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

    def get_children(self, vertex):
        """
        Returns the names of all the child vertices of a given vertex.
        Equivalent to neighbors if all edges are undirected.

        Args:
            vertex: A vertex in the graph.

        Returns:
            A list of the child vertices of the inputted vertex.

        Raises:
            ValueError: The inputted vertex is not in the graph.
        """
        cdef list children = []
        cdef int u, v

        v = self.__get_vertex_int(vertex)

        for u in range(<int>len(self.vertices)):
            if self.__adjacency_matrix_view[v][u] is not None:
                children.append(self.__reverse_vertex_map[u])

        return children

    @property
    def edges(self):
        """
        A list of tuples contianing the two vertices of each edge.
        """
        cdef int u, v, n_vertices
        cdef list edges = []
        cdef set edge

        n_vertices = len(self.vertices)

        if self.directed:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    if self.__adjacency_matrix_view[u][v] is not None:
                        edges.append((u, v))
        else:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    if self.__adjacency_matrix_view[u][v] is not None:
                        edge = {u, v}
                        if edge not in edges:
                            edges.append(edge)
            edges = [tuple(edge) for edge in edges]

        return edges