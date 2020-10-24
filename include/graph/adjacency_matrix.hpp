/*
An implementation of the graph data structure using an adjacency matrix.
*/

#ifndef CYGRAPH_ADJACENCY_MATRIX_HPP
#define CYGRAPH_ADJACENCY_MATRIX_HPP

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "graph.hpp"


using std::vector;


namespace cygraph {

    template <class Vertex, class EdgeWeight>
    class AdjacencyMatrixGraph: public cygraph::Graph<Vertex, EdgeWeight> {

        protected:
            vector<vector<EdgeWeight*>> adjacency_matrix;

        public:
            AdjacencyMatrixGraph() {
                /*
                Default constructor.
                */
            }

            AdjacencyMatrixGraph(bool directed,
                    vector<Vertex>& vertices) {
                /*
                Class constructor.
                */
                
                this->directed = directed;
                this->vertices = vertices;
                this->n_vertices = vertices.size();

                // Initialize adjacency matrix and vertex indices
                // dictionary.
                adjacency_matrix.reserve(this->n_vertices);
                for ( int i = 0; i < this->n_vertices; i++ ) {
                    this->vertex_indices[this->vertices[i]] = i;
                    adjacency_matrix.emplace_back(
                        vector<EdgeWeight*>(this->n_vertices, nullptr));
                }
            }

            EdgeWeight get_edge_weight(const Vertex& u, const Vertex &v) {
                /*
                Returns the weight of an edge.
                */

                int u_index = this->vertex_indices[u];
                int v_index = this->vertex_indices[v];
                return *adjacency_matrix[u_index][v_index];
            }

            void add_vertex(const Vertex& v) {
                /*
                Adds a vertex to the graph.
                */

                if ( std::find(this->vertices.begin(), this->vertices.end(), v)
                        != this->vertices.end() ) {
                    throw std::invalid_argument("Vertex is already in graph.");
                }

                this->vertices.push_back(v);
                this->vertex_indices[v] = this->n_vertices;
                this->n_vertices++;

                // Add new column to adjacency matrix.
                for ( vector<Vertex>& row : adjacency_matrix ) {
                    row.push_back(nullptr);
                }
                // Add new row to adjacency matrix.
                adjacency_matrix.emplace_back(
                    vector<EdgeWeight*>(this->n_vertices, nullptr));
            }

            void add_vertices(Vertex vertices[], int n_vertices) {
                /*
                Adds an array of vertices to the graph.
                */

                for ( Vertex& v : vertices ) {
                    if (    std::find(this->vertices.begin(),
                                this->vertices.end(), v)
                            != this->vertices.end() ) {

                        throw std::invalid_argument("Vertex is already in graph.");
                    }
                }

                this->vertices.insert(this->vertices.end(), vertices,
                    vertices + n_vertices);
                // Update vertex indices dictionary.
                for ( int i = 0; i < n_vertices; i++ ) {
                    this->vertex_indices[vertices[i]] = n_vertices + i;
                }
                this->n_vertices += n_vertices;

                // Add new columns to adjacency matrix.
                EdgeWeight* new_edge_weights[n_vertices] = { nullptr };
                for ( vector<Vertex>& row : adjacency_matrix ) {
                    row.insert(row.end(), new_edge_weights,
                        new_edge_weights + n_vertices);
                }
                // Add new rows to adjacency matrix.
                vector<EdgeWeight*> new_rows[n_vertices] = 
                    { vector<EdgeWeight*>(this->n_vertices, nullptr) };
                adjacency_matrix.insert(adjacency_matrix.end(), new_rows,
                    new_rows + n_vertices);
            }

            void remove_vertex(const Vertex& v) {
                /*
                Removes a vertex from the graph.
                */

                int v_index = this->get_vertex_int(v);

                // Remove row from adjacency matrix.
                auto v_iter_row = adjacency_matrix.begin() + v_index;
                adjacency_matrix.erase(v_iter_row, v_iter_row + 1);

                // Remove column from adjacency matrix.
                for ( int i = 0; i < this->n_vertices; i++ ) {
                    auto v_iter_col = adjacency_matrix[i].begin() + v_index;
                    adjacency_matrix[i].erase(v_iter_col, v_iter_col + 1);
                }

                auto v_iter_vertices = std::find(this->vertices.begin(),
                    this->vertices.end(), v);
                this->vertices.erase(v_iter_vertices, v_iter_vertices + 1);
                this->vertex_indices.erase(v);
                this->n_vertices--;
            }

            void set_edge_weight(const Vertex& u, const Vertex& v,
                    EdgeWeight weight) {
                /*
                Adds an edge to the graph.
                */

                int u_index = this->get_vertex_int(u);
                int v_index = this->get_vertex_int(v);

                adjacency_matrix[u_index][v_index] = &weight;
                if ( !this->directed ) {
                    adjacency_matrix[v_index][u_index] = &weight;
                }
            }

            void remove_edge(const Vertex &u, const Vertex &v) {
                /*
                Removes an edge from the graph. A warning is raised if
                attempting to remove an edge that doesn't exist.
                */

                int u_index = this->get_vertex_int(u);
                int v_index = this->get_vertex_int(v);

                if ( adjacency_matrix[u_index][v_index] == nullptr ) {
                    std::cerr << "Attempting to remove edge that does not exist." << std::endl;
                } else {
                    adjacency_matrix[u_index][v_index] = nullptr;
                    if ( !this->directed ) {
                        adjacency_matrix[v_index][u_index] = nullptr;
                    }
                }
            }

            bool has_edge(const Vertex& u, const Vertex& v) {
                /*
                Returns whether or not a given edge is in the graph.
                If one or more of the vertices are not in the graph,
                false is returned.
                */
                try {
                    int u_index = this->get_vertex_int(u);
                    int v_index = this->get_vertex_int(v);
                    return adjacency_matrix[u_index][v_index] != nullptr;
                } catch ( std::invalid_argument e ) {
                    return false;
                }
            }

            bool has_vertex(const Vertex& v) {
                /*
                Returns whether or not a certain vertex is in the graph.
                */
                return std::find(this->vertices.begin(), this->vertices.end(),
                    v) != this->vertices.end();
            }

            std::set<Vertex> get_children(const Vertex& v) {
                /*
                Returns the children of a given vertex in the graph.
                In an undirected graph, this is equivalent to finding
                the "neighbors" of a vertex.
                */

                std::set<Vertex> children;
                int v_index = this->get_vertex_int(v);

                for ( int u_index = 0; u_index < this->n_vertices; u_index++ ) {
                    if ( u_index != v_index ) {
                        if ( adjacency_matrix[u_index][v_index] != nullptr ) {
                            children.insert(this->vertices[u_index]);
                        }
                    }
                }

                return children;               
            }

            std::set<Vertex> get_parents(const Vertex &v) {
                /*
                Returns the parents of a given vertex in the graph. This
                is equivalent to get_children in undirected graphs.
                */

                std::set<Vertex> parents;
                int v_index = this->get_vertex_int(v);

                for ( int u_index = 0; u_index < this->n_vertices; u_index++ ) {
                    if ( u_index != v_index ) {
                        if ( adjacency_matrix[v_index][u_index] != nullptr ) {
                            parents.insert(this->vertices[u_index]);
                        }
                    }
                }

                return parents;
            }
    };
}

#endif