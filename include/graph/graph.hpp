/*
A general graph class. Meant to be inherited. No real graph theory
functionality.
*/

#ifndef CYGRAPH_GRAPH_HPP_
#define CYGRAPH_GRAPH_HPP_

#include <algorithm>
#include <any>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


namespace cygraph {

    template<class Vertex, class EdgeWeight>
    class Graph {
        /*
        A generic Graph class. Not to be instantiated.
        */
        protected:
            std::vector<Vertex> vertices;
            std::unordered_map<Vertex, int> vertex_indices;
            bool directed;
            int n_vertices;

            int get_vertex_int(Vertex& v) {
                if ( std::find(vertices.begin(), vertices.end(), v)
                     != vertices.begin() ) {
                    return vertex_indices[v];
                } else {
                    throw std::invalid_argument("Vertex not in graph.")
                }
            }

        public:
            bool get_directed() {
                /*
                Returns whether or not the graph is directed.
                */
                return directed;
            }

            std::vector<Vertex> get_vertices() {
                /*
                Returns a vector containing the vertices in the graph.
                */
                return vertices;
            }

            EdgeWeight get_edge_weight(Vertex& u, Vertex& v);
            /*
            Returns the weight of an edge.
            */

            void add_vertex(Vertex& v);
            /*
            Adds a vertex to the graph.
            */

            void add_vertices(Vertex vertices[], int n_vertices);
            /*
            Adds an array of vertices to the graph.
            */

            void remove_vertex(Vertex& v);
            /*
            Removes a vertex from the graph.
            */

            void set_edge_weight(Vertex& u, Vertex& v, EdgeWeight weight);
            /*
            Changes the weight of an edge.
            */

            void remove_edge(Vertex& u, Vertex& v);
            /*
            Removes an edge from the graph. A warning is raised if
            attempting to remove an edge that doesn't exist.
            */

            void set_edge_weights(Vertex edges[][2], EdgeWeight weights[],
                    int n_edges) {
                /*
                Sets the weights of an array of edges in the graph.
                */
                std::set<Vertex[2]> set_edges;
                Vertex edge[2];
                for ( edge : edges ) {
                    try {
                        set_edge_weight(edge[0], edge[1]);
                        set_edges.insert(edge);
                    } catch ( std::invalid_argument e ) {
                        for ( edge : set_edges ) {
                            remove_edge(edge[0], edge[1]);
                            throw e;
                        }
                    }
                }
            }

            bool has_edge(Vertex& u, Vertex& v);
            /*
            Returns whether or not a given edge is in the graph.
            If one or more of the vertices are not in the graph,
            false is returned.
            */

            bool has_vertex(Vertex& v);
            /*
            Returns whether or not a certain vertex is in the graph.
            */

            std::set<Vertex> get_children(Vertex& v);
            /*
            Returns the children of a given vertex in the graph.
            */

            std::set<Vertex> get_parents(Vertex& v);
            /*
            Returns the parents of a given vertex in the graph. This is
            equivalent to get_children in undirected graphs.
            */
    };
}


#endif