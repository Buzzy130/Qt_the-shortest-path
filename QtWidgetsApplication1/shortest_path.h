#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H
#pragma once

#include "visibility_graph.h"
#include "dijkstra.h"

using namespace std;

inline
void create_aliases(const C_obs& obs, const vertex& init, const vertex& goal, map<vertex, int>& vertex2int, map<int, vertex>& int2vertex) {
    set<vertex> vertices = obs.get_vertices();
    vertex2int[init] = 0;
    int2vertex[0] = init;
    int i = 1;
    for (vertex v : vertices) {
        int2vertex[i] = v;
        vertex2int[v] = i++;
    }
    vertex2int[goal] = i;
    int2vertex[i] = goal;
}

inline
vector<vertex> shortest_path(vertex init, vertex goal, C_obs obs, int r) {
    map<vertex, int> vertex2int;
    map<int, vertex> int2vertex;
    create_aliases(obs, init, goal, vertex2int, int2vertex);
    Graph visibility_graph = build_visibility_graph(vertex2int, init, goal, obs, r);
    vector<int> path = dijkstra(visibility_graph, vertex2int[init], vertex2int[goal]);
    vector<vertex> vertices_path;
    for (auto int_value : path) {
        vertices_path.push_back(int2vertex[int_value]);
    }

    return vertices_path;
}

#endif // SHORTEST_PATH_H
