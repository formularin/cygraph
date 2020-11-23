/*
An implementation of the graph data structure using an adjacency list.
*/

#ifndef CYGRAPH_ADJACENCY_LIST_HPP
#define CYGRAPH_ADJACENCY_LIST_HPP

#include <algorithm>
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
        // An empty base adjaceny list class.
    };

    template <class Vertex, class EdgeWeight>
    class WeightedAdjacencyListGraph: public cygraph::Graph<Vertex, EdgeWeight> {
        /*
        A graph class implemented useing an adjacency matrix. Vertex and EdgeWeigh types must have
        std::hash overriden for them.
        */

        protected:
            unordered_map<Vertex, unordered_set<pair<Vertex, EdgeWeight>>> adjacency_list;
            unordered_set<Vertex> vertices;

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
                for ( int i = 0; i < n_vertices; i++ ) {
                    adjacency_list[vertices[i]] = std::unordered_set();
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
            if ( std::find(vertices.begin(), vertices.end(), v) != vertices.end() ) {
                throw std::invalid_argument("Vertex is already in graph.");
            }
            vertices.insert(v);
            // Add new list to adjacency list.
            adjacency_list[v] = unordered_set<pair<Vertex, EdgeWeight>>();
        }

        void add_vertices(Vertex vertices[], int n_vertices) override {
            /*
            Adds an array of vertices to the graph.
            */

            // Check if vertices are already in graph.
            Vertex v;
            for ( int i = 0; i < n_vertices; i++ ) {
                v = vertices[i];
                if ( std::find(this->vertices.begin(), this->vertices.end(), v)
                        != this->vertices.end() ) {
                    throw std::invalid_argument("Vertex is already in graph.");
                }
            }
            this->vertices.insert(this->vertices.end(), vertices, vertices + n_vertices);

            // Add new lists to adjacency list.
            for ( int i = 0; i < n_vertices; i++ ) {
                adjacency_list[v] = unordered_set<pair<Vertex, EdgeWeight>>();
            }
        }

        void remove_vertex(const Vertex& v) override {
            /*
            Removes a vertex from the graph.
            */

            // Check that the vertex exists.
            if ( std::find(vertices.begin(), vertices.end(), v) == vertices.end() ) {
                throw std::invalid_argument("Vertex not in graph.");
            }

            // Remove neighbor set from adjacency list and vertex from vertices set.
            adjacency_list.erase(v);
            vertices.erase(v);

            // Remove from each neighbor list of other vertices.
            Vertex u;
            unordered_set<pair<Vertex, EdgeWeight>>& children;
            pair<Vertex, EdgeWeight> to_remove;
            bool remove = false;
            for ( auto& it : adjacency_list ) {
                u = it.first;
                children = it.second;
                for ( pair<Vertex, EdgeWeight> child : children ) {
                    if ( child.first == v ) {
                        to_remove = child;
                        remove = true;
                    }
                }
                if ( remove ) {
                    children.erase(to_remove);
                    remove = false;
                }
            }
        }

        void remove_vertices(const unordered_set<Vertex>& vertices) override {
            /*
            Removes a set of vertices from the graph.
            */

            // Check that all of the vertices exist.
            for ( Vertex v : vertices ) {
                if ( std::find(this->vertices.begin(), this->vertices.end(), v)
                        == this->vertices.end()) {
                    throw std::invalid_argument("Vertex not in graph.");
                }
            }

            // Remove the neighbor sets from the adjacency list and the vertices from the
            // vertices set.
            for ( Vertex v : vertices ) {
                this->vertices.erase(v);
                this->adjacency_list.erase(v);
            }

            // Remove from child lists in adjacency list.
            Vertex v;
            unordered_set<pair<Vertex, EdgeWeight>> children, removal_set;
            for ( auto& it : adjacency_list ) {
                v = it.first;
                children = it.second;
                removal_set = {};
                for ( pair<Vertex, EdgeWeight> child : children ) {
                    if ( std::find(vertices.begin(), vertices.end(), child.first)
                            != vertices.end() ) {
                        removal_set.insert(child);
                    }
                }
                for ( pair<Vertex, EdgeWeight> to_remove : removal_set ) {
                    it.second.erase(to_remove);
                }
            }
        }

        void set_edge_weight(const Vertex& u, const Vertex& v, EdgeWeight weight) override {
            /*
            Sets the weight of an edge.
            */

            adjacency_list[u].insert(pair<Vertex, EdgeWeight>(v, weight));
            if ( !this->directed ) {
                adjacency_list[v].insert(pair<Vertex, EdgeWeight>(u, weight));
            }
        }

        void remove_edge(const Vertex& u, const Vertex& v) override {
            /*
            Removes an edge from the graph. A warning is raised if attempting to remove an edge
            that doesn't exist.
            */

            bool edge_exists = std::find(adjacency_list[u].begin(), adjacency_list[u].end(), v)
                != adjacency_list[u].end();
            if ( edge_exists ) {
                std::cerr << "Attempting to remove edge that does not exist." << std::endl;
            } else {
                pair<Vertex, EdgeWeight> to_remove;
                for ( pair<Vertex, EdgeWeight> child : adjacency_list[u] ) {
                    if ( child.first == v ) {
                        to_remove = child;
                    }
                }
                adjacency_list[u].erase(to_remove);

                if ( !this->directed ) {
                    for ( pair<Vertex, EdgeWeight> child : adjacency_list[v] ) {
                        if ( child.first == u ) {
                            to_remove = child;
                        }
                    }
                    adjacency_list[v].erase(to_remove);
                }
            }
        }

        bool has_edge(const Vertex &u, const Vertex& v) override {
            /*
            Returns whether or not a given edge is in the graph. If one or more of the vertices are
            not in the graph, false is returned.
            */
            for ( pair<Vertex, EdgeWeight> child : adjacency_list[u] ) {
                if ( child.first == v ) {
                    return true;
                }
            }
            return false;
        }

        bool has_vertex(const Vertex& v) override {
            /*
            Returns whether or not a certain vertex is in the graph.
            */
            return std::find(vertices.begin(), vertices.end(), v) != vertices.end();
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