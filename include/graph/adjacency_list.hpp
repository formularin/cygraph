/*
An implementation of the graph data structure using an adjacency list.
*/

#ifndef CYGRAPH_ADJACENCY_LIST_HPP
#define CYGRAPH_ADJACENCY_LIST_HPP

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "graph.hpp"


using std::vector;


namespace cygraph {
    template <class Vertex, class EdgeWeight>
    class AdjacencyListGraph: public cygraph::Graph<Vertex, EdgeWeight> {

        protected:
            std::unordered_map<Vertex, std::unordered_set<std::pair<Vertex, EdgeWeight>>>
            adjacency_list;

        public:
            AdjacencyListGraph() {
                /*
                Default constructor.
                */
            }

            AdjacencyMatrixGraph(bool directed, const vector<Vertex>& vertices) {
                /*
                Class constructor.
                */

                this->directed = directed;
                this->vertices = vertices;
                this->n_vertices = vertices.size();

                // Initialize adjacency list.
                for ( int i = 0; i < this->n_vertices; i++ ) {
                    adjacency_list[this->vertices[i]] = std::unordered_set();
                }
            }

        EdgeWeight get_edge_weight(const Vertex& u, const Vertex& v) override {
            /*
            Returns the weight of an edge.
            */

            for ( std::pair<Vertex, EdgeWeight> child : adjacency_list[u] ) {
                if ( child.first == v ) {
                    return child.second;
                }
            }
            throw std::invalid_argument("Edge does not exist.");
        }

        void add_vertex(const Vertex& v) override {
            /*
            Adds a vertex to the graph.
            */

            if ( std::find(this->vertices.begin(), this->vertices.end(), v)
                    != this->vertices.end() ) {
                throw std::invalid_argument("Vertex is already in graph.");
            }

            this->vertices.push_back(v);
            this->n_vertices++;

            // Add new list to adjacency list.
            adjacency_list[v] = std::unordered_set<std::pair<Vertex, EdgeWeight>>();
        }

        void add_vertices(Vertex vertices[], int n_vertices) override {
            /*
            Adds an array of vertices to the graph.
            */

            // Check if vertices are already in graph.
            Vertex v;
            for ( int i = 0; i < n_vertices; i++ ) {
                v = vertices[i];
                if ( std::find(this->vertices.begin(),
                                this->vertices.end(), v)
                        != this->vertices.end() ) {

                    throw std::invalid_argument("Vertex is already in graph.");
                }
            }

            this->vertices.insert(this->vertices.end(), vertices, vertices + n_vertices);
            this->n_vertices += n_vertices;

            // Add new lists to adjacency list.
            for ( int i = 0; i < n_vertices; i++ ) {
                adjacency_list[v] = std::unordered_set<std::pair<Vertex, EdgeWeight>>();
            }
        }
    };
}


#endif