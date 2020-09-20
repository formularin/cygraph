#!python
#cython: language_level=3

from cygraph.graph_ cimport DynamicGraph, Graph, StaticGraph


cdef list get_connected_components(Graph graph, bint static)
cdef int get_number_connected_components(Graph graph) except *
cdef list get_strongly_connected_components(Graph graph, bint static)
cdef int get_number_strongly_connected_components(Graph graph) except *