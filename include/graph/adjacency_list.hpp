/*
An implementation of the graph data structure using an adjacency list.
*/

#ifndef CYGRAPH_ADJACENCY_LIST_HPP
#define CYGRAPH_ADJACENCY_LIST_HPP

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "graph.hpp"

using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::vector;


namespace cygraph {

    template<class Vertex, class EdgeWeight>
    class AdjacencyListGraphCommon: public cygraph::Graph<Vertex, EdgeWeight> {
        /*
        Contains common functionality between weighted and unweighted adjacency list graphs.
        */

        protected:

        unordered_set<Vertex> vertices;

        public:

        unordered_set<Vertex> get_vertices() { return vertices; }
        /*
        Returns the vertices in the graph.
        */

        void add_vertex(const Vertex& v) override;
        /*
        Checks if the vertex is already in the graph and adds if not.
        */

        void add_vertices(const unordered_set<Vertex>& vertices) override;
        /*
        Checks if any of the vertices are already in the graph and adds them all if not.
        */

        void remove_vertex(const Vertex& v) override;
        /*
        Checks if vertex is already in graph and removes it if it is.
        */

        bool has_vertex(const Vertex& v) override;
        /*
        Returns whether or not a certain vertex is in the graph.
        */
    };

    template<class Vertex, class EdgeWeight>
    class AdjacencyListGraph: public AdjacencyListGraphCommon<Vertex, EdgeWeight> {
        /*
        A graph class implemented using an adjacency list. Vertex type must have std::hash
        overriden.
        */

        private:

        unordered_map<Vertex, vector<pair<Vertex, EdgeWeight>>> adjacency_list;

        public:

        AdjacencyListGraph() {
            /*
            Default constructor.
            */
        }

        AdjacencyListGraph(bool directed, const unordered_set<Vertex>& vertices);
        /*
        Class constructor.
        */

        EdgeWeight get_edge_weight(const Vertex& u, const Vertex& v) override;
        /*
        Returns the weight of an edge.
        */

        void add_vertex(const Vertex& v) override;
        /*
        Adds a vertex to the graph.
        */

        void add_vertices(const unordered_set<Vertex>& vertices) override;
        /*
        Adds an array of vertices to the graph.
        */

        void remove_vertex(const Vertex& v) override;
        /*
        Removes a vertex from the graph.
        */

        void set_edge_weight(const Vertex& u, const Vertex& v, const EdgeWeight& weight);
            /*
            Sets the weight of an edge.
            */

        void remove_edge(const Vertex& u, const Vertex& v) override;
        /*
        Removes an edge from the graph. A warning is raised if attempting to remove an edge
        that doesn't exist.
        */

        bool has_edge(const Vertex &u, const Vertex& v) override;
        /*
        Returns whether or not a given edge is in the graph. If one or more of the vertices are
        not in the graph, false is returned.
        */

        unordered_set<Vertex> get_children(const Vertex& v) override;
        /*
        Returns the children of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_parents.
        */

        unordered_set<Vertex> get_parents(const Vertex& v) override;
        /*
        Returns the parents of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_children.
        */
    };

    template<class Vertex>
    class AdjacencyListGraph<Vertex, bool>: public AdjacencyListGraphCommon<Vertex, bool> {
        /*
        A graph class implemented using an adjacency list, without edge weight functionality.
        Vertex type must have std::hash overriden.
        */

        private:

        unordered_map<Vertex, unordered_set<Vertex>> adjacency_list;

        public:
        AdjacencyListGraph() {
            /*
            Default constructor.
            */
        }

        AdjacencyListGraph(bool directed, const unordered_set<Vertex>& vertices);
        /*
        Class constructor.
        */

        bool get_edge_weight(const Vertex& u, const Vertex& v) override;
        /*
        Returns whether or not an edge exists. An alias to has_edge except throws an error if
        one of the vertices is not in the graph.
        */

        void add_vertex(const Vertex& v) override;
        /*
        Adds a vertex to the graph.
        */

        void add_vertices(const unordered_set<Vertex>& vertices) override;
        /*
        Adds an array of vertices to the graph.
        */

        void remove_vertex(const Vertex& v) override;
        /*
        Removes a vertex from the graph.
        */

        void set_edge_weight(const Vertex& u, const Vertex& v, const bool& weight) override;
        /*
        Adds or removes an edge.
        */

        void add_edge(const Vertex& u, const Vertex& v);
        /*
        Adds an edge between two vertices in the graph.
        */

        void add_edges(const vector<pair<Vertex, Vertex>>& edges);
        /*
        Adds multiple edges to the graph.
        */

        void remove_edge(const Vertex& u, const Vertex& v) override;
        /*
        Removes an edge from the graph.
        */

        bool has_edge(const Vertex& u, const Vertex& v) override;
        /*
        Returns whether or not a given edge is in the graph. If one or more of the vertices are
        not in the graph, false is returned.
        */

        unordered_set<Vertex> get_children(const Vertex& v) override;
        /*
        Returns the children of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_parents.
        */

        unordered_set<Vertex> get_parents(const Vertex& v) override;
        /*
        Returns the parents of a given vertex in the graph. In an undirected graph, this is
        equivalent to finding the "neighbors" of a vertex, and is the same as the method
        get_children.
        */
    };
}

#include "adjacency_list.tpp"

#endif