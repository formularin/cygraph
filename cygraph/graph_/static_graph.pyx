#!python
#cython: language_level=3
"""Implementation of graph data strucutre with numpy array as adjacency
matrix.
"""

import warnings

cimport numpy as np
import numpy as np

from cygraph.graph_.graph cimport Graph


cdef type DTYPE = np.float64
ctypedef np.float64_t DTYPE_t


cdef class StaticGraph(Graph):
    """A class representing a graph data structure.

    This is a directed graph class, although it will function as an
    undirected graph by creating directed edges both ways between two
    vertices. This class contains only basic functionality; algorithms
    are implemented externally.

    Adding vertices to a StaticGraph is relatively slow, expecially for
    already large graphs. If you are going to be adding lots of
    vertices to your graph, consider using cygraph.DynamicGraph.

    Parameters
    graph: cygraph.Graph, optional
        A graph to create a copy of.
    directed: bint, optional
        Whether or not the graph contains directed edges.
    vertices: list, optional
        A list of vertices (can be any hashable type).

    Note that `directed` and `vertices` args will be ignored if
    `graph` is not None.

    Attributes
    ----------
    directed: bint
        Whether or not the graph contains directed edges.
    vertices: list
        The vertices in this graph.
    edges: set
        Tuples contianing the two vertices of each edge.
    edge_attribtues: dict
        Maps (v1, v2) tuples to dicts mapping edge attribute keys to
        corresponding values.
    vertex_attribtues: dict
        Maps vertices to dicts mapping vertex attribute keys to
        corresponding values.
    """

    def __cinit__(self, Graph graph=None, bint directed=False, list vertices=[]):

        cdef int size
        cdef object v

        if graph is not None:

            size = len(graph.vertices)
            self._adjacency_matrix = np.full((size, size), np.nan, dtype=DTYPE)
            self._adjacency_matrix_view = self._adjacency_matrix

            for edge in graph.edges:
                self.add_edge(edge[0], edge[1])

        else:
            self._vertex_attributes = {}
            self._edge_attributes = {}

            self.directed = directed
            self.vertices = list(vertices)

            # Map vertex names to numbers and vice versa.
            for v in self.vertices:
                self._vertex_attributes[v] = {}

            # Create adjacency matrix.
            size = len(self.vertices)
            self._adjacency_matrix = np.full((size, size), np.nan, dtype=DTYPE)
            self._adjacency_matrix_view = self._adjacency_matrix

    cpdef void add_edge(self, object v1, object v2, DTYPE_t weight=1.0) except *:
        """Adds edge to graph between two vertices with a weight.

        Parameters
        ----------
        v1
            One of the edge's vertices.
        v2
            One of the edge's vertices.
        weight: np.float64, optional
            The weight of the edge.
        """
        cdef int u = self._get_vertex_int(v1)
        cdef int v = self._get_vertex_int(v2)

        self._edge_attributes[(v1, v2)] = {}

        self._adjacency_matrix_view[u][v] = weight
        if not self.directed:
            self._adjacency_matrix_view[v][u] = weight

    cpdef void remove_edge(self, object v1, object v2) except *:
        """Removes an edge between two vertices in this graph.

        Parameters
        ----------
        v1
            One of the edge's vertices.
        v2
            One of the edge's vertices.
        """
        cdef int u = self._get_vertex_int(v1)
        cdef int v = self._get_vertex_int(v2)

        if np.isnan(self._adjacency_matrix_view[u][v]):
            warnings.warn("Attempting to remove edge that doesn't exist.")
        else:
            self._adjacency_matrix_view[u][v] = np.nan
            if not self.directed:
                self._adjacency_matrix_view[v][u] = np.nan

    cpdef bint has_edge(self, object v1, object v2) except *:
        """Returns whether or not an edge exists in this graph.

        Parameters
        ----------
        v1
            First vertex of the edge.
        v2
            Second vertex of the edge.

        Returns
        -------
        bint
            Whether or not edge is in graph.
        """
        cdef int u, v
        try:
            u = self.vertices.index(v1)
            v = self.vertices.index(v2)
        except ValueError:
            return False

        return not np.isnan(self._adjacency_matrix_view[u][v])

    cpdef DTYPE_t get_edge_weight(self, object v1, object v2) except *:
        """Returns the weight of the edge between vertices v1 and v2.

        Parameters
        ----------
        v1
            One of the edge's vertices.
        v2
            One of the edge's vertices.

        Returns
        -------
        np.float64
            The weight of the edge between v1 and v2.
        """
        cdef int u = self._get_vertex_int(v1)
        cdef int v = self._get_vertex_int(v2)
        cdef DTYPE_t weight = self._adjacency_matrix_view[u][v]
        if not np.isnan(weight):
            return weight
        else:
            raise ValueError(f"There is no edge ({v1}, {v2}) in graph.")

    cpdef void add_vertex(self, object v) except *:
        """Adds vertex to the graph.

        Parameters
        ----------
        v
            A vertex of any hashable type.
        """
        cdef int vertex_number = len(self.vertices)
        cdef np.ndarray new_row, new_column

        self._vertex_attributes[v] = {}

        if v in self.vertices:
            raise ValueError(f"{v} is already in graph")
        else:
            # Map vertex name to number.
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

    cpdef void remove_vertex(self, object v) except *:
        """Removes a vertex from this graph.

        Parameters
        ----------
        v
            A vertex in this graph.
        """
        cdef int u = self._get_vertex_int(v)

        self.vertices.remove(v)
        np.delete(self._adjacency_matrix, u, axis=1)  # Delete column.
        np.delete(self._adjacency_matrix, u, axis=0)  # Delete row.

    cpdef set get_children(self, object v):
        """Returns the names of all the child vertices of a given
        vertex. Equivalent to neighbors if graph is undirected.

        Parameters
        ----------
        v
            A vertex in the graph.

        Returns
        -------
        set
            The child vertices of `v`.
        """
        cdef set children = set()
        cdef int u, w

        w = self._get_vertex_int(v)

        for u in range(len(self.vertices)):
            if not np.isnan(self._adjacency_matrix_view[w][u]):
                children.add(self.vertices[u])

        return children
    
    cpdef set get_parents(self, object v):
        """Returns the parents (aka "in-neighbors") of a given vertex.
        Equivalent to get_children in undirected graphs. 

        Parameters
        ----------
        v
            A vertex in the graph.
        
        Returns
        -------
        set
            The parent vertices of `v`.
        """
        cdef set parents = set()
        cdef int u, w

        w = self._get_vertex_int(v)

        for u in range(len(self.vertices)):
            if not np.isnan(self._adjacency_matrix_view[u][w]):
                parents.add(self.vertices[u])

        return parents

    @property
    def edges(self):
        cdef int u, v, n_vertices
        cdef set edges = set()
        cdef tuple new_edge, existing_edge
        cdef bint edge_found
        cdef DTYPE_t edge_weight

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

    def __copy__(self):
        cdef StaticGraph new_graph = \
            StaticGraph(directed=self.directed, vertices=self.vertices)

        # Add edges and edge attributes.
        cdef tuple edge
        cdef object key
        for edge in self.edges:
            new_graph.add_edge(*edge)
            for key in self._edge_attributes[edge]:
                new_graph.set_edge_attribute(
                    edge, key, self._edge_attributes[edge][key])

        # Add vertex attributes.
        cdef object vertex
        for vertex in self.vertices:
            for key in self._vertex_attributes[vertex]:
                new_graph.set_vertex_attribtue(
                    vertex, key, self._vertex_attributes[vertex][key])

        return new_graph