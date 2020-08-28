#!python
#cython: language_level=3

from cygraph.graph_ cimport DynamicGraph, Graph, StaticGraph


cdef tuple partition_karger(Graph graph, bint static)