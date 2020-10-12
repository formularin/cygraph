/*
An implementation of the graph data structure using an adjacency matrix.
*/

#ifndef CYGRAPH_ADJACENCY_MATRIX_HPP_
#define CYGRAPH_ADJACENCY_MATRIX_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "graph.hpp"


namespace cygraph {

    template <class Vertex, class EdgeWeight>
    class AdjacencyMatrixGraph: public cygraph::Graph<Vertex, EdgeWeight> {

        private:
            EdgeWeight*** adjacency_matrix;

        public:
            AdjacencyMatrixGraph(bool directed,
                    std::vector<Vertex>& vertices) {
                /*
                Class constructor.
                */
                
                this->directed = directed;
                this->vertices = vertices;
                this->n_vertices = vertices.size();

                // Create adjacency matrix, attribute dictionaries, and
                // vertex indices dictionary.
                for ( int i = 0; i < this->n_vertices; i++ ) {
                    this->vertex_indices[this->vertices[i]] = i;
                    for ( int j = 0; j < this->n_vertices; j++ ) {
                        adjacency_matrix[i][j] = nullptr;
                    }
                }
            }
    };
}

#endif