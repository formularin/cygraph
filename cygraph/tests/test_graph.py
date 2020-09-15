"""Unit tests for classes implemented in cygraph/graph.pyx
"""

import numpy as np
import pytest

import cygraph as cg


def test_constructor():
    """Tests initialization of a StaticGraph and DynamicGraph objects.
    """

    graphs = []

    # Smoke tests.
    cg.graph()
    vertex_lists = [['s', 'e'], [0, 1]]
    for lst in vertex_lists:
        for directed in [True, False]:
            for static in [True, False]:
                g = cg.graph(static=static, directed=directed, vertices=lst)
                graphs.append(g)

    # Exception-raising tests.
    for directed in [True, False]:
        for static in [True, False]:
            with pytest.raises(TypeError):
                # Non-hashable type vertices.
                g = cg.graph(static=static, directed=directed,
                    vertices=[['s'], ['e']])
                graphs.append(g)

    # Generating from graph data structures.
    adjacency_matrix = [[0, 1, 0, 0, 0, 0, 0],
                        [1, 0, 1, 0, 0, 0, 0],
                        [0, 1, 0, 1, 1, 1, 0],
                        [0, 0, 1, 0, 0, 0, 0],
                        [0, 0, 1, 0, 0, 1, 0],
                        [0, 0, 1, 0, 1, 0, 1],
                        [0, 0, 0, 0, 0, 1, 0]]
    adjacency_list = [
        [1],
        [0, 2],
        [1, 3, 4, 5],
        [2],
        [2, 5],
        [2, 4, 6],
        [5]
    ]
    edges = [(0, 1), (1, 2), (2, 3), (2, 4), (2, 5), (4, 5), (5, 6)]
    for static in [True, False]:
        adj_list_graph = cg.graph(static=static, vertices=list(range(7)),
            adjacency_list=adjacency_list)
        if static:
            np_adjacency_matrix = [[val if val else np.nan for val in row]
                                   for row in adjacency_matrix]
            np_adjacency_matrix = np.array(np_adjacency_matrix)
            adj_mat_graph = cg.graph(static=static, vertices=list(range(7)),
                adjacency_matrix=np_adjacency_matrix)
        else:
            adj_mat_graph = cg.graph(static=static, vertices=list(range(7)),
                adjacency_matrix=[[val if val else None for val in row]
                                  for row in adjacency_matrix])
        for edge in edges:
            assert adj_list_graph.has_edge(*edge)
            assert adj_mat_graph.has_edge(*edge)
        for edge in adj_mat_graph.edges:
            assert edge[:-1] in edges
        for edge in adj_list_graph.edges:
            assert edge[:-1] in edges
        graphs.append(adj_list_graph)
        graphs.append(adj_mat_graph)

    # Copying another graph.
    for graph in graphs:
        g = cg.graph(graph_=graph)
        assert g.vertices == graph.vertices
        assert g.edges == graph.edges


