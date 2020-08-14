"""
Unit tests for classes implemented in cygraph/graph.pyx
"""

import pytest

from .. import create_graph
from ..graph import Graph


def test_constructor():
    """
    Tests initialization of a StaticGraph and DynamicGraph objects.
    """

    graphs = []

    # Smoke tests.
    vertex_lists = [['s', 'e'], [0, 1]]
    for lst in vertex_lists:
        for directed in [True, False]:
            for static in [True, False]:
                g = create_graph(static=static, directed=directed, vertices=lst)
                graphs.append(g)

    # Exception-raising tests.
    for directed in [True, False]:
        for static in [True, False]:
            with pytest.raises(TypeError):
                # Non-hashable type vertices.
                g = create_graph(static=static, directed=directed,
                    vertices=[['s'], ['e']])
                graphs.append(g)

    for graph in graphs:
        g = create_graph(graph=graph)
        assert g.vertices == graph.vertices
        assert g.edges == graph.edges

def test_edges():
    """
    Tests:
        - add_edge
        - get_children
        - edges
        - has_edge
        - remove_edge
    for StaticGraph and DynamicGraph classes.
    """

    for static in [True, False]:
        # Directed graph.
        g = create_graph(static=static, directed=True, vertices=['s', 'a', 'b', 'e'])

        # add_edge method
        g.add_edge('s', 'a', weight=0.0)  # Make sure weight zero edges are tested.
        g.add_edge('a', 's')
        g.add_edge('a', 'b')
        g.add_edge('b', 'e')
        with pytest.raises(ValueError):
            g.add_edge('d', 'f')

        # edges property
        g_edges = g.edges
        assert g_edges == {('s', 'a', 0.0), ('a', 'b', 1.0), ('b', 'e', 1.0), ('a', 's', 1.0)}

        # has_edge
        for edge in g_edges:
            assert g.has_edge(edge[0], edge[1]) == True
        assert g.has_edge('e', 'b') == False
        assert g.has_edge(1, 2) == False

        # remove_edge
        g.remove_edge('s', 'a')
        with pytest.raises(ValueError):
            g.remove_edge('sdaf', 'dsafsd')
        with pytest.warns(Warning):
            g.remove_edge('s', 'e')
        assert g.has_edge('s', 'a') == False
        assert g.has_edge('a', 's') == True

        # get_edge_weight
        assert g.get_edge_weight('a', 'b') == 1.0
        with pytest.raises(ValueError):
            g.get_edge_weight('b', 'a')

        # get_children
        assert g.get_children('a') == {'s', 'b'}
        with pytest.raises(ValueError):
            g.get_children('d')

        # Undirected graph.
        g2 = create_graph(static=static, directed=False, vertices=['s', 'a', 'b', 'e'])

        # add_edge method
        g2.add_edge('s', 'a', weight=0.0)  # Make sure weight zero edges are tested.
        g2.add_edge('a', 'b')
        g2.add_edge('b', 'e')
        with pytest.raises(ValueError):
            g2.add_edge('d', 'f')

        # edges property
        g2_edges = g2.edges
        assert len(g2_edges) == 3
        for edge in g2_edges:
            assert set(edge) in [{'s', 'a', 0.0}, {'a', 'b', 1.0}, {'b', 'e', 1.0}]

        # has_edge
        for edge in g2_edges:
            assert g2.has_edge(edge[0], edge[1]) == True
        assert g2.has_edge('e', 'b') == True
        assert g2.has_edge(1, 2) == False

        # remove_edge
        g2.remove_edge('s', 'a')
        with pytest.raises(ValueError):
            g2.remove_edge('sdaf', 'dsafsd')
        with pytest.warns(Warning):
            g2.remove_edge('s', 'e')
        assert g2.has_edge('s', 'a') == False
        assert g2.has_edge('a', 's') == False

        # get_edge_weight
        assert g2.get_edge_weight('b', 'a') == 1.0
        with pytest.raises(ValueError):
            g2.get_edge_weight('d', 'e')

        # get_children
        assert g2.get_children('a') == {'b'}
        with pytest.raises(ValueError):
            g2.get_children('d')

