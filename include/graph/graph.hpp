/*
A general graph class. Meant to be inherited. No real graph theory
functionality.
*/

#ifndef CYGRAPH_GRAPH_H
#define CYGRAPH_GRAPH_H

#include <set>
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

            void add_vertices(Vertex vertices[]);
            /*
            Adds an array of vertices to the graph.
            */

            void remove_vertex(Vertex& v);
            /*
            Removes a vertex from the graph.
            */

            void add_edge(Vertex& u, Vertex& v, EdgeWeight weight);
            /*
            Adds an edge to the graph.
            */

            void set_edge_weight(Vertex& u, Vertex& v, EdgeWeight weight);
            /*
            Changes the weight of an edge. Cannot be called on
            unweighted graphs.
            */

            void add_edges(Vertex edges[][2], EdgeWeight weights[]);
            /*
            Adds an array of edges to the graph.
            */

            void remove_edge(Vertex& u, Vertex& v);
            /*
            Removes an edge from the graph.
            */

            bool has_edge(Vertex& u, Vertex& v);
            /*
            Returns whether or not a given edge is in the graph.
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