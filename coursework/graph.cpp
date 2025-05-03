#include "graph.h"
#include <climits>

Graph::Graph(size_t number_vertices):
     number_vertices_(number_vertices), path_(number_vertices, -1) {}

void Graph::addEdge(size_t from, size_t to, int weight){
    graph_[from].emplace_back(to, weight);
    graph_[to].emplace_back(from, weight);
}

void Graph::dijkstra(){
    size_t begin_index = 0;
    const int size = number_vertices_;
    std::vector<int> distance(number_vertices_, INT_MAX);
    std::vector<bool> visited(number_vertices_, false);
    distance[begin_index] = 0;
    for(int i = 0; i < size; i++){
        int vertice = -1;
        for(int u = 0; u < size; u++){
            if(!visited[vertice] && (vertice == -1 || distance[u] < distance[vertice]))
                vertice = u;
        }

        visited[vertice] = true;
        for(auto [u, v]: graph_[vertice]){
            if(distance[u] > distance[vertice] + v)
            distance[u] = distance[vertice] + v;
            path_[u] = vertice;
        }
    }
}
