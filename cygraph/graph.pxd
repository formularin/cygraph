"""
Graph base class header file for cimport by algorithms.pyx
"""


cdef class Graph:
    cpdef int _get_vertex_int(self, vertex) except -1