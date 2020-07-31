"""
Unit tests for algorithms implemented in cygraph/algorithms.pyx
"""

import pytest

from .. import create_graph, StaticGraph, DynamicGraph
from ..algorithms import (
    find_shortest_path,
    get_min_spanning_tree,
    partition_graph,
    get_components,
    get_strongly_connected_components
)


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
        with pytest.raises(ValueError):
            get_components(g2)


def test_get_strongly_connected_components():
    """
    Tests get_strongly_connected_components function.
    """