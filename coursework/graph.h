#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <utility>

class Graph
{
private:
    size_t number_vertices_ = 0;
    std::vector<int> path_;
    std::vector<std::vector<std::pair<size_t,int>>> graph_;
    size_t start_index = -1;
    size_t end_index = -1;
public:
    void addStartOrEnd(size_t index);
    Graph(size_t number_vertices);
    //Graph& operator=(const Graph& other);
    void addEdge(size_t from, size_t to, int weight);
    void dijkstra();

    /*void initializationStartOrEnd();*/
    std::vector<size_t> getPath() const;

};

#endif // GRAPH_H
