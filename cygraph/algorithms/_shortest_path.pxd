#!python
#cython: language_level=3
from ..graph_ cimport DynamicGraph, Graph, StaticGraph


cdef list find_shortest_path_dijkstra(Graph graph, object source, object target)