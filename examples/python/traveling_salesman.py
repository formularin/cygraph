"""Implementation of the ant colony optimization (ACO) algorithm for the
traveling salesman problem (TSP).
"""

from itertools import combinations
from os.path import abspath, dirname, join

import cygraph as cg


def _get_squared_distance(x1, y1, x2, y2):
    return (x1 - x2) ** 2 + (y1 - y2) ** 2


examples_dir = dirname(dirname(__file__))
data_dir = join(abspath(examples_dir), 'data')
DATA_FILE = join(data_dir, 'ar9152.tsp')

with open(DATA_FILE, 'r') as f:
    lines = f.read().strip().split('\n')

data_lines = [[float(i) for i in line.split()] for line in lines[7:-1]]
city_graph = cg.graph(vertices=list(range(len(data_lines))))

# Add an edge for every pair of cities with the weight being the squared
# distance between them.
for u, v in combinations(city_graph.vertices, 2):
    distance = _get_squared_distance(data_lines[u][1], data_lines[u][2],
                                     data_lines[v][1], data_lines[v][2])
    city_graph.add_edge(u, v, weight=distance)