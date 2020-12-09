/*
A general graph class. Meant to be inherited. No real graph theory
functionality.
*/

#ifndef CYGRAPH_GRAPH_HPP
#define CYGRAPH_GRAPH_HPP

#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


using std::get;
using std::tuple;
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

        virtual void add_vertices(const unordered_set<Vertex>& vertices) = 0;
        /*
        Adds an array of vertices to the graph.
        */

        virtual void remove_vertex(const Vertex& v) = 0;
        /*
        Removes a vertex from the graph.
        */

        virtual void remove_vertices(const unordered_set<Vertex>& vertices) {
            /*
            Removes a set of vertices from the graph.
            */
            for ( Vertex v : vertices ) {
                if ( !has_vertex(v) ) {
                    throw std::invalid_argument("Attempting to remove vertex that does not exist");
                }
            }
            for ( Vertex v : vertices ) {
                remove_vertex(v);
            }
        }

        virtual void set_edge_weight(const Vertex& u, const Vertex& v,
            const EdgeWeight weight) = 0;
        /*
        Sets the weight of an edge.
        */

        virtual void set_edge_weights(const vector<tuple<Vertex, Vertex, EdgeWeight>>& edges) {
            /*
            Sets the weights of a vector of edges in the graph.
            */
            vector<pair<Vertex, Vertex>> set_edges;
            pair<Vertex, Vertex> edge_;
            Vertex u, v;
            for ( tuple<Vertex, Vertex, EdgeWeight> edge : edges ) {
                try {
                    u = get<0>(edge);
                    v = get<1>(edge);
                    set_edge_weight(u, v, get<2>(edge));
                    edge_ = {u, v};
                    set_edges.push_back(edge_);
                } catch ( std::invalid_argument e ) {
                    for ( pair<Vertex, Vertex> removal_edge : set_edges ) {
                        remove_edge(removal_edge.first, removal_edge.second);
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

        virtual void remove_edges(const vector<pair<Vertex, Vertex>>& edges) {
            /*
            Removes a set of edges from the graph.
            */
            vector<tuple<Vertex, Vertex, EdgeWeight>> removed_edges;
            EdgeWeight weight;
            tuple<Vertex, Vertex, EdgeWeight> edge_;

            for ( pair<Vertex, Vertex> edge : edges ) {
                try {
                    weight = get_edge_weight(edge.first, edge.second);
                    remove_edge(edge.first, edge.second);
                    edge_ = {edge.first, edge.second, weight};
                    removed_edges.push_back(edge_);

                } catch ( std::invalid_argument e ) {
                    for ( tuple<Vertex, Vertex, EdgeWeight> readdition_edge : removed_edges ) {
                        set_edge_weight(get<0>(readdition_edge), get<1>(readdition_edge),
                                        get<2>(readdition_edge));
                    }
                    throw e;
                }
            }
        }

        virtual bool has_edge(const Vertex& u, const Vertex& v) = 0;
        /*
        Returns whether or not a given edge is in the graph. If one or more of the vertices are
        not in the graph, false is returned.
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