#!python
#cython: language_level=3
from cygraph.graph_ cimport Graph, DynamicGraph, StaticGraph


__version__ = '0.1.3'


cpdef Graph graph(bint static=*, Graph graph_=*, bint directed=*,
    list vertices=*)