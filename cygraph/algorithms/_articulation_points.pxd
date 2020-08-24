from ..graph cimport DynamicGraph, Graph, StaticGraph


cdef set get_articulation_points(Graph graph)