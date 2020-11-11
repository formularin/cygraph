/*
A general graph class. Meant to be inherited. No real graph theory
functionality.
*/

#ifndef CYGRAPH_GRAPH_HPP
#define CYGRAPH_GRAPH_HPP

#include <iostream>

#include <algorithm>
#include <array>
#include <unordered_set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


using std::vector;


namespace cygraph {

    template<class Vertex, class EdgeWeight>
    class Graph {
        /*
        A generic Graph class. Not to be instantiated.
        */
        protected:
            vector<Vertex> vertices;
            std::unordered_map<Vertex, int> vertex_indices;
            bool directed;
            int n_vertices;

            int get_vertex_int(const Vertex& v) {
                if ( std::find(vertices.begin(), vertices.end(), v)
                     != vertices.end() ) {
                    return vertex_indices[v];
                } else {
                    throw std::invalid_argument("Vertex not in graph.");
                }
            }

        public:
            bool get_directed() {
                /*
                Returns whether or not the graph is directed.
                */
                return directed;
            }

            vector<Vertex> get_vertices() {
                /*
                Returns a vector containing the vertices in the graph.
                */
                return vertices;
            }

            virtual EdgeWeight get_edge_weight(const Vertex& u, const Vertex& v) = 0;
            /*
            Returns the weight of an edge.
            */

            virtual void add_vertex(const Vertex& v) = 0;
            /*
            Adds a vertex to the graph.
            */

            virtual void add_vertices(Vertex vertices[], int n_vertices) = 0;
            /*
            Adds an array of vertices to the graph.
            */

            virtual void remove_vertex(const Vertex& v) = 0;
            /*
            Removes a vertex from the graph.
            */

            virtual void remove_vertices(Vertex vertices[], int n_vertices) = 0;
            /*
            Removes an array of vertices from the graph.
            */

            virtual void set_edge_weight(const Vertex& u, const Vertex& v,
                EdgeWeight weight) = 0;
            /*
            Changes the weight of an edge.
            */

            void set_edge_weights(Vertex edges[][2], EdgeWeight weights[],
                    int n_edges) {
                /*
                Sets the weights of an array of edges in the graph.
                */
                vector<std::array<Vertex, 2>> set_edges;
                std::array<Vertex, 2> edge;
                for ( int i = 0; i < n_edges; i++ ) {
                    try {
                        set_edge_weight(edges[i][0], edges[i][1], weights[i]);
                        std::copy(std::begin(edges[i]), std::end(edges[i]), edge.begin());
                        set_edges.push_back(edge);
                    } catch ( std::invalid_argument e ) {
                        for ( std::array<Vertex, 2> removal_edge : set_edges ) {
                            remove_edge(removal_edge[0], removal_edge[1]);
                        }
                        throw e;
                    }
                }
            }

            virtual void remove_edge(const Vertex& u, const Vertex& v) = 0;
            /*
            Removes an edge from the graph. A warning is raised if
            attempting to remove an edge that doesn't exist.
            */

            void remove_edges(Vertex edges[][2], int n_edges) {
                /*
                Removes an array of edges from the graph. A warning is
                raised if attempting to remove an edge that doesn't exist.
                */
                vector<std::array<Vertex, 2>> removed_edges;
                std::array<Vertex, 2> edge;
                vector<EdgeWeight> removed_edge_weights;
                EdgeWeight edge_weight;
                for ( int i = 0; i < n_edges; i++ ) {
                    try {
                        edge_weight = get_edge_weight(edges[i][0], edges[i][1]);
                        remove_edge(edges[i][0], edges[i][1]);
                        std::copy(std::begin(edges[i]), std::end(edges[i]), edge.begin());
                        removed_edges.push_back(edge);
                        removed_edge_weights.push_back(edge_weight);
                    } catch ( std::invalid_argument e ) {
                        for ( int i = 0; i < removed_edges.size(); i++ ) {
                            set_edge_weight(removed_edges[i][0],
                                removed_edges[i][1], removed_edge_weights[i]);
                        }
                        throw e;
                    }
                }
            }

            virtual bool has_edge(const Vertex& u, const Vertex& v) = 0;
            /*
            Returns whether or not a given edge is in the graph.
            If one or more of the vertices are not in the graph,
            false is returned.
            */

            virtual bool has_vertex(const Vertex& v) = 0;
            /*
            Returns whether or not a certain vertex is in the graph.
            */

            virtual std::unordered_set<Vertex> get_children(const Vertex& v) = 0;
            /*
            Returns the children of a given vertex in the graph.
            */

            virtual std::unordered_set<Vertex> get_parents(const Vertex& v) = 0;
            /*
            Returns the parents of a given vertex in the graph. This is
            equivalent to get_children in undirected graphs.
            */
    };
}


#endif