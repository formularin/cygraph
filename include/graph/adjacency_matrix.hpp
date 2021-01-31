/*
An implementation of the graph data structure using an adjacency matrix.
*/

#ifndef CYGRAPH_ADJACENCY_MATRIX_HPP
#define CYGRAPH_ADJACENCY_MATRIX_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "graph.hpp"


using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::vector;


// Overload std::hash for pair<int, int>
namespace std {
    template <> struct hash<pair<int, int>> {
        size_t operator()(const pair<int, int>& object) const {
            size_t a_hash = hash<int>()(object.first);
            size_t b_hash = hash<int>()(object.second) << 1;
            return a_hash ^ b_hash;
        }
    };
}


namespace cygraph {

    template<class Vertex, class EdgeWeight>
    class AdjacencyMatrixGraph: public cygraph::Graph<Vertex, EdgeWeight> {
        /*
        A graph class implemented using an adjacency matrix. Vertex type must have std::hash
        overridden.
        */

        protected:

        vector<vector<EdgeWeight*>> adjacency_matrix;
        // Exists so that temporary memory addresses are not saved in adjacency matrix. All 
        // adjacency matrix values are pointing to this vector.
        unordered_map<pair<int, int>, EdgeWeight> edge_weights;
        unordered_map<Vertex, int> vertex_indices;
        vector<Vertex> vertices;

        int get_vertex_int(const Vertex& v);
        /*
        Returns the corresponding integer to a vertex.
        */

        public:

        AdjacencyMatrixGraph() {}
        /*
         * Default constructor.
         */

        AdjacencyMatrixGraph(bool directed, const unordered_set<Vertex>& vertices);
        /*
         * Class constructor
         */

        vector<Vertex> get_vertices() { return vertices; }
        /*
         * Returns the vertices in the graph.
         */

        EdgeWeight get_edge_weight(const Vertex& u, const Vertex& v) override;
        /*
         * Returns the weight of an edge (u, v).
         * Throws std::invalid_argument if edge does not exist.
         */

        void add_vertex(const Vertex& v) override;
        /*
         * Adds a vertex to the graph.
         * 
         * Throws std::invalid_argument if v is already in the graph.
         */

        void add_vertices(const unordered_set<Vertex>& vertices) override;
        /*
         * Adds a set of vertices to the graph.
         * Throws std::invalid_argument if any of the vertices are already in the graph.
         */

        void remove_vertex(const Vertex& v) override;
        /*
         * Removes a vertex from the graph.
         * Throws std::invalid_argument if the vertex is not in the graph.
         */

        void set_edge_weight(const Vertex& u, const Vertex& v, const EdgeWeight& weight) override;
        /*
         * Sets the weight of the edge (u, v)
         */

        void remove_edge(const Vertex& u, const Vertex& v) override;
        /*
         * Removes an edge from the graph.
         * 
         * Throws std::invalid_argument if attempting to remove an edge that doesn't exist,
         * including if one or more of the vertices is not in the graph.
         */

        bool has_edge(const Vertex& u, const Vertex& v) override;
        /*
         * Returns whether or not a given edge is in the graph. If one or more of the vertices are
         * not in the graph, false is returned and a warning is thrown.
         */

        bool has_vertex(const Vertex& v) override;
        /*
         * Returns whether or not a certain vertex is in the vertices vector.
         */

        unordered_set<Vertex> get_children(const Vertex& v) override;
        /*
         * Returns the children of a given vertex in the graph. In an undirected graph, this is
         * equivalent to finding the "neighbors" of a vertex, and is the same as the method
         * get_parents.
        */

        unordered_set<Vertex> get_parents(const Vertex& v) override;
        /*
         * Returns the parents of a given vertex in the graph. In an undirected graph, this is
         * equivalent to finding the "neighbors" of a vertex, and is the same as the method
         * get_children.
        */
    };
}

#include "adjacency_matrix.tpp"

#endif