def test_edges():
    """Tests various edge-related methods.

    Tests:
        - add_edge
        - get_children
        - get_parents
        - edges
        - has_edge
        - remove_edge
        - adjacency_matrix
        - adjacency_list
        - add_edges

    for StaticGraph and DynamicGraph classes.
    """

    for static in [True, False]:
        # Directed graph.
        g = cg.graph(static=static, directed=True, vertices=['s', 'a', 'b', 'e'])

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
            assert g.has_edge(edge[0], edge[1])
        assert not g.has_edge('e', 'b')
        assert not g.has_edge(1, 2)

        # remove_edge
        g.remove_edge('s', 'a')
        with pytest.raises(ValueError):
            g.remove_edge('sdaf', 'dsafsd')
        with pytest.warns(Warning):
            g.remove_edge('s', 'e')
        assert not g.has_edge('s', 'a')
        assert g.has_edge('a', 's')

        # get_edge_weight
        assert g.get_edge_weight('a', 'b') == 1.0
        with pytest.raises(ValueError):
            g.get_edge_weight('b', 'a')

        # get_children
        assert g.get_children('a') == {'s', 'b'}
        with pytest.raises(ValueError):
            g.get_children('d')

        # get_parents
        assert g.get_parents('e') == {'b'}
        with pytest.raises(ValueError):
            g.get_parents('d')

        # add_edges
        g.add_edges({('b', 'a', 2.0), ('e', 'b')})
        assert g.get_edge_weight('b', 'a') == 2.0
        assert g.get_edge_weight('e', 'b') == 1.0
        with pytest.raises(ValueError):
            g.add_edges({('s', 'a'), ('sdaf', 'dsafsd')})
        assert not g.has_edge('s', 'a')

        # Undirected graph.
        g2 = cg.graph(static=static, directed=False, vertices=['s', 'a', 'b', 'e'])

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
            assert g2.has_edge(edge[0], edge[1])
        assert g2.has_edge('e', 'b')
        assert not g2.has_edge(1, 2)

        # remove_edge
        g2.remove_edge('s', 'a')
        with pytest.raises(ValueError):
            g2.remove_edge('sdaf', 'dsafsd')
        with pytest.warns(Warning):
            g2.remove_edge('s', 'e')
        assert not g2.has_edge('s', 'a')
        assert not g2.has_edge('a', 's')

        # get_edge_weight
        assert g2.get_edge_weight('b', 'a') == 1.0
        with pytest.raises(ValueError):
            g2.get_edge_weight('d', 'e')

        # get_children
        assert g2.get_children('a') == {'b'}
        with pytest.raises(ValueError):
            g2.get_children('d')

        # get_parents
        assert g2.get_parents('a') == {'b'}
        with pytest.raises(ValueError):
            g2.get_parents('d')

        # add_edges
        g.remove_edge('e', 'b')
        g.add_edges({('b', 'a', 2.0), ('e', 'b')})
        assert g.get_edge_weight('b', 'a') == 2.0
        assert g.get_edge_weight('e', 'b') == 1.0
        with pytest.raises(ValueError):
            g.add_edges({('s', 'a'), ('sdaf', 'dsafsd')})
        assert not g.has_edge('s', 'a')

        # adjacency_matrix and adjacency_list
        g = cg.graph(static=static, directed=False, vertices=list(range(3)))
        g.add_edge(0, 1)
        g.add_edge(1, 2)
        undirected_adj = [[0, 1, 0],
                          [1, 0, 1],
                          [0, 1, 0]]
        if static:
            assert (np.nan_to_num(g.adjacency_matrix) == undirected_adj).all()
        else:
            assert g.adjacency_matrix == \
                [[None if not x else x for x in lst] for lst in undirected_adj]
        assert g.adjacency_list == [[1], [0, 2], [1]]
        g = cg.graph(static=static, directed=True, vertices=list(range(3)))
        g.add_edge(0, 1)
        g.add_edge(1, 2)
        directed_adj = [[0, 1, 0],
                        [0, 0, 1],
                        [0, 0, 0]]
        if static:
            assert (np.nan_to_num(g.adjacency_matrix) == directed_adj).all()
        else:
            assert g.adjacency_matrix == \
                [[None if not x else x for x in lst] for lst in directed_adj]
        assert g.adjacency_list == [[1], [2], []]


def test_vertices():
    """Tests various vertex-related methods.

    Tests methods:
        - add_vertex
        - has_vertex
        - vertices
        - remove_vertex
        - add_vertices

    for StaticGraph and DynamicGraph classes.
    """
    for static in [True, False]:
        g = cg.graph(static=static, directed=True)

        # Adding to graph with no vertices.
        g.add_vertex('s')
        # Adding to graph with vertices.
        g.add_vertex(1)
        with pytest.raises(TypeError):
            g.add_vertex([])
        with pytest.raises(ValueError):
            g.add_vertex('s')

        # vertices attribute
        assert g.vertices == ['s', 1]

        # has_vertex
        assert g.has_vertex('s')
        assert g.has_vertex(1)
        assert not g.has_vertex(2)

        # remove_vertex
        g.remove_vertex('s')
        with pytest.raises(ValueError):
            g.remove_vertex(2)
        assert not g.has_vertex('s')

        # add_vertices
        g.add_vertices({'a', 'b', 's'})
        assert g.has_vertex('a')
        assert g.has_vertex('b')
        assert g.has_vertex('s')
        with pytest.raises(ValueError):
            g.add_vertices({'c', 's'})
        assert not g.has_vertex('c')
        assert g.has_vertex('s')

def test_attributes():
    """Tests various edge and vertex attribute-related methods.

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
        g = cg.graph(static=static, directed=True, vertices=['a', 'b', 'c'])
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
        g2 = cg.graph(static=static, directed=False, vertices=['a', 'b', 'c'])
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
            g = cg.graph(static=static,
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


def test_comparisons():
    """Tests comparison operations between graphs.
    """
    # __eq__ and equals
    for directed in [True, False]:
        for static1 in [True, False]:
            for static2 in [True, False]:
                g1 = cg.graph(static=static1, directed=directed,
                    vertices=list(range(3)))
                g2 = cg.graph(static=static2, directed=directed,
                    vertices=list(range(3)))
                with pytest.raises(ValueError):
                    g1 == g2
                    g2 == g1
                assert g1.equals(g2)
                assert g2.equals(g1)
                g1.add_edge(0, 1)
                assert not g1.equals(g2)
                assert not g2.equals(g1)
                g2.add_edge(0, 1)
                assert g1.equals(g2)
                assert g2.equals(g1)
                g1.set_edge_attribute((0, 1), key="Attribute", val=True)
                assert not g1.equals(g2, edge_attrs=True)
                assert not g2.equals(g1, edge_attrs=True)
                g1.set_vertex_attribute(0, key="Attribute", val=True)
                assert not g1.equals(g2, vertex_attrs=True)
                assert not g2.equals(g1, vertex_attrs=True)
