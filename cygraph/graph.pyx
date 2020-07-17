"""Implementation of the Graph class
"""

import numpy as np


DTYPE = np.float64


cdef class StaticGraph:
    """
    A class representing a graph with a set number of vertices.

    Args:
        list vertices: A list of vertices (can be any hashable type)
    """

    cdef object __adjacency_matrix
    cdef double[:,:] __adjacency_matrix_view
    cdef dict __vertex_map  # Maps vertex names to numbers.
    cdef list __reverse_vertex_map  # Maps numbers to vertex names.

    cdef readonly bint directed
    cdef readonly list vertices

    def __cinit__(self, list vertices, directed=False):
        
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
        self.__adjacency_matrix = np.zeros((size, size), dtype=DTYPE)
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
            if self.__adjacency_matrix_view[v][u] != <double>0.0:
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
                    if self.__adjacency_matrix_view[u][v] != 0.0:
                        edges.append((u, v))
        else:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    if self.__adjacency_matrix_view[u][v] != 0.0:
                        edge = {u, v}
                        if edge not in edges:
                            edges.append(edge)
            edges = [tuple(edge) for edge in edges]
        
        return edges