"""
Graph base class header file for cimport by algorithms.pyx
"""


cdef class Graph:
    cdef dict _vertex_attributes
    cdef dict _edge_attributes
    cdef dict _vertex_map
    
    cdef readonly list vertices
    cdef readonly bint directed

    cpdef int _get_vertex_int(self, vertex) except -1

    cpdef void add_vertex(self, v) except *
    cpdef void set_vertex_attribute(self, vertex, key, val) except *
    cpdef object get_vertex_attribute(self, vertex, key)
    cpdef void add_edge(self, v1, v2, double weight=*) except *
    cpdef void set_edge_attribute(self, edge, key, val) except *
    cpdef object get_edge_attribute(self, edge, key)
    cpdef double get_edge_weight(self, v1, v2) except *
    cpdef set get_children(self, vertex)

    @property
    def edges(self)

    def __len__(self)
    def __repr__(self)
    def __str__(self)


cdef class StaticGraph(Graph):
    # _adjacency_matrix_view[u][v] -> weight of edge between u and v.
    # np.nan means there is no edge.
    cdef double[:,:] _adjacency_matrix_view

    cdef readonly object _adjacency_matrix

cdef class DynamicGraph(Graph):
    # _adjacency_matrix[u][v] -> weight of edge between u and v.
    # None means there is no edge.
    cdef readonly list _adjacency_matrix