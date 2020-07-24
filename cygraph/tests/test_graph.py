"""
Unit tests for classes implemented in cygraph/graph.pyx
"""


import pytest

from .. import create_graph


class TestGraph:
    """
    Unit tests for the cygraph.StaticGraph and cygraph.DynamicGraph class.
    """

    def test_constructor(self):
        """
        Tests initialization of a cygraph.StaticGraph and cygraph.DynamicGraph objects.
        """

        # Smoke tests.
        vertex_lists = [["s", "e"], [0, 1]]
        for lst in vertex_lists:
            for directed in [True, False]:
                for static in [True, False]:
                    create_graph(static=static, directed=directed, vertices=lst)

        # Exception-raising tests.
        for directed in [True, False]:
            for static in [True, False]:
                with pytest.raises(TypeError):
                    # Non-hashable type vertices.
                    create_graph(static=static, directed=directed,
                        vertices=[["s"], ["e"]])
    
    def test_edges(self):
        """
        Tests:
         - add_edge
         - get_children
         - edges
        for StaticGraph and DynamicGraph classes.
        """

        for static in [True, False]:
            # Directed graph.
            g = create_graph(static=static, directed=True, vertices=["s", "a", "b", "e"])

            # add_edge method
            g.add_edge("s", "a", weight=0.0)  # Make sure weight zero edges are tested.
            g.add_edge("a", "b")
            g.add_edge("b", "e")
            with pytest.raises(ValueError):
                g.add_edge("d", "f")

            # edges property
            g_edges = g.edges
            assert len(g_edges) == 3
            assert g_edges == {("s", "a", 0.0), ("a", "b", 1.0), ("b", "e", 1.0)}

            # get_children
            assert g.get_children("s") == {"a"}
            with pytest.raises(ValueError):
                g.get_children("d")

            # Undirected graph.
            g2 = create_graph(static=static, directed=False, vertices=["s", "a", "b", "e"])

            # add_edge method
            g2.add_edge("s", "a", weight=0.0)  # Make sure weight zero edges are tested.
            g2.add_edge("a", "b")
            g2.add_edge("b", "e")
            with pytest.raises(ValueError):
                g2.add_edge("d", "f")

            # edges property
            g2_edges = g2.edges
            assert len(g2_edges) == 3
            for edge in g2_edges:
                assert set(edge) in [{"s", "a", 0.0}, {"a", "b", 1.0}, {"b", "e", 1.0}]

            # get_children
            assert g2.get_children("s") == {"a"}
            with pytest.raises(ValueError):
                g2.get_children("d")

    def test_vertices(self):
        """
        Tests add_vertex method and vertices property
        for StaticGraph and DynamicGraph classes
        """
        for static in [True, False]:
            g = create_graph(static=static, directed=True)

            # Adding to graph with no vertices.
            g.add_vertex("s")
            # Adding to graph with vertices.
            g.add_vertex(1)
            with pytest.raises(TypeError):
                g.add_vertex([])
            print(g.vertices)
            with pytest.raises(ValueError):
                g.add_vertex("s")

            # vertices property
            assert g.vertices == ["s", 1]
    
    def test_attributes(self):
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
            g = create_graph(static=static, directed=True, vertices=["a", "b", "c"])
            g.add_edge("a", "b")

            # Setting attributes.
            g.set_edge_attribute(("a", "b"), key="key", val="val")
            with pytest.raises(TypeError):
                g.set_edge_attribute(("a", "b"), key=[], val="val")
            with pytest.raises(ValueError):
                g.set_edge_attribute(("b", "a"), key="key 2", val="val")
            g.set_edge_attribute(("a", "b"), key="key", val="new val")

            # Getting attributes.
            assert g.get_edge_attribute(("a", "b"), key="key") == "new val"
            with pytest.raises(TypeError):
                g.get_edge_attribute(("a", "b"), key=[])
            with pytest.raises(ValueError):
                g.get_edge_attribute(("b", "a"), key="key")
            with pytest.raises(KeyError):
                g.get_edge_attribute(("a", "b"), key="this is not a key")
            
            # Undirected graph.
            g2 = create_graph(static=static, directed=False, vertices=["a", "b", "c"])
            g2.add_edge("a", "b")

            # Setting attributes.
            g2.set_edge_attribute(("a", "b"), key="key", val="val")
            with pytest.raises(TypeError):
                g2.set_edge_attribute(("a", "b"), key=[], val="val")
            with pytest.raises(ValueError):
                g2.set_edge_attribute(("a", "c"), key="key 2", val="val")
            g2.set_edge_attribute(("a", "b"), key="key", val="new val")

            # Getting attributes.
            assert g2.get_edge_attribute(("a", "b"), key="key") == "new val"
            assert g2.get_edge_attribute(("b", "a"), key="key") == "new val"
            with pytest.raises(TypeError):
                g2.get_edge_attribute(("a", "b"), key=[])
            with pytest.raises(ValueError):
                g2.get_edge_attribute(("a", "c"), key="key")
            with pytest.raises(KeyError):
                g2.get_edge_attribute(("a", "b"), key="this is not a key")
    
    # Vertex attributes.
    for static in [True, False]:
        for directed in [True, False]:
            g = create_graph(static=static,
                    directed=directed, vertices=["a", "b", "c"])
            
            g.set_vertex_attribute("a", key="key", val="val")
            with pytest.raises(TypeError):
                g.set_vertex_attribute("a", key=[], val="val")
            with pytest.raises(ValueError):
                g.set_vertex_attribute("d", key="key", val="val")

            assert g.get_vertex_attribute("a", key="key") == "val"
            with pytest.raises(TypeError):
                g.get_vertex_attribute("a", key=[])
            with pytest.raises(ValueError):
                g.get_vertex_attribute("d", key="key")
            with pytest.raises(KeyError):
                g.get_vertex_attribute("a", key="this is not a key")