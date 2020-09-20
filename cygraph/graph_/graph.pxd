#!python
#cython: language_level=3

cdef class Graph:
    cdef dict _vertex_attributes
    cdef dict _edge_attributes

    cdef readonly list vertices
    cdef readonly bint directed

    cdef int _get_vertex_int(self, object vertex) except -1

    cpdef void add_vertex(self, object v) except *
    cpdef void add_vertices(self, set vertices) except *
    cpdef void remove_vertex(self, object v) except *
    cpdef void set_vertex_attribute(self, object vertex, object key, object val) except *
    cpdef void remove_vertex_attribute(self, object vertex, object key) except *
    cpdef void set_vertex_attributes(self, object vertex, dict attributes) except *
    cpdef object get_vertex_attribute(self, vertex, key)
    cpdef bint has_vertex(self, object vertex) except *
    cpdef void add_edge(self, object v1, object v2, double weight=*) except *
    cpdef void set_edge_weight(self, object v1, object v2, double weight) except *
    cpdef void add_edges(self, set edges) except *
    cpdef void remove_edge(self, object v1, object v2) except *
    cpdef void set_edge_attribute(self, tuple edge, object key, object val) except *
    cpdef void remove_edge_attribute(self, tuple edge, object key) except *
    cpdef void set_edge_attributes(self, tuple edge, dict attributes) except *
    cpdef object get_edge_attribute(self, tuple edge, object key)
    cpdef double get_edge_weight(self, object v1, object v2) except *
    cpdef bint has_edge(self, object v1, object v2) except *
    cpdef set get_children(self, object v)
    cpdef set get_parents(self, object v)
    cpdef bint equals(self, Graph other, bint vertex_attrs=*, bint edge_attrs=*) except *
