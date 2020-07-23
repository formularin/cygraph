"""
Graph base class header file for cimport by algorithms.pyx
"""


cdef class Graph:
    cpdef int _get_vertex_int(self, vertex) except -1
    cpdef void add_vertex(self, v) except *
    cpdef void add_edge(self, v1, v2, double weight=*) except *
    cpdef set get_children(self, vertex)