"""Implementation of the Graph class
"""


import numpy as np


cdef class Graph:
    
    cdef readonly int val;

    def __cinit__(self, int v):
        self.val = v