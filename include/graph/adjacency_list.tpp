/* ============================================================================================== *
 * adjacency_list.tpp                                                                             *
 * authors: lol-cubes                                                                             *
 *                                                                                                *
 * Template implementation file for adjacency list graph classes.                                 *
 * ============================================================================================== */


#include <algorithm>
#include <iostream>
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

    // AdjacencyListGraphCommon<Vertex, EdgeWeight>

    template<class Vertex, class EdgeWeight>
    void AdjacencyListGraphCommon<Vertex, EdgeWeight>::add_vertex(const Vertex& v) {
        if ( has_vertex(v) )
            throw std::invalid_argument("Vertex is already in graph.");
        vertices.insert(v);
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyListGraphCommon<Vertex, EdgeWeight>::add_vertices
            (const unordered_set<Vertex>& vertices) {
        // Implemented using low-level functions again to avoid unecessary function call to 
        // add_vertex.
        for ( Vertex v : vertices ) {
            if ( has_vertex(v) )
                throw std::invalid_argument("Vertex is already in graph.");
        }
        this->vertices.insert(vertices.begin(), vertices.end());
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyListGraphCommon<Vertex, EdgeWeight>::remove_vertex(const Vertex& v) {
        if ( has_vertex(v) )
            vertices.erase(v);
        else
            throw std::invalid_argument("Vertex not in graph.");
    }

    template<class Vertex, class EdgeWeight>
    bool AdjacencyListGraphCommon<Vertex, EdgeWeight>::has_vertex(const Vertex& v) {
        return std::find(vertices.begin(), vertices.end(), v) != vertices.end();
    }


    // AdjacencyListGraph<Vertex, EdgeWeight>


    template<class Vertex, class EdgeWeight>
    AdjacencyListGraph<Vertex, EdgeWeight>::AdjacencyListGraph(bool directed, const unordered_set<Vertex>& vertices) {
        this->directed = directed;
        this->vertices = vertices;

        int n_vertices = vertices.size();

        // Initialize adjacency list.
        for ( Vertex v : vertices ) {
            adjacency_list[v] = vector<pair<Vertex, EdgeWeight>>();
        }
    }

    template<class Vertex, class EdgeWeight>
    EdgeWeight AdjacencyListGraph<Vertex, EdgeWeight>::get_edge_weight(const Vertex& u,
            const Vertex& v) {

        for ( std::pair<Vertex, EdgeWeight> child : adjacency_list[u] ) {
            if ( child.first == v ) {
                return child.second;
            }
        }
        throw std::invalid_argument("Edge does not exist.");
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyListGraph<Vertex, EdgeWeight>::add_vertex(const Vertex& v) {

        AdjacencyListGraphCommon<Vertex, EdgeWeight>::add_vertex(v);
        // Add new list to adjacency list.
        adjacency_list[v] = vector<pair<Vertex, EdgeWeight>>();
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyListGraph<Vertex, EdgeWeight>::add_vertices(
            const unordered_set<Vertex>& vertices) {

        AdjacencyListGraphCommon<Vertex, EdgeWeight>::add_vertices(vertices);
        // Add new lists to adjacency list.
        for ( Vertex v : vertices ) {
            adjacency_list[v] = vector<pair<Vertex, EdgeWeight>>();
        }
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyListGraph<Vertex, EdgeWeight>::remove_vertex(const Vertex& v) {
        
        AdjacencyListGraphCommon<Vertex, EdgeWeight>::remove_vertex(v);

        // Remove neighbor set from adjacency list.
        adjacency_list.erase(v);

        // Remove from each neighbor list of other vertices.
        vector<pair<Vertex, EdgeWeight>> children;
        int to_remove = 0;
        for ( auto& it : adjacency_list ) {
            children = it.second;
            for ( pair<Vertex, EdgeWeight> child : children ) {
                if ( child.first == v ) break;
                to_remove++;
            }
            if ( to_remove < children.size() ) {
                children.erase(children.begin() + to_remove);
            }
            to_remove = 0;
        }
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyListGraph<Vertex, EdgeWeight>::set_edge_weight(const Vertex& u,
            const Vertex& v, const EdgeWeight& weight) {

        if ( !this->has_vertex(u) || !this->has_vertex(v) ) {
            throw std::invalid_argument("Vertex not in graph.");
        }

        int to_remove = 0;
        for ( pair<Vertex, EdgeWeight> child : adjacency_list[u] ) {
            if ( child.first == v ) break;
            to_remove++;
        }
        if ( to_remove < adjacency_list[u].size() ) {
            // (u, v) already exists.
            adjacency_list[u].erase(adjacency_list[u].begin() + to_remove);
        }
        pair<Vertex, EdgeWeight> neighbor_pair = {v, weight};
        adjacency_list[u].push_back(neighbor_pair);

        if ( !this->directed ) {
            to_remove = 0;
            for ( pair<Vertex, EdgeWeight> child : adjacency_list[v] ) {
                if ( child.first == u ) break;
                to_remove++;
            }
            if ( to_remove < adjacency_list[v].size() ) {
                // (v, u) already exists.
                adjacency_list[v].erase(adjacency_list[v].begin() + to_remove);
            }
            adjacency_list[v].push_back(pair<Vertex, EdgeWeight>(u, weight));
        }
    }

    template<class Vertex, class EdgeWeight>
    void AdjacencyListGraph<Vertex, EdgeWeight>::remove_edge(const Vertex& u,
            const Vertex& v) {

        if ( !this->has_vertex(u) || !this->has_vertex(v) )
            throw std::invalid_argument("Vertex not in graph.");

        int to_remove = 0;
        for ( pair<Vertex, EdgeWeight> child : adjacency_list[u] ) {
            if ( child.first == v ) break;
            to_remove++;
        }
        if ( to_remove < adjacency_list[u].size() ) {
            adjacency_list[u].erase(adjacency_list[u].begin() + to_remove);
        } else {
            throw std::invalid_argument("Attempting to remove edge that doesn't exist.");
            return;
        }

        if ( !this->directed ) {
            to_remove = 0;
            for ( pair<Vertex, EdgeWeight> child : adjacency_list[v] ) {
                if ( child.first == u ) break;
                to_remove++;
            }
            adjacency_list[v].erase(adjacency_list[v].begin() + to_remove);
        }
    }

    template<class Vertex, class EdgeWeight>
    bool AdjacencyListGraph<Vertex, EdgeWeight>::has_edge(const Vertex &u, const Vertex& v) {
        if ( !this->has_vertex(u) || !this->has_vertex(v) ) {
            std::cerr << "has_edge called with one or more vertices not in graph. false returned." << std::endl;
            return false;
        }
        for ( pair<Vertex, EdgeWeight> child : adjacency_list[u] )
            if ( child.first == v ) return true;
        return false;
    }

    template<class Vertex, class EdgeWeight>
    unordered_set<Vertex> AdjacencyListGraph<Vertex, EdgeWeight>::get_children(const Vertex& v) {
        if ( !this->has_vertex(v) ) throw std::invalid_argument("Vertex not in graph.");

        unordered_set<Vertex> children;
        for ( pair<Vertex, EdgeWeight> child : adjacency_list[v] )
            children.insert(child.first);
        return children;
    }

    template<class Vertex, class EdgeWeight>
    unordered_set<Vertex> AdjacencyListGraph<Vertex, EdgeWeight>::get_parents(const Vertex& v) {
        if ( !this->has_vertex(v) ) {
            throw std::invalid_argument("Vertex not in graph.");
        }

        if ( this->directed ) {
            // Search for vertices who have v as a child.
            unordered_set<Vertex> parents;
            for ( auto& it : adjacency_list ) {
                if ( it.first == v ) continue;
                for ( pair<Vertex, EdgeWeight> child : it.second ) {
                    if ( child.first == v ) {
                        parents.insert(it.first);
                        break;
                    }
                }
            }
            return parents;
        } else {
            // Children and parents are equivalent in undirected graphs.
            return get_children(v);
        }
    }

    // AdjacencyListGraph<Vertex, bool>

    template<class Vertex>
    AdjacencyListGraph<Vertex, bool>::AdjacencyListGraph(bool directed,
            const unordered_set<Vertex>& vertices) {

        this->directed = directed;
        this->vertices = vertices;

        int n_vertices = vertices.size();

        // Initialize adjacency list.
        for ( Vertex v : vertices ) adjacency_list[v] = unordered_set<Vertex>();
    }

    template<class Vertex>
    bool AdjacencyListGraph<Vertex, bool>::get_edge_weight(const Vertex& u, const Vertex& v) {
        if ( !this->has_vertex(u) || !this->has_vertex(v) )
            throw std::invalid_argument("Vertex not in graph.");
        return has_edge(u, v);
    }

    template<class Vertex>
    void AdjacencyListGraph<Vertex, bool>::add_vertex(const Vertex& v) {
        AdjacencyListGraphCommon<Vertex, bool>::add_vertex(v);
        // Add new list to adjacency list.
        adjacency_list[v] = unordered_set<Vertex>();
    }

    template<class Vertex>
    void AdjacencyListGraph<Vertex, bool>::add_vertices(const unordered_set<Vertex>& vertices) {
        AdjacencyListGraphCommon<Vertex, bool>::add_vertices(vertices);
        // Add new lists to adjacency list.
        for ( Vertex v : vertices ) adjacency_list[v] = unordered_set<Vertex>();
    }

    template<class Vertex>
    void AdjacencyListGraph<Vertex, bool>::remove_vertex(const Vertex& v) {
        AdjacencyListGraphCommon<Vertex, bool>::remove_vertex(v);

        // Remove neighbor set from adjacency list.
        adjacency_list.erase(v);

        // Remove from each neighbor list of other vertices.
        unordered_set<Vertex> children;
        for ( auto& it : adjacency_list ) it.second.erase(v);
    }

    template<class Vertex>
    void AdjacencyListGraph<Vertex, bool>::set_edge_weight(const Vertex& u, const Vertex& v,
            const bool& weight) {

        if ( !this->has_vertex(u) || !this->has_vertex(v) )
            throw std::invalid_argument("Vertex not in graph.");

        if ( weight ) {
            adjacency_list[u].insert(v);
            if ( !this->directed ) adjacency_list[v].insert(u);
        } else {
            try {
                remove_edge(u, v);
            } catch ( std::invalid_argument e ) {}
        }
    }

    template<class Vertex>
    void AdjacencyListGraph<Vertex, bool>::add_edge(const Vertex& u, const Vertex& v) {
        if ( has_edge(u, v) )
            throw std::invalid_argument("Edge already exists.");
        if ( !this->has_vertex(u) || !this->has_vertex(v) )
            throw std::invalid_argument("Vertex not in graph.");
        adjacency_list[u].insert(v);
        if ( !this->directed ) adjacency_list[v].insert(u);
    }

    template<class Vertex>
    void AdjacencyListGraph<Vertex, bool>::add_edges(const vector<pair<Vertex, Vertex>>& edges) {
        vector<pair<Vertex, Vertex>> added_edges;
        try {
            for ( const pair<Vertex, Vertex>& edge : edges ) {
                add_edge(edge.first, edge.second);
                added_edges.push_back(edge);
            }
        } catch ( std::invalid_argument e ) {
            for ( const pair<Vertex, Vertex>& edge : added_edges )
                remove_edge(edge.first, edge.second);
            throw e;
        }
    }

    template<class Vertex>
    void AdjacencyListGraph<Vertex, bool>::remove_edge(const Vertex& u, const Vertex& v) {
        if ( !has_edge(u, v) ) {
            throw std::invalid_argument("Attempting to remove edge that doesn't exist.");
        }

        adjacency_list[u].erase(v);
        if ( !this->directed ) adjacency_list[v].erase(u);
    }

    template<class Vertex>
    bool AdjacencyListGraph<Vertex, bool>::has_edge(const Vertex& u, const Vertex& v) {
        if ( !this->has_vertex(u) || !this->has_vertex(v) ) {
            std::cerr << "has_edge called with one or more vertices not in graph. false returned." << std::endl;
            return false;
        }
        return std::find(adjacency_list[u].begin(), adjacency_list[u].end(), v)
                != adjacency_list[u].end();
    }

    template<class Vertex>
    unordered_set<Vertex> AdjacencyListGraph<Vertex, bool>::get_children(const Vertex& v) {
        if ( !this->has_vertex(v) )
            throw std::invalid_argument("Vertex not in graph.");
        return adjacency_list[v];
    }

    template<class Vertex>
    unordered_set<Vertex> AdjacencyListGraph<Vertex, bool>::get_parents(const Vertex& v) {
        if ( !this->has_vertex(v) )
            throw std::invalid_argument("Vertex not in graph.");
        unordered_set<Vertex> parents;
        for ( auto& it : adjacency_list ) {
            for ( Vertex child : it.second ) {
                if ( child == v ) {
                    parents.insert(it.first);
                    break;
                }
            }
        }
        return parents;
    }
}