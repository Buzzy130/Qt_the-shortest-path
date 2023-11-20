#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "graph.h"
#include <vector>
#include "shortest_path.h"
#include <algorithm>
#include <map>
#include <utility>

using namespace std;

inline
int min_dist(const map<int, double>& p) {
    double min = INT_MAX;
    int vert = INT_MAX;
    for (auto x : p) {
        if (x.second <= min) {
            min = x.second;
            vert = x.first;
        }
    }
    return vert;
}

inline
vector<int> build_path(map<int, int> parent, int start, int end) {
    vector<int> path;

    if (parent.size() < 0) {
        return  vector<int>{};
    }

    for (int i = end; i != start; /*&& parent.size() > 0 && parent[i] < parent.size();*/) {
        if (parent[i] == INT_MAX) {
            return vector<int>{};
        }
        else {
            path.insert(path.begin(), i);
            i = parent[i];
        }
    }
    if (end != start) {
        path.insert(path.begin(), start);
    }

    return path;
}

inline
vector<int> dijkstra(const Graph& graph, int start_vertex, int end_vertex) {
    // Return shortest path in the graph from start vertex to end vertex as array of vertices.
    // First item in the result should be start vertex, last - end vertex.
    // Return empty array if there is no path.

    // Your implementation here.

    map<int, int> parent;

    map<int, double> Q;

    for (int i = 0; i < graph.get_vertices().size(); i++) {
        Q[graph.get_vertices()[i]] = INT_MAX;
        parent[graph.get_vertices()[i]] = INT_MAX;
    }

    if (Q.size() > 0 && parent.size() > 0) {
        Q[start_vertex] = 0;
        parent[start_vertex] = end_vertex;
    }

    while (Q.size() > 0) {
        int u = min_dist(Q);

        if (u == end_vertex) {
            return build_path(parent, start_vertex, end_vertex);
        }
        for (auto v : graph.get_adjacent_vertices(u)) {
            if (Q.find(v) == Q.end()) {
                continue;
            }
            else {
                if (Q[v] > Q[u] + graph.edge_weight(u, v)) {
                    Q[v] = Q[u] + graph.edge_weight(u, v);
                    parent[v] = u;
                }
            }
        }
        Q.erase(u);

    }
    return vector<int>{};
}
#endif // DIJKSTRA_H

