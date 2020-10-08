/*
A fast and flexible Graph theory library.
*/


#include <any>
#include <set>
#include <unordered_map>
#include <vector>


using std::any;


namespace cygraph {

    template<class Vertex, class EdgeWeight> class Graph {
        /*
        A generic Graph class. Not to be instantiated.
        */
        protected:
            std::unordered_map<Vertex, std::unordered_map<any, any>>
                vertex_attributes;
            std::unordered_map<Vertex[2], std::unordered_map<any, any>>
                edge_attributes;
            std::vector<Vertex> vertices;
            std::unordered_map<Vertex, int> vertex_indices;
            bool directed;

        public:
            bool get_directed();
            /*
            Returns whether or not the graph is directed.
            */

            vector<Vertex> get_vertices();
            /*
            Returns a vector containing the vertices in the graph.
            */

            EdgeWeight get_edge_weight(Vertex* u, Vertex* v);
            /*
            Returns the weight of an edge.
            */

            void set_vertex_attribute(Vertex* v, char* key, any value);
            /*
            Stores an attribute to a vertex. Can alter existing
            attributes.

            Parameters
            ----------
            v: Vertex*
                The vertex to store the attribute at.
            key: char*
                The name (key) under which the attribute will be stored.
            value: any
                The value of the attribute.
            */

            void remove_vertex_attribute(Vertex* v, char* key);
            /*
            Removes an attribute from a vertex.

            Parameters
            ----------
            v: Vertex*
                The vertex containing the attribute to be removed.
            key: char*
                Pointer to the name (key) of the attribute to remove.
            */

            void set_vertex_attributes(Vertex* v,
                std::unordered_map<char*, any>* attributes);
            /*
            Sets multiple attributes to a vertex. Can alter existing
            attributes.

            Parameters
            ----------
            v: Vertex*
                The vertex that will receive the new attributes.
            attributes: unordered_map<any, any>*
                A dictionary of attributes in key-value pairs.
            */

            any get_vertex_attribute(Vertex* v, char* key);
            /*
            Returns an attribute of a vertex.

            Parameters
            ----------
            v: Vertex*
                A vertex in the graph.
            key: char*
                The key of an attribute of the vertex.
            */
            
            void set_edge_attribute(Vertex* u, Vertex* v, char* key,
                any value);
            /*
            Sets an attribute to an edge. Can alter existing attributes.

            Parameters
            ----------
            u: Vertex*
                The first vertex of the edge.
            v: Vertex*
                The second vertex of the edge.
            key: char*
                The name under which the attribute will be stored.
            value: any
                The value of the attribute.
            */

            void remove_edge_attribute(Vertex* u, Vertex* v, char* key);
            /*
            Removes an attribute from an edge.

            Parameters
            ----------
            u: Vertex*
                The first vertex in the edge.
            v: Vertex*
                The second vertex in the edge.
            key: char*
                The name under which the attribute is stored.
            */

            void set_edge_attributes(Vertex* u, Vertex* v,
                std::unordered_map<any, any>* attributes);
            /*
            Sets multiple attributes to an edge.

            Parameters
            ----------
            u: Vertex*
                The first vertex in the edge.
            v: Vertex*
                The second vertex in the edge.
            attributes: unordered_map<any, any>*
                A dictionary of attributes in key-value pairs.
            */

            any get_edge_attribute(Vertex* u, Vertex* v, char* key);
            /*
            Returns the value of an attribute of an edge.

            Parameters
            ----------
            u: Vertex*
                The first vertex in the edge.
            v: Vertex*
                The second vertex in the edge.
            key: char*
                The name under which the attribute is stored.
            */

            void add_vertex(Vertex* v);
            /*
            Adds a vertex to the graph.
            */

            void add_vertices(Vertex[] vertices);
            /*
            Adds an array of vertices to the graph.
            */

            void remove_vertex(Vertex* v);
            /*
            Removes a vertex from the graph.
            */

            void add_edge(Vertex* u, Vertex* v, EdgeWeight weight);
            /*
            Adds an edge to the graph.
            */

            void set_edge_weight(Vertex* u, Vertex* v, EdgeWeight weight);
            /*
            Changes the weight of an edge. Cannot be called on
            unweighted graphs.
            */

            void add_edges(Vertex edges[2][], EdgeWeight weights[]);
            /*
            Adds an array of edges to the graph.
            */

            void remove_edge(Vertex* u, Vertex* v);
            /*
            Removes an edge from the graph.
            */

            bool has_edge(Vertex* u, Vertex* v);
            /*
            Returns whether or not a given edge is in the graph.
            */

            bool has_vertex(Vertex* v);
            /*
            Returns whether or not a certain vertex is in the graph.
            */


            set<Vertex> get_children(Vertex* v);
            /*
            Returns the children of a given vertex in the graph.
            */

            set<Vertex> get_parents(Vertex* v);
            /*
            Returns the parents of a given vertex in the graph. This is
            equivalent to get_children in undirected graphs.
            */
    };
}