/*
A general graph class. Meant to be inherited. No real graph theory
functionality.
*/

#ifndef CYGRAPH_GRAPH_HPP
#define CYGRAPH_GRAPH_HPP

#include <algorithm>
#include <unordered_set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::vector;


namespace cygraph {

    template<class Vertex, class EdgeWeight>
    class Graph {
        /*
        A generic Graph class. Not to be instantiated.
        */
        protected:
            bool directed;

        public:
            bool get_directed() {
                /*
                Returns whether or not the graph is directed.
                */
                return directed;
            }

            virtual EdgeWeight get_edge_weight(const Vertex& u, const Vertex& v) = 0;
            /*
            Returns the weight of an edge.
            */

            virtual void add_vertex(const Vertex& v) = 0;
            /*
            Adds a vertex to the graph.
            */

            virtual void add_vertices(unordered_set<Vertex> vertices) = 0;
            /*
            Adds an array of vertices to the graph.
            */

            virtual void remove_vertex(const Vertex& v) = 0;
            /*
            Removes a vertex from the graph.
            */

            virtual void remove_vertices(unordered_set<Vertex> vertices) {
                /*
                Removes an array of vertices from the graph.
                */
                for ( int i = 0; i < n_vertices; i++ ) {
                    remove_vertex(vertices[i]);
                }
            }

            virtual void set_edge_weight(const Vertex& u, const Vertex& v, EdgeWeight weight) = 0;
            /*
            Changes the weight of an edge.
            */

            void set_edge_weights(unordered_set<pair<pair<Vertex, Vertex>, EdgeWeight>> edges) {
                /*
                Sets the weights of an array of edges in the graph.
                */
                unordered_set<pair<Vertex, Vertex>> set_edges;
                pair<Vertex, Vertex> edge_vertices;
                EdgeWeight weight;
                for ( pair<pair<Vertex, Vertex>, EdgeWeight> edge : edges ) {
                    edge_vertices = pair.first;
                    weight = pair.second;
                    try {
                        set_edge_weight(edge_vertices.first, edge_vertices.second, weight);
                        set_edges.insert(edge_vertices);
                    } catch ( std::invalid_argument e ) {
                        for ( pair<Vertex, Vertex> removal_edge : set_edges ) {
                            remove_edge(removal_edge);
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

            void remove_edges(unordered_set<pair<Vertex, Vertex>> edges) {
                /*
                Removes an array of edges from the graph. A warning is
                raised if attempting to remove an edge that doesn't exist.
                */
                unordered_set<pair<pair<Vertex, Vertex>, EdgeWeight>> removed_edges;
                for ( pair<Vertex, Vertex> edge : edges ) {
                    try {
                        remove_edge(edge.first, edge.second, weight);
                    } catch ( std::invalid_argument e ) {
                        for ( pair<pair<Vertex, Vertex>, Edge> readdition_edge : removed_edges ) {
                            set_edge_weight(readdition_edge.first.first,
                                readdition_edge.first.second, readdition_edge.second);
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