def test_vertices():
    """
    Tests methods:
        - add_vertex 
        - has_vertex
        - vertices
        - remove_vertex
    for StaticGraph and DynamicGraph classes.
    """
    for static in [True, False]:
        g = create_graph(static=static, directed=True)

        # Adding to graph with no vertices.
        g.add_vertex('s')
        # Adding to graph with vertices.
        g.add_vertex(1)
        with pytest.raises(TypeError):
            g.add_vertex([])
        with pytest.raises(ValueError):
            g.add_vertex('s')

        # vertices property
        assert g.vertices == ['s', 1]

        # has_vertex
        assert g.has_vertex('s') == True
        assert g.has_vertex(1) == True
        assert g.has_vertex(2) == False

        # Remove vertices.
        g.remove_vertex('s')
        with pytest.raises(ValueError):
            g.remove_vertex(2)
        assert g.has_vertex('s') == False

def test_attributes():
    """
    Tests:
        - set_vertex_attribute
        - get_vertex_attribute
        - set_edge_attribute
        - get_edge_attribute
    for StaticGraph and DynamicGraph classes
    """

    # Edge attributes.
    for static in [True, False]:
        # Directed graph.
        g = create_graph(static=static, directed=True, vertices=['a', 'b', 'c'])
        g.add_edge('a', 'b')

        # Setting attributes.
        g.set_edge_attribute(('a', 'b'), key='key', val='val')
        with pytest.raises(TypeError):
            g.set_edge_attribute(('a', 'b'), key=[], val='val')
        with pytest.raises(ValueError):
            g.set_edge_attribute(('b', 'a'), key='key 2', val='val')
        g.set_edge_attribute(('a', 'b'), key='key', val='new val')

        # Getting attributes.
        assert g.get_edge_attribute(('a', 'b'), key='key') == 'new val'
        with pytest.raises(TypeError):
            g.get_edge_attribute(('a', 'b'), key=[])
        with pytest.raises(ValueError):
            g.get_edge_attribute(('b', 'a'), key='key')
        with pytest.raises(KeyError):
            g.get_edge_attribute(('a', 'b'), key="this is not a key")

        # Undirected graph.
        g2 = create_graph(static=static, directed=False, vertices=['a', 'b', 'c'])
        g2.add_edge('a', 'b')

        # Setting attributes.
        g2.set_edge_attribute(('a', 'b'), key='key', val='val')
        with pytest.raises(TypeError):
            g2.set_edge_attribute(('a', 'b'), key=[], val='val')
        with pytest.raises(ValueError):
            g2.set_edge_attribute(('a', 'c'), key='key 2', val='val')
        g2.set_edge_attribute(('a', 'b'), key='key', val='new val')

        # Getting attributes.
        assert g2.get_edge_attribute(('a', 'b'), key='key') == 'new val'
        assert g2.get_edge_attribute(('b', 'a'), key='key') == 'new val'
        with pytest.raises(TypeError):
            g2.get_edge_attribute(('a', 'b'), key=[])
        with pytest.raises(ValueError):
            g2.get_edge_attribute(('a', 'c'), key='key')
        with pytest.raises(KeyError):
            g2.get_edge_attribute(('a', 'b'), key="this is not a key")

    # Vertex attributes.
    for static in [True, False]:
        for directed in [True, False]:
            g = create_graph(static=static,
                    directed=directed, vertices=['a', 'b', 'c'])

            g.set_vertex_attribute('a', key='key', val='val')
            with pytest.raises(TypeError):
                g.set_vertex_attribute('a', key=[], val='val')
            with pytest.raises(ValueError):
                g.set_vertex_attribute('d', key='key', val='val')

            assert g.get_vertex_attribute('a', key='key') == 'val'
            with pytest.raises(TypeError):
                g.get_vertex_attribute('a', key=[])
            with pytest.raises(ValueError):
                g.get_vertex_attribute('d', key='key')
            with pytest.raises(KeyError):
                g.get_vertex_attribute('a', key='this is not a key')

def test_not_implemented(subtests):
    """Tests that abstract methods raise NotImplementedErrors.
    """

    g = Graph()

    methods = {
        "add_vertex": [0],
        "remove_vertex": [0],
        "add_edge": [0, 1],
        "get_edge_weight": [0, 1],
        "get_children": [0]
    }

    for method, args in methods.items():
        with subtests.test(method=method):
            with pytest.raises(NotImplementedError):
                eval(f"g.{method}(*{repr(args)})")

    with pytest.raises(NotImplementedError):
        g.edges