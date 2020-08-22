"""
Unit tests for algorithms implemented in cygraph/algorithms.pyx
"""

import itertools
import string

import pytest

from .. import create_graph, StaticGraph, DynamicGraph
from ..algorithms import (
    find_articulation_points,
    find_shortest_path,
    get_min_spanning_tree,
    partition_graph,
    get_components,
    get_strongly_connected_components
)


def test_find_articulation_points():
    """
    Tests find_articulation_points function.
    """
    random_graph = {0: {74, 11, 76}, 1: {33, 59, 77}, 2: {53, 78}, 3: {64}, 4: {74}, 5: {80, 81, 89}, 6: {29, 70, 47}, 7: {8}, 8: {58, 36, 7, 95}, 9: {26, 23}, 10: set(), 11: {0, 70}, 12: {73}, 13: {99, 62, 39}, 14: set(), 15: {26, 50, 76}, 16: {93, 22}, 17: {84, 86}, 18: {50, 77}, 19: set(), 20: {56, 58, 66, 39}, 21: {97, 67, 71, 50, 28}, 22: {16, 77}, 23: {9}, 24: {49, 33}, 25: set(), 26: {9, 15}, 27: {41, 82, 33}, 28: {80, 21, 70}, 29: {43, 6}, 30: {73, 46}, 31: {80, 50, 82}, 32: {46}, 33: {1, 73, 80, 24, 57, 27}, 34: {48, 84, 70, 71}, 35: {65, 63}, 36: {8, 46, 47}, 37: set(), 38: {98, 85}, 39: {69, 20, 13, 54}, 40: {66}, 41: {56, 27}, 42: set(), 43: {88, 29}, 44: {68}, 45: {48}, 46: {32, 36, 30}, 47: {36, 6}, 48: {81, 34, 45}, 49: {24, 81, 54}, 50: {18, 15, 21, 31}, 51: set(), 52: set(), 53: {2}, 54: {65, 49, 39}, 55: {89}, 56: {65, 41, 73, 20, 59}, 57: {33}, 58: {8, 97, 20}, 59: {56, 1}, 60: set(), 61: set(), 62: {13}, 63: {35}, 64: {3}, 65: {56, 81, 35, 54}, 66: {40, 90, 20}, 67: {21}, 68: {44}, 69: {97, 93, 39}, 70: {34, 6, 11, 86, 28}, 71: {73, 34, 21}, 72: {98, 94}, 73: {33, 71, 12, 56, 30}, 74: {0, 4}, 75: set(), 76: {0, 15}, 77: {1, 18, 22}, 78: {2}, 79: set(), 80: {33, 5, 87, 28, 31}, 81: {65, 49, 48, 5}, 82: {27, 31}, 83: {87}, 84: {17, 34}, 85: {90, 38}, 86: {17, 70}, 87: {80, 83}, 88: {43}, 89: {5, 55}, 90: {66, 85}, 91: set(), 92: set(), 93: {16, 69}, 94: {72}, 95: {8}, 96: set(), 97: {58, 69, 21}, 98: {72, 38}, 99: {13}}
    sample_graphs = {
        'static': {
            'undirected': create_graph(static=True, directed=False, vertices=list(random_graph)),
            'directed': create_graph(static=True, directed=True, vertices=list(random_graph))
        },
        'dynamic': {
            'undirected': create_graph(static=False, directed=False, vertices=list(random_graph)),
            'directed': create_graph(static=False, directed=True, vertices=list(random_graph))
        }
    }
    for graph in [g for s in sample_graphs for g in sample_graphs[s].values()]:
        for vertex in random_graph:
            for neighbor in random_graph[vertex]:
                if not graph.has_edge(vertex, neighbor):
                    graph.add_edge(vertex, neighbor)

    random_graph_articulation_points = {33, 46, 48, 13, 39, 21, 89, 5, 87, 80, 35, 65, 73, 9, 26, 15, 72, 98, 38, 85, 90, 66, 20, 8, 43, 29, 6, 74, 0, 2}

    for static in sample_graphs:
        g1 = sample_graphs[static]['directed']
        with pytest.raises(NotImplementedError):
            find_articulation_points(g1)
        
        g2 = sample_graphs[static]['undirected']
        assert find_articulation_points(g2) == random_graph_articulation_points


