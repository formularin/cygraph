"""Implementation of StaticGraph and DynamicGraph classes.
"""

import numpy as np


DTYPE = np.float64


cdef class Graph:
    """
    A semi-abstract base graph class.
    """

    cpdef int _get_vertex_int(self, vertex) except -1:
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
            return <int>(self._vertex_map[vertex])
        except KeyError:
            raise ValueError(f"{vertex} is not in graph.")
    
    cpdef void add_vertex(self, v) except *:
        pass

    cpdef void set_vertex_attribute(self, vertex, key, val) except *:
        """
        Sets an attribute to a vertex.

        Args:
            vertex: A vertex in the graph.
            key: The name of the attribute. Must be of hashable type.
            val: The value of the attribute.

        Raises:
            TypeError: key is not of hashable type.
            ValueError: vertex is not in graph.
        """
        try:
            self._vertex_attributes[vertex][key] = val
        except KeyError:
            raise ValueError(f"{vertex} is not in graph.")
    
    cpdef object get_vertex_attribute(self, vertex, key):
        """
        Gets an attribute of a vertex.

        Args:
            vertex: A vertex in the graph.
            key: The name of the attribute. Must be of hashable type.
        
        Returns:
            The value of the attribute.
        
        Raises:
            TypeError: key is not of hashable type.
            ValueError: vertex is not in graph.
            KeyError: key is not in vertex's attributes dict.
        """
        cdef dict vertex_attributes
        try:
            vertex_attributes = self._vertex_attributes[vertex]
        except KeyError:
            raise ValueError(f"{vertex} is not in graph.")
        return vertex_attributes[key]

    cpdef void add_edge(self, v1, v2, double weight=1.0) except *:
        pass
    
    cpdef void set_edge_attribute(self, edge, key, val) except *:
        """
        Sets an attribute to an edge.

        Args:
            edge: An edge in the graph.
            key: The name of the attribute. Must be of hashable type.
            val: The value of the attribute.

        Raises:
            TypeError: key is not of hashable type.
            ValueError: edge is not in graph.
        """
        try:
            self._edge_attributes[edge][key] = val
        except KeyError:
            if self.directed:
                raise ValueError(f"{edge} is not in graph.")
            else:
                try:
                    self._edge_attributes[(edge[1], edge[0])][key] = val
                except KeyError:
                    raise ValueError(f"{edge} is not in graph.")

    cpdef object get_edge_attribute(self, edge, key):
        """
        Gets an attribute of an edge.

        Args:
            edge: An edge in the graph.
            key: The name of the attribute. Must be of hashable type.
        
        Returns:
            The value of the attribute.
        
        Raises:
            TypeError: key is not of hashable type.
            ValueError: edge is not in graph.
            KeyError: key is not in edge's attributes dict.
        """
        cdef dict edge_attributes
        try:
            edge_attributes = self._edge_attributes[edge]
        except KeyError:
            if self.directed:
                raise ValueError(f"{edge} is not in graph.")
            else:
                try:
                    edge_attributes = self._edge_attributes[(edge[1], edge[0])]
                except KeyError:
                    raise ValueError(f"{edge} is not in graph.")
        return edge_attributes[key]

    cpdef set get_children(self, vertex):
        pass
    
    def __repr__(self):
        return f"<{self.__class__.__name__}; vertices: {self.vertices}; edges: {self.edges}>"

    def __str__(self):
        return str(np.array(self._adjacency_matrix))


