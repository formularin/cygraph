#!python
#cython: language_level=3

from cygraph.graph_ cimport DynamicGraph, Graph, StaticGraph


cdef set get_articulation_points(Graph graph)