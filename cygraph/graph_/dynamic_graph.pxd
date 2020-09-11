#!python
#cython: language_level=3

from cygraph.graph_.graph cimport Graph


cdef class DynamicGraph(Graph):
    # _adjacency_matrix[u][v] -> weight of edge between u and v.
    # None means there is no edge.
    cdef readonly list _adjacency_matrix