cdef class StaticGraph(Graph):
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

    # _adjacency_matrix_view[u][v] -> weight of edge between u and v.
    # np.nan means there is no edge.
    cdef double[:,:] _adjacency_matrix_view

    cdef readonly object _adjacency_matrix
    cdef readonly dict _vertex_map  # Maps vertex names to numbers.
    cdef readonly list vertices

    def __cinit__(self, bint directed=False, list vertices=[]):

        self.directed = directed

        # Map vertex names to numbers and vice versa.
        self._vertex_map = {}
        cdef int i
        cdef object v
        for i, v in enumerate(<list?>vertices):
            self._vertex_map[v] = i
            self._vertex_attributes[v] = {}
        
        self.vertices = vertices[:]

        # Create adjacency matrix.
        cdef int size = len(vertices)
        self._adjacency_matrix = np.full((size, size), np.nan, dtype=DTYPE)
        self._adjacency_matrix_view = self._adjacency_matrix

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
        cdef int u = self._get_vertex_int(v1)
        cdef int v = self._get_vertex_int(v2)

        self._edge_attributes[(v1, v2)] = {}

        self._adjacency_matrix_view[u][v] = weight
        if not self.directed:
            self._adjacency_matrix_view[v][u] = weight

    cpdef void add_vertex(self, v) except *:
        """
        Adds vertex to the graph.

        Args:
            v: A vertex of any hashable type.
        
        Raises:
            TypeError: vertex type is not hashable.
            ValueError: vertex is already in graph.
        """
        cdef int vertex_number = len(<dict>self._vertex_map)
        cdef object new_row, new_column

        self._vertex_attributes[v] = {}

        if v in self.vertices:
            raise ValueError(f"{v} is already in graph")
        else:
            # Map vertex name to number.
            self._vertex_map[v] = vertex_number
            self.vertices.append(v)

            if vertex_number == 0:
                self._adjacency_matrix = np.full((1, 1), np.nan, dtype=DTYPE)
            else:
                # Add new row.
                new_row = np.full((1, vertex_number), np.nan, dtype=DTYPE)
                self._adjacency_matrix = \
                    np.append(self._adjacency_matrix, new_row, axis=0)

                # Add new column.
                new_column = np.full((vertex_number + 1, 1), np.nan, dtype=DTYPE)
                self._adjacency_matrix = \
                    np.append(self._adjacency_matrix, new_column, axis=1)
    
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

        v = self._get_vertex_int(vertex)

        for u in range(<int>len(self.vertices)):
            if not np.isnan(self._adjacency_matrix_view[v][u]):
                children.add(self.vertices[u])

        return children

    @property
    def edges(self):
        """
        A list of tuples contianing the two vertices of each edge.
        """
        cdef int u, v, n_vertices
        cdef set edges = set()
        cdef tuple new_edge, existing_edge
        cdef bint edge_found
        cdef object edge_weight

        n_vertices = len(self.vertices)

        if self.directed:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    edge_weight = self._adjacency_matrix_view[u][v]
                    if not np.isnan(edge_weight):
                        edges.add(
                            (self.vertices[u],
                             self.vertices[v],
                             edge_weight
                            )
                        )
        else:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    edge_weight = self._adjacency_matrix_view[u][v]
                    if not np.isnan(edge_weight):
                        # Edge exists. Add it if it hasn't already been found.
                        new_edge = (
                            self.vertices[u],
                            self.vertices[v],
                            edge_weight
                        )
                        edge_found = False
                        for existing_edge in edges:
                            if (    existing_edge[0] == new_edge[1]
                                    and existing_edge[1] == new_edge[0]):
                                edge_found = True
                                break
                        if edge_found:
                            continue
                        else:
                            edges.add(new_edge)

        return edges


cdef class DynamicGraph(Graph):
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

    # _adjacency_matrix[u][v] -> weight of edge between u and v.
    # None means there is no edge.
    cdef readonly list _adjacency_matrix
    cdef readonly dict _vertex_map
    cdef readonly list vertices

    def __cinit__(self, bint directed=False, list vertices=[]):

        self.directed = directed

        # Map vertex names to numbers and vice versa.
        self._vertex_map = {}
        cdef int i
        cdef object v
        for i, v in enumerate(<list?>vertices):
            self._vertex_map[v] = i
            self._vertex_attributes[v] = {}

        self.vertices = vertices[:]

        # Create adjacency matrix.
        cdef int size = len(vertices)
        self._adjacency_matrix = []
        for i in range(size):
            self._adjacency_matrix.append([])
            for _ in range(size):
                self._adjacency_matrix[i].append(None)
    
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
        cdef int u = self._get_vertex_int(v1)
        cdef int v = self._get_vertex_int(v2)

        self._vertex_attributes[(v1, v2)] = {}

        self._adjacency_matrix[u][v] = weight
        if not self.directed:
            self._adjacency_matrix[v][u] = weight
    
    cpdef void add_vertex(self, v) except *:
        """
        Adds vertex to the graph.

        Args:
            v: A vertex of any hashable type.
        
        Raises:
            TypeError: vertex type is not hashable.
        """
        cdef int vertex_number, i
        cdef list new_row

        self._vertex_attributes[v] = {}

        if v in self.vertices:
            raise ValueError(f"{v} is already in graph")
        else:
            # Map vertex name to number.
            vertex_number = len(<dict>self._vertex_map)
            self._vertex_map[v] = vertex_number
            self.vertices.append(v)

            # Add new row.
            new_row = [None for _ in range(vertex_number + 1)]
            self._adjacency_matrix.append(new_row)
            
            # Add new column.
            for i in range(<int> (len(self._adjacency_matrix) - 1)):
                self._adjacency_matrix[i].append(None)
    
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

        v = self._get_vertex_int(vertex)

        for u in range(<int>len(self.vertices)):
            if self._adjacency_matrix[v][u] is not None:
                children.add(self.vertices[u])

        return children

    @property
    def edges(self):
        """
        A set of tuples contianing the two vertices of each edge.
        """
        cdef int u, v, n_vertices
        cdef set edges = set()
        cdef tuple new_edge, existing_edge
        cdef bint edge_found
        cdef object edge_weight

        n_vertices = len(self.vertices)

        if self.directed:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    edge_weight = self._adjacency_matrix[u][v]
                    if edge_weight is not None:
                        edges.add(
                            (self.vertices[u],
                             self.vertices[v],
                             edge_weight)
                        )
        else:
            for u in range(n_vertices):
                for v in range(n_vertices):
                    edge_weight = self._adjacency_matrix[u][v]
                    if edge_weight is not None:
                        # Edge exists. Add it if it hasn't already been found.
                        new_edge = (
                            self.vertices[u],
                            self.vertices[v],
                            edge_weight
                        )
                        edge_found = False
                        for existing_edge in edges:
                            if (    existing_edge[0] == new_edge[1]
                                    and existing_edge[1] == new_edge[0]):
                                edge_found = True
                                break
                        if edge_found:
                            continue
                        else:
                            edges.add(new_edge)
        return edges