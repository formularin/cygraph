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
    ----------
    graph: cygraph.Graph, optional
        A graph to create a copy of. If this is not None, all other
        parameters are ignored. Edge and vertex attributes are
        deepcopied.
    directed: bint, optional
        Whether or not the graph contains directed edges.
    vertices: list, optional
        A list of vertices (can be any hashable type).
    adjacency_matrix: list, optional
        An adjacency matrix to generate the graph from.
    adjacency_list: list of lists, optional
        An adjacency list to generate the graph from. Assumes all edge
        weights are 1.0.

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
    adjacency_matrix: np.ndarray
        The adjacency matrix that represents this graph.
    adjacency_list: list of lists
        The adjacency list that represents this graph.
    """

    def __cinit__(self, Graph graph=None, bint directed=False, list vertices=[],
            np.ndarray adjacency_matrix=None, list adjacency_list=[]):

        cdef int size, n_vertices, n_rows, n_adj_list_vertices, vertex
        cdef object v

        if graph is not None:

            size = len(graph.vertices)
            self._adjacency_matrix = np.full((size, size), np.nan, dtype=DTYPE)
            self._adjacency_matrix_view = self._adjacency_matrix

            for edge in graph.edges:
                self.add_edge(*edge)

        else:
            self._vertex_attributes = {}
            self._edge_attributes = {}

            self.directed = directed
            self.vertices = list(vertices)

            # Initialize attribute dictionary for each vertex.
            for v in self.vertices:
                self._vertex_attributes[v] = {}

            if adjacency_matrix is not None:
                # Check that there is the right number of vertices.
                n_vertices = len(vertices)
                n_rows = len(adjacency_matrix)
                if n_vertices != n_rows:
                    raise ValueError("Different number of vertices as rows in"
                                    f" adjacency matrix. {n_vertices} vertices"
                                    f" and {n_rows} rows.")
                # Check that there is no specified adjacency list.
                if adjacency_list != []:
                    raise ValueError("both adjacency list and adjacency matrix "
                                     "specified.")

                self._adjacency_matrix = adjacency_matrix
                self._adjacency_matrix_view = self._adjacency_matrix
            else:

                # Create empty adjacency matrix.
                size = len(self.vertices)
                self._adjacency_matrix = np.full((size, size), np.nan, dtype=DTYPE)
                self._adjacency_matrix_view = self._adjacency_matrix

                if adjacency_list:
                    # Check that there is the right number of vertices.
                    n_vertices = len(vertices)
                    n_adj_list_vertices = len(adjacency_list)
                    if n_vertices != n_adj_list_vertices:
                        raise ValueError("Different number of vertices in "
                                        "`vertices` than in `adjacency_list`."
                                        f"{n_vertices} in `vertices` and "
                                        f"{n_adj_list_vertices} in "
                                        "`adjacency_list`")
                    # Fill in the adjacency matrix.
                    for vertex in range(n_vertices):
                        for vertex_ in range(n_vertices):
                            if vertex_ in adjacency_list[vertex]:
                                self._adjacency_matrix[vertex][vertex_] = 1.0

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

    def __reduce__(self):
        return (rebuild_static_graph, (self._vertex_attributes,
                self._edge_attributes, self.vertices, self.directed,
                self._adjacency_matrix))

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

        if not np.isnan(self._adjacency_matrix_view[u][v]):
            raise ValueError(f"Edge ({v1}, {v2}) already exists.")

        self._edge_attributes[(v1, v2)] = {}

        self._adjacency_matrix_view[u][v] = weight
        if not self.directed:
            self._adjacency_matrix_view[v][u] = weight

    cpdef void set_edge_weight(self, object v1, object v2, DTYPE_t weight
            ) except *:
        """Changes the weight of an edge.

        Parameters
        ----------
        v1
            One of the edge's vertices.
        v2
            One of the edge's vertices
        weight: np.float64
            The new weight of the edge.
        """
        cdef int u = self._get_vertex_int(v1)
        cdef int v = self._get_vertex_int(v2)

        if np.isnan(self._adjacency_matrix_view[u][v]):
            raise ValueError(f"Edge ({v1}, {v2}) does not exist.")

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
        """Returns whether or not an edge exists in this graph. If one
        of the vertices is not in the graph, still returns False.

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
                self._adjacency_matrix = np.array([[np.nan]], dtype=DTYPE)
                self._adjacency_matrix_view = self._adjacency_matrix
            else:
                # Add new row.
                new_row = np.full((1, vertex_number), np.nan, dtype=DTYPE)
                self._adjacency_matrix = \
                    np.append(self._adjacency_matrix, new_row, axis=0)

                # Add new column.
                new_column = np.full((vertex_number + 1, 1), np.nan, dtype=DTYPE)
                self._adjacency_matrix = \
                    np.append(self._adjacency_matrix, new_column, axis=1)

    cpdef void add_vertices(self, set vertices) except *:
        """Adds a set of vertices to the graph.

        Parameters
        ----------
        vertices: set
            A set of vertices, which can be of any hashable type.
        """
        cdef object v
        cdef np.ndarray new_rows, new_columns
        cdef int starting_n_vertices, new_n_vertices, n_new_vertices

        starting_n_vertices = len(self.vertices)
        n_new_vertices = len(vertices)
        new_n_vertices = starting_n_vertices + n_new_vertices

        for v in vertices:
            if v in self.vertices:
                raise ValueError(f"{v} is already in graph.")

        for v in vertices:
            self._vertex_attributes[v] = {}
            self.vertices.append(v)

        if starting_n_vertices == 0:
            self._adjacency_matrix = np.full((n_new_vertices, n_new_vertices),
                np.nan, dtype=DTYPE)
            # Add new rows.
            new_rows = np.full((n_new_vertices, starting_n_vertices), np.nan,
                dtype=DTYPE)
            self._adjacency_matrix = np.append(self._adjacency_matrix, new_rows,
                axis=0)

            # Add new columns.
            new_columns = np.full((new_n_vertices, n_new_vertices), np.nan,
                dtype=DTYPE)
            self._adjacency_matrix = np.append(self._adjacency_matrix, new_columns,
                axis=1)

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


def rebuild_static_graph(vertex_attributes, edge_attributes, vertices, directed,
        adjacency_matrix):
    """Rebuilds a StaticGraph instance from unpickled values.

    Parameters
    ----------
    Each of the parameters corresponds to an attribute in the
    StaticGraph class.

    Returns
    -------
    The rebuilt StaticGraph instance.
    """
    static_graph = StaticGraph(directed=directed, vertices=vertices,
        adjacency_matrix=adjacency_matrix)
    for vertex in vertex_attributes:
        for key, val in vertex_attributes[vertex].items():
            static_graph.set_vertex_attribute(vertex, key, val)
    for edge in edge_attributes:
        for key, val in edge_attributes[edge].items():
            static_graph.set_edge_attribute(edge, key, val)
    return static_graph
