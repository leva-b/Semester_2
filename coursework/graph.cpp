#include "graph.h"
#include <climits>
#include <QMessageBox>

Graph::Graph(size_t number_vertices):
    number_vertices_(number_vertices), path_(number_vertices, -1), graph_(number_vertices) {}

void Graph::addEdge(size_t from, size_t to, int weight){
    graph_[from].emplace_back(to, weight);
    graph_[to].emplace_back(from, weight);
}

void Graph::dijkstra(){
    const size_t size = number_vertices_;
    std::vector<int> distance(number_vertices_, INT_MAX);
    std::vector<bool> visited(number_vertices_, false);
    distance[start_index] = 0;

    for(size_t i = 0; i < size; i++){

        size_t vertice = -1;
        for(size_t u = 0; u < size; u++){
            if(!visited[u] && (vertice == (size_t)-1 || distance[u] < distance[vertice]))
                vertice = u;
        }

        if (vertice == (size_t)-1 || distance[vertice] == INT_MAX) {
            break; // Нет достижимых вершин
        }
        visited[vertice] = true;
        for(const auto& [u, v]: graph_[vertice]){
            if(distance[u] > distance[vertice] + v){
            distance[u] = distance[vertice] + v;
            path_[u] = vertice;
            }
        }
    }
    if (distance[end_index] == INT_MAX) {
        QMessageBox::warning(nullptr, "No Path", "The maze has no exit.");
        return;
    }
}

std::vector<size_t> Graph::getPath() const {
    std::vector<size_t> result;
    //qDebug() << start_index << " " << end_index;
    if(end_index == (size_t)-1 || start_index == (size_t)-1 || path_.empty()) return result;

    // Восстанавливаем путь от конца к началу
    for(size_t v = end_index; v != start_index; v = path_[v]) {
        result.push_back(v);
        if(v == path_[v]) break;
    }
    result.push_back(start_index);
    reverse(result.begin(), result.end());
    qDebug() << "getPath";
    return result;
}

void Graph::addStartOrEnd(size_t index){
    if(start_index == (size_t)-1){
        start_index = index;
    }else if(end_index == (size_t)-1){
        end_index = index;
    }
}
