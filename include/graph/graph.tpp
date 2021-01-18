/*
Template implementation file for base graph class.
*/


#include <algorithm>
#include <stdexcept>
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
    void Graph<Vertex, EdgeWeight>::remove_vertices(
            const unordered_set<Vertex>& vertices) {

        // Check if all vertices are in graph.
        for ( Vertex v : vertices ) {
            if ( !has_vertex(v) ) {
                throw std::invalid_argument("Attempting to remove vertex that does not exist");
            }
        }
        // Remove all vertices.
        for ( Vertex v : vertices ) {
            remove_vertex(v);
        }
    }

    template<class Vertex, class EdgeWeight>
    void Graph<Vertex, EdgeWeight>::set_edge_weights(
            const vector<tuple<Vertex, Vertex, EdgeWeight>>& edges) {

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
                // One of the edges was invalid.
                for ( pair<Vertex, Vertex> removal_edge : set_edges ) {
                    remove_edge(removal_edge.first, removal_edge.second);
                }
                throw e;
            }
        }
    }


    template<class Vertex, class EdgeWeight>
    void Graph<Vertex, EdgeWeight>::remove_edges(
            const vector<pair<Vertex, Vertex>>& edges) {

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
                // One of the edges was invalid.
                for ( tuple<Vertex, Vertex, EdgeWeight> readdition_edge : removed_edges ) {
                    set_edge_weight(get<0>(readdition_edge), get<1>(readdition_edge),
                                    get<2>(readdition_edge));
                }
                throw e;
            }
        }
    }
}