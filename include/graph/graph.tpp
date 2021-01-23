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
    void Graph<Vertex, EdgeWeight>::set_edge_weights(const vector<Vertex>& us,
            const vector<Vertex>& vs, const vector<EdgeWeight>& weights) {

        int n_weights = weights.size();
        if ( us.size() != n_weights || vs.size != n_weights )
            throw std::invalid_argument("Number of us, vs, and weights must be the same.");

        vector<tuple<Vertex, Vertex, EdgeWeight*>> overridden_edges;
        tuple<Vertex, Vertex, EdgeWeight> overridden_edge;
        EdgeWeight* original_weight;
        
        for ( int i = 0; i < us.size(); i++ ) {
            try {
                try {
                    original_weight = &get_edge_weight(us[i], vs[i]);
                } catch ( std::invalid_argument ) {
                    // Edge doesn't exist. (New edge being added)
                    original_weight = nullptr;
                }
                set_edge_weight(us[i], vs[i], weights[i]);
                overridden_edge = {us[i], vs[i], original_weight};
                overridden_edges.push_back(overridden_edge);
            } catch ( std::exception e ) {
                // One of the edges was invalid.
                for ( tuple<Vertex, Vertex, EdgeWeight*> edge : overridden_edges ) {
                    if ( get<2>(edge) == nullptr )
                        remove_edge(get<0>(edge), get<1>(edge));
                    else
                        set_edge_weight(get<0>(edge), get<1>(edge), *get<2>(edge));
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