def test_find_shortest_path():
    """
    Tests find_shortest_path function.
    """
    for static in [True, False]:

        undirected_graph = create_graph(static=static, directed=False,
            vertices=['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 's'])
        undirected_graph_edges = [
            ('s', 'a', 7.0), ('s', 'b', 2.0), ('s', 'c', 3.0), ('a', 'b', 3.0),
            ('a', 'd', 4.0), ('b', 'h', 4.0), ('b', 'd', 1.0), ('c', 'l', 2.0),
            ('d', 'f', 5.0), ('l', 'i', 4.0), ('l', 'j', 4.0), ('i', 'k', 4.0),
            ('i', 'j', 6.0), ('j', 'k', 4.0), ('k', 'e', 5.0), ('f', 'h', 3.0),
            ('h', 'g', 2.0), ('g', 'e', 2.0)
        ]
        for edge in undirected_graph_edges:
            undirected_graph.add_edge(*edge)

        undirected_path = find_shortest_path(undirected_graph, 's', 'e')
        assert undirected_path == ['s', 'b', 'h', 'g', 'e']
        with pytest.raises(ValueError):
            find_shortest_path(undirected_graph, 'y', 'z')

        disconnected_undirected_graph = create_graph(
            static=static, directed=False, vertices=['a', 'b'])
        with pytest.raises(ValueError):
            find_shortest_path(disconnected_undirected_graph, 'a', 'b')

        directed_graph = create_graph(static=static, directed=True,
            vertices=['s', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'])
        directed_graph_edges = [
            ('s', 'a', 2.0), ('a', 'e', 3.0), ('e', 'h', 4.0), ('h', 'g', 1.0),
            ('h', 'e', 2.0), ('g', 'd', 2.0), ('d', 's', 20.0), ('e', 'b', 1.0),
            ('e', 'g', 6.0), ('b', 'c', 7.0), ('c', 'f', 5.0), ('f', 'b', 0.0)
        ]
        for edge in directed_graph_edges:
            directed_graph.add_edge(*edge)

        directed_path = find_shortest_path(directed_graph, 's', 'e')
        assert directed_path == ['s', 'a', 'e']

        disconnected_directed_graph = create_graph(
            static=static, directed=True, vertices=['a', 'b'])
        disconnected_directed_graph.add_edge('a', 'b')
        with pytest.raises(ValueError):
            find_shortest_path(disconnected_directed_graph, 'b', 'a')


def test_get_min_spanning_tree():
    """
    Tests get_min_spanning_tree function.
    """


def test_partition_graph():
    """
    Tests partition_graph function.
    """
    sample_vertices = list(range(1, 6))
    sample_edges = list(itertools.combinations(sample_vertices, 2))
    sample_graphs = {
        'static': {
            'undirected': create_graph(static=True, directed=False, vertices=sample_vertices),
            'directed': create_graph(static=True, directed=True, vertices=sample_vertices)
        },
        'dynamic': {
            'undirected': create_graph(static=False, directed=False, vertices=sample_vertices),
            'directed': create_graph(static=False, directed=True, vertices=sample_vertices)
        }
    }
    for graph in [g for s in sample_graphs for g in sample_graphs[s].values()]:
        for edge in sample_edges:
            if not graph.has_edge(*edge):
                graph.add_edge(*edge)
    for static in sample_graphs:
        g2 = create_graph(static=(static == 'static'), directed=False, vertices=[])
        with pytest.raises(ValueError):
            partition_graph(g2)
        g2.add_vertex(1)
        with pytest.raises(ValueError):
            partition_graph(g2)

        g = sample_graphs[static]['directed']
        with pytest.raises(NotImplementedError):
            partition_graph(g)

    for graphs in sample_graphs.values():
        input_graph = graphs['undirected']
        output_g1, output_g2, cutset = partition_graph(input_graph)
        # Check that all edges and vertices of original graph are in
        # the new graphs and the cutset.
        output_edges = output_g1.edges | output_g2.edges | cutset
        input_edges = input_graph.edges
        for edge in set(output_edges):
            output_edges.remove(edge)
            output_edges.add((tuple(sorted(edge[:2])), edge[2]))
        for edge in set(input_edges):
            input_edges.remove(edge)
            input_edges.add((tuple(sorted(edge[:2])), edge[2]))
        assert input_edges == output_edges
        assert set(output_g1.vertices + output_g2.vertices) \
            == set(input_graph.vertices)


def test_get_components():
    """
    Tests get_components function.
    """

    for static in [True, False]:
        g1 = create_graph(static=static, directed=False, vertices=['a', 'b', 'c'])
        components = get_components(g1, static=True)
        assert isinstance(components, set)
        for component in components:
            assert isinstance(component, StaticGraph)
            assert component.vertices in [['a'], ['b'], ['c']]
            assert len(get_components(component)) == 1

        dynamic_components = get_components(g1, static=False)
        for component in dynamic_components:
            assert isinstance(component, DynamicGraph)
            assert component.vertices in [['a'], ['b'], ['c']]
            assert len(get_components(component)) == 1

        g2 = create_graph(static=static, directed=True, vertices=['a', 'b', 'c'])
        with pytest.raises(NotImplementedError):
            get_components(g2)


def test_get_strongly_connected_components():
    """
    Tests get_strongly_connected_components function.
    """

    edges = [
        ('a', 'b'), ('b', 'c'), ('b', 'e'), ('b', 'f'), ('c', 'd'), ('c', 'g'),
        ('d', 'c'), ('d', 'h'), ('h', 'd'), ('h', 'g'), ('g', 'f'), ('f', 'g'),
        ('e', 'a'), ('e', 'f')
    ]
    component_vertices = [{'a', 'b', 'e'}, {'c', 'd', 'g'}, {'f', 'g'}]
    component_edges = [
        {('a', 'b'), ('b', 'e'), ('e', 'a')}, {('f', 'g'), ('g', 'f')},
        {('h', 'd'), ('d', 'h'), ('d', 'c'), ('c', 'd')}
    ]

    for static in [True, False]:
        g = create_graph(directed=True, vertices=list(string.ascii_lowercase[:8]))
        for edge in edges:
            g.add_edge(*edge)
        test_components = get_strongly_connected_components(g)
        for component in test_components:
            assert set(component.vertices) in component_vertices
            test_component_edges = {e[:-1] for e in component.edges}
            assert test_component_edges in component_edges

        g2 = create_graph(directed=False)
        with pytest.raises(NotImplementedError):
            get_strongly_connected_components(g2)