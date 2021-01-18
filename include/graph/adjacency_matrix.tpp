/*
Template implementation file for adjacency matrix graph class.
*/


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


namespace cygraph {

    template<class Vertex, class EdgeWeight>
    int AdjacencyMatrixGraph<Vertex, EdgeWeight>::get_vertex_int(const Vertex& v) {
        if ( std::find(vertices.begin(), vertices.end(), v) != vertices.end() ) {
            return vertex_indices[v];
        } else {
            throw std::invalid_argument("Vertex not in graph.");
        }
    }

    template<class Vertex, class EdgeWeight>
    AdjacencyMatrixGraph<Vertex, EdgeWeight>::AdjacencyMatrixGraph(bool directed,
            const unordered_set<Vertex>& vertices) {

        this->directed = directed;
        int n_vertices = vertices.size();

        // Initialize adjacency matrix and vertex indices dictionary.
        adjacency_matrix.reserve(n_vertices);
        int i = 0;
        for ( Vertex v : vertices ) {
            vertex_indices[v] = i;
            adjacency_matrix.emplace_back(
                vector<EdgeWeight*>(n_vertices, nullptr));
            this->vertices.push_back(v);
            i++;
        }
    }

    template<class Vertex, class EdgeWeight>
    EdgeWeight AdjacencyMatrixGraph<Vertex, EdgeWeight>::get_edge_weight(const Vertex& u,
            const Vertex &v) override {

        int u_index = get_vertex_int(u);
        int v_index = get_vertex_int(v);
        EdgeWeight* weight_ptr = adjacency_matrix[u_index][v_index];
        if ( weight_ptr != nullptr ) {
            return *weight_ptr;
        } else {
            throw std::invalid_argument("Edge does not exist.");
        }
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyMatrixGraph<Vertex, EdgeWeight>::add_vertex(const Vertex& v) override {
        if ( std::find(vertices.begin(), vertices.end(), v) != vertices.end() ) {
            throw std::invalid_argument("Vertex is already in graph.");
        }

        int n_vertices = vertices.size();
        vertices.push_back(v);
        vertex_indices[v] = n_vertices;

        // Add new column to adjacency matrix.
        for ( vector<EdgeWeight*>& row : adjacency_matrix ) {
            row.push_back(nullptr);
        }
        // Add new row to adjacency matrix.
        adjacency_matrix.emplace_back(vector<EdgeWeight*>(n_vertices, nullptr));
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyMatrixGraph<Vertex, EdgeWeight>::add_vertices(
            const unordered_set<Vertex>& vertices) override {

        int n_new_vertices = vertices.size();

        // Check if vertices are already in graph.
        for ( Vertex v : vertices ) {
            if ( std::find(this->vertices.begin(), this->vertices.end(), v)
                    != this->vertices.end() ) {
                throw std::invalid_argument("Vertex is already in graph.");
            }
        }

        // Update vertex indices map.
        int n_vertices = this->vertices.size();
        for ( Vertex v : vertices ) {
            this->vertices.push_back(v);
            vertex_indices[v] = n_vertices;
            n_vertices++;
        }

        // Add new columns to adjacency matrix.
        EdgeWeight* new_edge_weights[n_new_vertices] = { nullptr };
        for ( vector<EdgeWeight*>& row : adjacency_matrix ) {
            row.insert(row.end(), new_edge_weights,
                new_edge_weights + n_new_vertices);
        }
        // Add new rows to adjacency matrix.
        vector<EdgeWeight*> new_row(n_vertices, nullptr);
        for ( int i = 0; i < n_new_vertices; i++ ) {
            adjacency_matrix.push_back(new_row);
        }
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyMatrixGraph<Vertex, EdgeWeight>::remove_vertex(const Vertex& v) override {
        int v_index = get_vertex_int(v);

        // Remove weights from edge_weights.
        int neighbor_index;
        if ( this->directed ) {
            for ( const Vertex& child : get_children(v) ) {
                neighbor_index = get_vertex_int(child);
                edge_weights.erase(pair<int, int>(v_index, neighbor_index));
            }
            for ( const Vertex& parent : get_parents(v) ) {
                neighbor_index = get_vertex_int(parent);
                edge_weights.erase(pair<int, int>(neighbor_index, v_index));
            }
        } else {
            for ( const Vertex& child : get_children(v) ) {
                neighbor_index = get_vertex_int(child);
                edge_weights.erase(pair<int, int>(v_index, neighbor_index));
                edge_weights.erase(pair<int, int>(neighbor_index, v_index));
            }
        }

        // Remove row from adjacency matrix.
        adjacency_matrix.erase(adjacency_matrix.begin() + v_index);

        // Remove column from adjacency matrix.
        for ( int i = 0; i < vertices.size() - 1; i++ ) {
            adjacency_matrix[i].erase(adjacency_matrix[i].begin() + v_index);
        }

        // Remove from vertices and vertex_indices.
        vertices.erase(vertices.begin() + v_index);
        vertex_indices.erase(v);
        // Change vertex indices to account for loss.
        for ( int i = v_index; i < vertices.size(); i++ ) {
            vertex_indices[vertices[i]] = i;
        }
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyMatrixGraph<Vertex, EdgeWeight>::set_edge_weight(const Vertex& u, const Vertex& v, 
            const EdgeWeight& weight) override {

        int u_index = get_vertex_int(u);
        int v_index = get_vertex_int(v);

        pair<int, int> edge = pair<int, int>(u_index, v_index);
        edge_weights[edge] = weight;

        adjacency_matrix[u_index][v_index] = &edge_weights[edge];
        if ( !this->directed ) {
            edge = pair<int, int>(v_index, u_index);
            edge_weights[edge] = weight;
            adjacency_matrix[v_index][u_index] = &edge_weights[edge];
        }
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyMatrixGraph<Vertex, EdgeWeight>::remove_edge(const Vertex& u, const Vertex& v) 
            override {

        int u_index = get_vertex_int(u);
        int v_index = get_vertex_int(v);

        if ( adjacency_matrix[u_index][v_index] == nullptr ) {
            throw std::invalid_argument("Edge does not exist.");
        } else {
            edge_weights.erase(pair<int, int>(u_index, v_index));

            adjacency_matrix[u_index][v_index] = nullptr;
            if ( !this->directed ) {
                edge_weights.erase(pair<int, int>(v_index, u_index));
                adjacency_matrix[v_index][u_index] = nullptr;
            }
        }
    }

    template<class Vertex, class EdgeWeight>
    bool AdjacencyMatrixGraph<Vertex, EdgeWeight>::has_edge(const Vertex& u, const Vertex& v) 
            override {

        int u_index, v_index;
        try {
            u_index = get_vertex_int(u);
            v_index = get_vertex_int(v);
        } catch ( std::invalid_argument ) {
            return false;
        }
        EdgeWeight* weight_ptr = adjacency_matrix[u_index][v_index];
        return weight_ptr != nullptr;
    }

    template<class Vertex, class EdgeWeight>
    bool AdjacencyMatrixGraph<Vertex, EdgeWeight>::has_vertex(const Vertex& v) override {
        return std::find(vertices.begin(), vertices.end(), v) != vertices.end();
    }

    template<class Vertex, class EdgeWeight>
    unordered_set<Vertex> AdjacencyMatrixGraph<Vertex, EdgeWeight>::get_children(const Vertex& v) 
            override {

        unordered_set<Vertex> children;
        int v_index = get_vertex_int(v);

        for ( int u_index = 0; u_index < this->vertices.size(); u_index++ ) {
            if ( adjacency_matrix[v_index][u_index] != nullptr ) {
                children.insert(vertices[u_index]);
            }
        }

        return children;
    }

    template<class Vertex, class EdgeWeight>
    unordered_set<Vertex> AdjacencyMatrixGraph<Vertex, EdgeWeight>::get_parents(const Vertex& v)
            override {

        unordered_set<Vertex> parents;
        int v_index = get_vertex_int(v);

        for ( int u_index = 0; u_index < this->vertices.size(); u_index++ ) {
            if ( adjacency_matrix[u_index][v_index] != nullptr ) {
                parents.insert(vertices[u_index]);
            }
        }

        return parents;
    }
}