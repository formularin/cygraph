/*
An implementation of the graph data structure using an adjacency matrix.
*/

#ifndef CYGRAPH_ADJACENCY_MATRIX_HPP
#define CYGRAPH_ADJACENCY_MATRIX_HPP

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "graph.hpp"


using std::vector;


// Overload std::hash for std::pair<int, int>
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

    template <class Vertex, class EdgeWeight>
    class AdjacencyMatrixGraph: public cygraph::Graph<Vertex, EdgeWeight> {

        protected:
            vector<vector<EdgeWeight*>> adjacency_matrix;
            // Exists so that temporary memory addresses are not saved in adjacency matrix. All 
            // adjacency matrix values are pointing to this vector.
            std::unordered_map<std::pair<int, int>, EdgeWeight> edge_weights;
            std::unordered_map<Vertex, int> vertex_indices;

            int get_vertex_int(const Vertex& v) {
                if ( std::find(vertices.begin(), vertices.end(), v)
                     != vertices.end() ) {
                    return vertex_indices[v];
                } else {
                    throw std::invalid_argument("Vertex not in graph.");
                }
            }

        public:
            AdjacencyMatrixGraph() {
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

                // Initialize adjacency matrix and vertex indices
                // dictionary.
                adjacency_matrix.reserve(this->n_vertices);
                for ( int i = 0; i < this->n_vertices; i++ ) {
                    vertex_indices[this->vertices[i]] = i;
                    adjacency_matrix.emplace_back(
                        vector<EdgeWeight*>(this->n_vertices, nullptr));
                }
            }

            EdgeWeight get_edge_weight(const Vertex& u, const Vertex &v) override {
                /*
                Returns the weight of an edge.
                */

                int u_index = get_vertex_int(u);
                int v_index = get_vertex_int(v);
                EdgeWeight* weight_ptr = adjacency_matrix[u_index][v_index];
                if ( weight_ptr != nullptr ) {
                    return *weight_ptr;
                } else {
                    throw std::invalid_argument("Edge does not exist.");
                }
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
                vertex_indices[v] = this->n_vertices;
                this->n_vertices++;

                // Add new column to adjacency matrix.
                for ( vector<EdgeWeight*>& row : adjacency_matrix ) {
                    row.push_back(nullptr);
                }
                // Add new row to adjacency matrix.
                adjacency_matrix.emplace_back(
                    vector<EdgeWeight*>(this->n_vertices, nullptr));
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

                this->vertices.insert(this->vertices.end(), vertices,
                    vertices + n_vertices);
                // Update vertex indices map.
                for ( int i = 0; i < n_vertices; i++ ) {
                    vertex_indices[vertices[i]] = n_vertices + i;
                }
                this->n_vertices += n_vertices;

                // Add new columns to adjacency matrix.
                EdgeWeight* new_edge_weights[n_vertices] = { nullptr };
                for ( vector<EdgeWeight*>& row : adjacency_matrix ) {
                    row.insert(row.end(), new_edge_weights,
                        new_edge_weights + n_vertices);
                }
                // Add new rows to adjacency matrix.
                vector<EdgeWeight*> new_rows[n_vertices] = 
                    { vector<EdgeWeight*>(this->n_vertices, nullptr) };
                adjacency_matrix.insert(adjacency_matrix.end(), new_rows,
                    new_rows + n_vertices);
            }

            void remove_vertex(const Vertex& v) override {
                /*
                Removes a vertex from the graph.
                */

                int v_index = get_vertex_int(v);

                // Remove weights from edge_weights.
                int neighbor_index;
                if ( this->directed ) {
                    for ( const Vertex& child : get_children(v) ) {
                        neighbor_index = get_vertex_int(child);
                        edge_weights.erase(
                            std::pair<int, int>(v_index, neighbor_index));
                    }
                    for ( const Vertex& parent : get_parents(v) ) {
                        neighbor_index = get_vertex_int(parent);
                        edge_weights.erase(
                            std::pair<int, int>(neighbor_index, v_index));
                    }
                } else {
                    for ( const Vertex& child : get_children(v) ) {
                        neighbor_index = get_vertex_int(child);
                        edge_weights.erase(
                            std::pair<int, int>(v_index, neighbor_index));
                        edge_weights.erase(
                            std::pair<int, int>(neighbor_index, v_index));
                    }
                }

                // Remove row from adjacency matrix.
                auto v_iter_row = adjacency_matrix.begin() + v_index;
                adjacency_matrix.erase(v_iter_row, v_iter_row + 1);

                // Remove column from adjacency matrix.
                for ( int i = 0; i < this->n_vertices; i++ ) {
                    auto v_iter_col = adjacency_matrix[i].begin() + v_index;
                    adjacency_matrix[i].erase(v_iter_col, v_iter_col + 1);
                }

                // Remove from vertices and vertex_indices.
                auto v_iter_vertices = std::find(this->vertices.begin(),
                    this->vertices.end(), v);
                this->vertices.erase(v_iter_vertices, v_iter_vertices + 1);
                vertex_indices.erase(v);
                this->n_vertices--;
            }

            void set_edge_weight(const Vertex& u, const Vertex& v,
                    EdgeWeight weight) override {
                /*
                Changes the weight of an edge.
                */

                int u_index = get_vertex_int(u);
                int v_index = get_vertex_int(v);

                std::pair<int, int> edge =
                    std::pair<int, int>(u_index, v_index);
                edge_weights[edge] = weight;

                adjacency_matrix[u_index][v_index] = &edge_weights[edge];
                if ( !this->directed ) {
                    edge = std::pair<int, int>(v_index, u_index);
                    edge_weights[edge] = weight;
                    adjacency_matrix[v_index][u_index] = &edge_weights[edge];
                }
            }

            void remove_edge(const Vertex& u, const Vertex& v) override {
                /*
                Removes an edge from the graph. A warning is raised if
                attempting to remove an edge that doesn't exist.
                */

                int u_index = get_vertex_int(u);
                int v_index = get_vertex_int(v);

                if ( adjacency_matrix[u_index][v_index] == nullptr ) {
                    std::cerr << "Attempting to remove edge that does not exist." << std::endl;
                } else {
                    edge_weights.erase(std::pair<int, int>(u_index, v_index));

                    adjacency_matrix[u_index][v_index] = nullptr;
                    if ( !this->directed ) {
                        edge_weights.erase(
                            std::pair<int, int>(v_index, u_index));

                        adjacency_matrix[v_index][u_index] = nullptr;
                    }
                }
            }

            bool has_edge(const Vertex& u, const Vertex& v) override {
                /*
                Returns whether or not a given edge is in the graph.
                If one or more of the vertices are not in the graph,
                false is returned.
                */
                int u_index = get_vertex_int(u);
                int v_index = get_vertex_int(v);
                EdgeWeight* weight_ptr = adjacency_matrix[u_index][v_index];
                return weight_ptr != nullptr;
            }

            bool has_vertex(const Vertex& v) override {
                /*
                Returns whether or not a certain vertex is in the graph.
                */
                return std::find(this->vertices.begin(), this->vertices.end(),
                    v) != this->vertices.end();
            }

            std::unordered_set<Vertex> get_children(const Vertex& v) override {
                /*
                Returns the children of a given vertex in the graph.
                In an undirected graph, this is equivalent to finding
                the "neighbors" of a vertex.
                */

                std::unordered_set<Vertex> children;
                int v_index = get_vertex_int(v);

                for ( int u_index = 0; u_index < this->n_vertices; u_index++ ) {
                    if ( adjacency_matrix[v_index][u_index] != nullptr ) {
                        children.insert(this->vertices[u_index]);
                    }
                }

                return children;               
            }

            std::unordered_set<Vertex> get_parents(const Vertex& v) override {
                /*
                Returns the parents of a given vertex in the graph. This
                is equivalent to get_children in undirected graphs.
                */

                std::unordered_set<Vertex> parents;
                int v_index = get_vertex_int(v);

                for ( int u_index = 0; u_index < this->n_vertices; u_index++ ) {
                    if ( adjacency_matrix[u_index][v_index] != nullptr ) {
                        parents.insert(this->vertices[u_index]);
                    }
                }

                return parents;
            }
    };
}

#endif