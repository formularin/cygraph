"""
Graph base class header file for cimport by algorithms.pyx
"""

cimport numpy as np


cdef class Graph:
    cdef dict _vertex_attributes
    cdef dict _edge_attributes

    cdef readonly list vertices
    cdef readonly bint directed

    cdef int _get_vertex_int(self, object vertex) except -1

    cpdef void add_vertex(self, object v) except *
    cpdef void remove_vertex(self, object v) except *
    cpdef void set_vertex_attribute(self, object vertex, object key, object val) except *
    cpdef object get_vertex_attribute(self, vertex, key)
    cpdef bint has_vertex(self, object vertex) except *
    cpdef void add_edge(self, object v1, object v2, double weight=*) except *
    cpdef void remove_edge(self, object v1, object v2) except *
    cpdef void set_edge_attribute(self, tuple edge, object key, object val) except *
    cpdef object get_edge_attribute(self, tuple edge, object key)
    cpdef double get_edge_weight(self, object v1, object v2) except *
    cpdef set get_children(self, object v)
    cpdef set get_parents(self, object v)


cdef class StaticGraph(Graph):
    # _adjacency_matrix_view[u][v] -> weight of edge between u and v.
    # np.nan means there is no edge.
    cdef double[:,:] _adjacency_matrix_view

    cdef readonly np.ndarray _adjacency_matrix

    cpdef bint has_edge(self, object v1, object v2) except *


cdef class DynamicGraph(Graph):
    # _adjacency_matrix[u][v] -> weight of edge between u and v.
    # None means there is no edge.
    cdef readonly list _adjacency_matrix

    cpdef bint has_edge(self, object v1, object v2) except *