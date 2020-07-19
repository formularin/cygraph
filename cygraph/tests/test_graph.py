"""
Unit tests for classes implemented in cygraph/graph.pyx
"""


import pytest

from .. import Graph


NORMAL_GRAPH_VERTICES = [""]


class TestGraph:
    """
    Unit tests for the cygraph.Graph class.
    """

    def test_constructor(self):
        """
        Tests initialization of a cygraph.Graph object.
        """

        # Smoke tests.
        vertex_lists = [["s", "e"], [0, 1]]
        for lst in vertex_lists:
            for val in [True, False]:
                Graph(directed=val, vertices=lst)

        # Exception-raising tests.
        for case in test_cases:
            for val in [True, False]:
                with pytest.raises(TypeError):
                    # Non-hashable type vertices.
                    Graph(directed=val, vertices=[["s"], ["e"]])
    
    def test_edges(self):
        """
        Tests:
         - cygraph.Graph.add_edge
         - cygraph.Graph.get_children
         - cygraph.Graph.edges
         - cygraph.Graph.get_descendants
        """

        # Directed graph.
        g = Graph(directed=True, vertices=["s", "a", "b", "e"])

        # add_edge method
        g.add_edge("s", "a", weight=0.0)  # Make sure weight zero edges are tested.
        g.add_edge("a", "b")
        g.add_edge("b", "e")
        with pytest.raises(ValueError):
            g.add_edge("d", "f")

        # edges property
        g_edges = g.edges
        assert len(g_edges) == 3
        assert g_edges == {("s", "a"), ("a", "b"), ("b", "e")}

        # get_children
        assert g.get_children("s") == ["e"]
        with pytest.raises(ValueError):
            g.get_children("a")
        
        # get_descendants
        assert g.get_descendants("s") == {"a", "b", "e"}
        with pytest.raises(ValueError):
            g.get_descendants("d")

        # Undirected graph.
        g2 = Graph(directed=False, vertices=["s", "a", "b", "e"])

        # add_edge method
        g2.add_edge("s", "a", weight=0.0)  # Make sure weight zero edges are tested.
        g2.add_edge("a", "b")
        g2.add_edge("b", "e")
        with pytest.raises(ValueError):
            g2.add_edge("d", "f")

        # edges property
        g2_edges = g2.edges
        assert len(g2_edges) == 3
        assert {set(e) for e in g2_edges} == {{"s", "a"}, {"a", "b"}, {"b", "e"}}

        # get_children
        assert g2.get_children("s") == ["e"]
        with pytest.raises(ValueError):
            g2.get_children("a")
        
        # get_descendants
        assert g2.get_descendants("s") == {"a", "b", "e"}
        with pytest.raises(ValueError):
            g2.get_descendants("d")

    def test_add_node(self):
        """
        Tests cygraph.Graph.add_node
        """