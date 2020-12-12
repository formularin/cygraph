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

    template <class Vertex, class EdgeWeight>
    class AdjacencyListGraph: public cygraph::Graph<Vertex, EdgeWeight> {
        /*
        A base adjacency list graph class with minimal functionality. Mainly exists to contain bits
        of code that would be repeated in both of the child adjacency list graph classes.
        */

        protected:

        unordered_set<Vertex> vertices;

        public:

        unordered_set<Vertex> get_vertices() {
            /*
            Returns the vertices in the graph.
            */
            return vertices;
        }

        void add_vertex(const Vertex& v) override {
            /*
            Checks if the vertex is already in the graph and adds if not.
            */
            if ( has_vertex(v) ) {
                throw std::invalid_argument("Vertex is already in graph.");
            }
            vertices.insert(v);
        }

        void add_vertices(const unordered_set<Vertex>& vertices) override {
            /*
            Checks if any of the vertices are already in the graph and adds them all if not.
            */
            for ( Vertex v : vertices ) {
                if ( has_vertex(v) ) {
                    throw std::invalid_argument("Vertex is already in graph.");
                }
            }
            this->vertices.insert(vertices.begin(), vertices.end());
        }

        void remove_vertex(const Vertex& v) override {
            /*
            Checks if vertex is already in graph and removes it if it is.
            */
            if ( has_vertex(v) ) {
                vertices.erase(v);
            } else {
                throw std::invalid_argument("Vertex not in graph.");
            }
        }

        bool has_vertex(const Vertex& v) override {
            /*
            Returns whether or not a certain vertex is in the graph.
            */
            return std::find(vertices.begin(), vertices.end(), v) != vertices.end();
        }
    };

    template <class Vertex>
    class UnweightedAdjacencyListGraph: public AdjacencyListGraph<Vertex, bool> {
        /*
        A graph class implemented using an adjacency list, without edge weight functionality.
        Vertex type must have std::hash overriden.
        */

        protected:

        unordered_map<Vertex, unordered_set<Vertex>> adjacency_list;

        public:
        UnweightedAdjacencyListGraph() {
            /*
            Default constructor.
            */
        }

        UnweightedAdjacencyListGraph(bool directed, const unordered_set<Vertex>& vertices) {
            /*
            Class constructor.
            */

            this->directed = directed;
            this->vertices = vertices;

            int n_vertices = vertices.size();

            // Initialize adjacency list.
            for ( Vertex v : vertices ) {
                adjacency_list[v] = unordered_set<Vertex>();
            }
        }

        bool get_edge_weight(const Vertex& u, const Vertex& v) override {
            /*
            Returns whether or not an edge exists.
            */
            if ( !this->has_vertex(u) || !this->has_vertex(v) ) {
                throw std::invalid_argument("Vertex not in graph.");
            }
            return has_edge(u, v);
        }

        void add_vertex(const Vertex& v) override {
            /*
            Adds a vertex to the graph.
            */
            AdjacencyListGraph<Vertex, bool>::add_vertex(v);
            // Add new list to adjacency list.
            adjacency_list[v] = unordered_set<Vertex>();
        }

        void add_vertices(const unordered_set<Vertex>& vertices) override {
            /*
            Adds an array of vertices to the graph.
            */
            AdjacencyListGraph<Vertex, bool>::add_vertices(vertices);
            // Add new lists to adjacency list.
            for ( Vertex v : vertices ) {
                adjacency_list[v] = unordered_set<Vertex>();
            }
        }

        void remove_vertex(const Vertex& v) override {
            /*
            Removes a vertex from the graph.
            */
            AdjacencyListGraph<Vertex, bool>::remove_vertex(v);

            // Remove neighbor set from adjacency list.
            adjacency_list.erase(v);

            // Remove from each neighbor list of other vertices.
            unordered_set<Vertex> children;
            for ( auto& it : adjacency_list ) {
                it.second.erase(v);
            }
        }

        void set_edge_weight(const Vertex& u, const Vertex& v, bool weight) override {
            /*
            Adds or removes an edge based on its weight.
            */
            if ( weight ) {
                adjacency_list[u].insert(v);
                if ( !this->directed ) adjacency_list[v].insert(u);
            } else {
                try {
                    remove_edge(u, v);
                } catch ( std::invalid_argument e ) {}
            }
        }

        void add_edge(const Vertex& u, const Vertex& v) {
            /*
            Adds an edge between two vertices in the graph.
            */
            if ( has_edge(u, v) ) {
                throw std::invalid_argument("Edge already exists.");
            }
            if ( !this->has_vertex(u) || !this->has_vertex(v) ) {
                throw std::invalid_argument("Vertex not in graph.");
            }
            adjacency_list[u].insert(v);
            if ( !this->directed ) adjacency_list[v].insert(u);
        }

        void add_edges(const vector<pair<Vertex, Vertex>>& edges) {
            /*
            Adds multiple edges to the graph.
            */
            vector<pair<Vertex, Vertex>> added_edges;
            try {
                for ( const pair<Vertex, Vertex>& edge : edges ) {
                    add_edge(edge.first, edge.second);
                    added_edges.push_back(edge);
                }
            } catch ( std::invalid_argument e ) {
                for ( const pair<Vertex, Vertex>& edge : added_edges ) {
                    remove_edge(edge.first, edge.second);
                }
                throw e;
            }
        }

        void remove_edge(const Vertex& u, const Vertex& v) override {
            /*
            Removes an edge from the graph.
            */
            if ( !has_edge(u, v) ) {
                throw std::invalid_argument("Attempting to remove edge that doesn't exist.");
            }

            adjacency_list[u].erase(v);
            if ( !this->directed ) adjacency_list[v].erase(u);
        }

        void remove_edges(const vector<pair<Vertex, Vertex>>& edges) override {
            /*
            Removes multiple edges from the graph.
            */
            vector<pair<Vertex, Vertex>> removed_edges;
            try {
                for ( const pair<Vertex, Vertex>& edge : edges ) {
                    remove_edge(edge.first, edge.second);
                    removed_edges.push_back(edge);
                }
            } catch ( std::invalid_argument e ) {
                for ( const pair<Vertex, Vertex>& edge : removed_edges ) {
                    add_edge(edge.first, edge.second);
                }
                throw e;
            }
        }

        bool has_edge(const Vertex& u, const Vertex& v) override {
            /*
            Returns whether or not a given edge is in the graph. If one or more of the vertices are
            not in the graph, false is returned.
            */
            return std::find(adjacency_list[u].begin(), adjacency_list[u].end(), v)
                   != adjacency_list[u].end();
        }

        unordered_set<Vertex> get_children(const Vertex& v) override {
            /*
            Returns the children of a given vertex in the graph. In an undirected graph, this is
            equivalent to finding the "neighbors" of a vertex, and is the same as the method
            get_parents.
            */
            return adjacency_list[v];
        }

        unordered_set<Vertex> get_parents(const Vertex& v) override {
            /*
            Returns the parents of a given vertex in the graph. In an undirected graph, this is
            equivalent to finding the "neighbors" of a vertex, and is the same as the method
            get_children.
            */
            unordered_set<Vertex> parents;
            for ( auto& it : adjacency_list ) {
                if ( it.first == v ) continue;
                for ( Vertex child : it.second ) {
                    if ( child == v ) {
                        parents.insert(it.first);
                        break;
                    }
                }
            }
            return parents;
        }
    };

    template <class Vertex, class EdgeWeight>
    class WeightedAdjacencyListGraph: public AdjacencyListGraph<Vertex, EdgeWeight> {
        /*
        A graph class implemented using an adjacency list. Vertex type must have std::hash overriden.
        */

        protected:

        unordered_map<Vertex, vector<pair<Vertex, EdgeWeight>>> adjacency_list;

        public:

        WeightedAdjacencyListGraph() {
            /*
            Default constructor.
            */
        }

        WeightedAdjacencyListGraph(bool directed, const unordered_set<Vertex>& vertices) {
            /*
            Class constructor.
            */

            this->directed = directed;
            this->vertices = vertices;

            int n_vertices = vertices.size();

            // Initialize adjacency list.
            for ( Vertex v : vertices ) {
                adjacency_list[v] = vector<pair<Vertex, EdgeWeight>>();
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
            AdjacencyListGraph<Vertex, EdgeWeight>::add_vertex(v);
            // Add new list to adjacency list.
            adjacency_list[v] = vector<pair<Vertex, EdgeWeight>>();
        }

        void add_vertices(const unordered_set<Vertex>& vertices) override {
            /*
            Adds an array of vertices to the graph.
            */
            AdjacencyListGraph<Vertex, EdgeWeight>::add_vertices(vertices);
            // Add new lists to adjacency list.
            for ( Vertex v : vertices ) {
                adjacency_list[v] = vector<pair<Vertex, EdgeWeight>>();
            }
        }

        void remove_vertex(const Vertex& v) override {
            /*
            Removes a vertex from the graph.
            */
            AdjacencyListGraph<Vertex, EdgeWeight>::remove_vertex(v);

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
            }
        }

        void set_edge_weight(const Vertex& u, const Vertex& v, EdgeWeight weight) override {
            /*
            Sets the weight of an edge.
            */
            if ( !this->has_vertex(u) || !this->has_vertex(v) ) {
                throw std::invalid_argument("Vertex not in graph.");
            }

            int to_remove = 0;
            for( pair<Vertex, EdgeWeight> child : adjacency_list[u] ) {
                if ( child.first == v ) break;
                to_remove++;
            }
            if ( to_remove < adjacency_list[v].size() ) {
                // (u, v) already exists.
                adjacency_list[u].erase(adjacency_list[u].begin() + to_remove);
            }
            adjacency_list[u].push_back(pair<Vertex, EdgeWeight>(v, weight));

            if ( this->directed ) {
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

        void remove_edge(const Vertex& u, const Vertex& v) override {
            /*
            Removes an edge from the graph. A warning is raised if attempting to remove an edge
            that doesn't exist.
            */
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

        bool has_edge(const Vertex &u, const Vertex& v) override {
            /*
            Returns whether or not a given edge is in the graph. If one or more of the vertices are
            not in the graph, false is returned.
            */
            if ( !this->has_vertex(u) || !this->has_vertex(v) ) {
                return false;
            }
            for ( pair<Vertex, EdgeWeight> child : adjacency_list[u] ) {
                if ( child.first == v ) {
                    return true;
                }
            }
            return false;
        }

        unordered_set<Vertex> get_children(const Vertex& v) override {
            /*
            Returns the children of a given vertex in the graph. In an undirected graph, this is
            equivalent to finding the "neighbors" of a vertex, and is the same as the method
            get_parents.
            */

            unordered_set<Vertex> children;
            for ( pair<Vertex, EdgeWeight> child : adjacency_list[v] ) {
                children.insert(child.first);
            }
            return children;
        }

        unordered_set<Vertex> get_parents(const Vertex& v) override {
            /*
            Returns the parents of a given vertex in the graph. In an undirected graph, this is
            equivalent to finding the "neighbors" of a vertex, and is the same as the method
            get_children.
            */

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
        }
    };
}

#endif