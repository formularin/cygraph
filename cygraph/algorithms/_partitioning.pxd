from ..graph cimport DynamicGraph, Graph, StaticGraph


cdef set partition_karger(Graph graph, bint static)