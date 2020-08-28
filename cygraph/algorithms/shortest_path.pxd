#!python
#cython: language_level=3
from cygraph.graph_ cimport DynamicGraph, Graph, StaticGraph


cdef list get_shortest_path_dijkstra(Graph graph, object source, object target)