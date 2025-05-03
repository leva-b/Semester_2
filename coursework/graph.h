#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <utility>

class Graph
{
private:
    const size_t number_vertices_ = 0;
    std::vector<size_t> path_;
    std::vector<std::vector<std::pair<int,int>>> graph_;
public:

    Graph(size_t number_vertices);
    Graph(){}
    void addEdge(size_t from, size_t to, int weight);
    void dijkstra();
};

#endif // GRAPH_H
