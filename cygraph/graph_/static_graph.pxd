#!python
#cython: language_level=3

cimport numpy as np

from cygraph.graph_.graph cimport Graph


cdef class StaticGraph(Graph):
    # _adjacency_matrix_view[u][v] -> weight of edge between u and v.
    # np.nan means there is no edge.
    cdef double[:,:] _adjacency_matrix_view
    cdef readonly np.ndarray _adjacency_matrix