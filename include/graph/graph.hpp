/* ============================================================================================== *
 * graph.hpp                                                                                      *
 * authors: lol-cubes                                                                             *
 *                                                                                                *
 * A generic Graph abstract class.                                                                *
 * ============================================================================================== */


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
         * A generic Graph abstract class.
         */

        protected:

        bool directed;

        public:

        bool get_directed() { return directed; }
        /*
         * Returns whether or not the graph is directed.
         */

        virtual EdgeWeight get_edge_weight(const Vertex& u, const Vertex& v) = 0;
        /*
         * Returns the weight of an edge.
         */

        virtual void add_vertex(const Vertex& v) = 0;
        /*
         * Adds a vertex to the graph.
         */

        virtual void add_vertices(const unordered_set<Vertex>& vertices) = 0;
        /*
         * Adds a set of vertices to the graph.
         */

        virtual void remove_vertex(const Vertex& v) = 0;
        /*
         * Removes a vertex from the graph.
         */

        virtual void set_edge_weight(const Vertex& u, const Vertex& v,
            const EdgeWeight& weight) = 0;
        /*
         * Sets the weight of an edge.
         */

        virtual void remove_edge(const Vertex& u, const Vertex& v) = 0;
        /*
         * Removes an edge from the graph.
         */

        virtual bool has_edge(const Vertex& u, const Vertex& v) = 0;
        /*
         * Returns whether or not a given edge is in the graph.
         */

        virtual bool has_vertex(const Vertex& v) = 0;
        /*
         * Returns whether or not a certain vertex is in the graph.
         */

        virtual std::unordered_set<Vertex> get_children(const Vertex& v) = 0;
        /*
         * Returns the children of a given vertex in the graph.
         */

        virtual std::unordered_set<Vertex> get_parents(const Vertex& v) = 0;
        /*
         * Returns the parents of a given vertex in the graph.
         */
    };
}


#endif