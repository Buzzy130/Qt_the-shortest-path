#ifndef VISIBILITY_GRAPH_H
#define VISIBILITY_GRAPH_H
#pragma once
#include "graph.h"
#include "C_obs.h"
#include <math.h>
inline
const Graph build_visibility_graph(std::map<vertex, int> alias, vertex const& init, vertex const& goal, const C_obs& obs, int r) {
    Graph visibility_graph;
    std::set<vertex> vertices = obs.get_vertices();
    vertices.insert(init);
    vertices.insert(goal);


    for (vertex v : vertices) {
        for (vertex u : obs.get_visible_vertices(r, v, goal)) {
            if (!(v == u)) {
                double distance = sqrt((v.x - u.x) * (v.x - u.x) + (v.y - u.y) * (v.y - u.y));
                visibility_graph.add_edge(alias[v], alias[u], distance);
            }
        }
    }

    if (!visibility_graph.has_vertex(alias[goal])) {
        return Graph{};
    }

    return visibility_graph;
}
#endif // VISIBILITY_GRAPH_H
