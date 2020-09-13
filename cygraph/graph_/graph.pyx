#!python
#cython: language_level=3
"""Implementation of StaticGraph and DynamicGraph classes.
"""

import copy

import numpy as np


NOT_IMPLEMENTED = ("%s is not implemented for "
    "cygraph.Graph instance. Try it with cygraph.StaticGraph or "
    "cygraph.DynamicGraph.")


cdef class Graph:
    """A semi-abstract base graph class.

    Semi-abstract because anything that involves the adjacency matrix
    is not implemented, and therefore this class is not meant to be
    used for instantiating objects. The reason why those methods are
    even in this class is so that downcasting still works because of
    early binding.
    """

    def __cinit__(self, *args, **kwargs):

        cdef Graph graph = None

        if len(args) == 3:
            graph = <Graph?>args[3]

        if "graph" in kwargs:
            graph = kwargs["graph"]

        if len(args) == 1:
            if isinstance(args[0], Graph):
                graph = args[0]

        if graph is not None:
            self._vertex_attributes = copy.deepcopy(graph._vertex_attributes)
            self._edge_attributes = copy.deepcopy(graph._edge_attributes)

            self.directed = bool(graph.directed)
            self.vertices = graph.vertices[:]

    def __iter__(self):
        return iter(self.vertices)

    def __len__(self):
        return len(self.vertices)

    def __repr__(self):
        return f"<{self.__class__.__name__}; vertices={self.vertices!r}; edges={self.edges!r}>"

    def __str__(self):
        return str(np.array(self._adjacency_matrix))

    def __eq__(self, other):
        raise NotImplementedError("Comparing graphs is ambiguous, use the "
                                  ".equals() method to specify whether or not "
                                  "to consider edge and vertex attributes.")

    @property
    def edge_attributes(self):
        return self._edge_attributes

    @property
    def vertex_attributes(self):
        return self._vertex_attributes

    @property
    def adjacency_list(self):
        cdef object u, v
        cdef int i, j
        cdef list adj_list = [[] for _ in self.vertices]
        for i, u in enumerate(self.vertices):
            for j, v in enumerate(self.vertices):
                if self.has_edge(u, v):
                    adj_list[i].append(j)
        return adj_list

    @property
    def adjacency_matrix(self):
        return self._adjacency_matrix

    cdef int _get_vertex_int(self, object vertex) except -1:
        """Returns the int corresponding to a vertex.

        Parameters
        ----------
        vertex
            A vertex in the graph.

        Returns
        -------
        int
            The integer corresponding to `vertex`.
        """
        try:
            return self.vertices.index(vertex)
        except ValueError:
            raise ValueError(f"{vertex} is not in graph.")

    cpdef void add_vertex(self, object v) except *:
        raise NotImplementedError(NOT_IMPLEMENTED % "add_vertex")

    cpdef void remove_vertex(self, object v) except *:
        raise NotImplementedError(NOT_IMPLEMENTED % "remove_vertex")

    cpdef void set_vertex_attribute(self, object vertex, object key, object val) except *:
        """Sets an attribute to a vertex.

        Parameters
        ----------
        vertex
            A vertex in the graph.
        key
            The name of the attribute. Must be of hashable type.
        val
            The value of the attribute.
        """
        try:
            self._vertex_attributes[vertex][key] = val
        except KeyError:
            raise ValueError(f"{vertex} is not in graph.")

    cpdef object get_vertex_attribute(self, object vertex, object key):
        """Gets an attribute of a vertex.

        Parameters
        ----------
        vertex
            A vertex in the graph.
        key
            The name of the attribute. Must be of hashable type.

        Returns
        -------
        The value of the attribute.
        """
        cdef dict vertex_attributes
        try:
            vertex_attributes = self._vertex_attributes[vertex]
        except KeyError:
            raise ValueError(f"{vertex} is not in graph.")
        return vertex_attributes[key]

    cpdef bint has_vertex(self, object vertex) except *:
        """Returns whether or not a vertex is in this graph.

        Parameters
        ----------
        vertex
            A valid vertex (hashable type).

        Returns
        -------
        bint
            Whether or not `vertex` is in this graph.
        """
        return vertex in self.vertices

    cpdef void add_edge(self, object v1, object v2, double weight=1.0) except *:
        raise NotImplementedError(NOT_IMPLEMENTED % "add_edge")

    cpdef void remove_edge(self, object v1, object v2) except *:
        raise NotImplementedError(NOT_IMPLEMENTED % "remove_edge")

    cpdef void set_edge_attribute(self, tuple edge, object key, object val) except *:
        """Sets an attribute to an edge.

        Parameters
        ----------
        edge: tuple
            An edge in the graph in the form (v1, v2).
        key
            The name of the attribute. Must be of hashable type.
        val
            The value of the attribute.
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

    cpdef object get_edge_attribute(self, tuple edge, object key):
        """Gets an attribute of an edge.

        Parameters
        ----------
        edge: tuple
            An edge in the graph in the form (v1, v2).
        key
            The name of the attribute. Must be of hashable type.

        Returns
        -------
        The value of the attribute.
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

    cpdef double get_edge_weight(self, object v1, object v2) except *:
        raise NotImplementedError(NOT_IMPLEMENTED % "get_edge_weight")

    cpdef bint has_edge(self, object v1, object v2) except *:
        raise NotImplementedError(NOT_IMPLEMENTED % "has_edge")

    cpdef set get_children(self, object vertex):
        raise NotImplementedError(NOT_IMPLEMENTED % "get_children")
    
    cpdef set get_parents(self, object vertex):
        raise NotImplementedError(NOT_IMPLEMENTED % "get_parents")

    cpdef bint equals(self, Graph other, bint vertex_attrs=False,
            bint edge_attrs=False) except *:
        """Determines whether two graphs are equal.

        Parameters
        ----------
        other: cygraph.Graph
            The graph to make the comparison with.
        vertex_attrs: bint, optional
            Whether or not to consider vertex attributes when detemining
            equality.
        edge_attrs: bint, optional
            Whether or not to consider edge attributes when determining
            equality.

        Returns
        -------
        bint
            Whether or not the two graphs are equal.
        """
        cdef bool vertices = (self.vertices == other.vertices)
        cdef object weight, u, v

        if self.directed != other.directed:
            return False

        if vertices:

            # Check that adjacency matrices are the same.
            for u in self.vertices:
                for v in self.vertices:
                    try:
                        weight = self.get_edge_weight(u, v)
                    except ValueError:
                        weight = None
                    try:
                        if other.get_edge_weight(u, v) != weight:
                            return False
                    except ValueError:
                        if weight is not None:
                            return False

            # Check that edge and vertex attributes are the same.
            if vertex_attrs:
                return self.vertex_attributes == other.vertex_attributes
            if edge_attrs:
                return self.edge_attributes == other.vertex_attributes

            return True
        else:
            return False