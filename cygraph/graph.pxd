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
    cpdef set get_children(self, vertex)