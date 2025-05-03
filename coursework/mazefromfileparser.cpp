#include "mazefromfileparser.h"
#include <fstream>
#include <graph.h>
#include <math.h>

MazeFromFileParser::MazeFromFileParser(std::string filepath, char wall_symbol, char path_symbol):
    MazeParser(), filepath_(filepath), wall_symbol_(wall_symbol), path_symbol_(path_symbol){}


MazeFromFileParser::MazeFromFileParser(std::vector<std::vector<char>> grid, char wall_symbol, char path_symbol):
    MazeParser(), filepath_(), grid_(grid), wall_symbol_(wall_symbol), path_symbol_(path_symbol){}

Graph MazeFromFileParser::buildGraph()
{
    if(!filepath_.empty())
        parse();

    std::vector<std::pair<int, int>> vertices_positions;

    for(size_t row = 0; row < grid_.size(); row++){             //добавление вершин
        for(size_t column = 0; column < grid_[row].size(); column++){
            if(grid_[row][column] == path_symbol_ && isVertex(row, column)){
                vertices_positions.emplace_back(row,column);
            }
        }
    }

    Graph cur_graph(vertices_positions.size());
    int weight = 0;
    for(size_t i = 0; i < vertices_positions.size(); i++){        //инициализируем граф
        for(size_t j = 0; j < vertices_positions.size(); j++){
            if(vertices_positions[i].first == vertices_positions[j].first){
                weight = abs(vertices_positions[i].second - vertices_positions[j].second);
            }else if(vertices_positions[i].second == vertices_positions[j].second){
                weight = abs(vertices_positions[i].first - vertices_positions[j].first);
            }
            cur_graph.addEdge(i, j, weight);
        }
    }
    return cur_graph;
}

bool MazeFromFileParser::isVertex(int row, int column){
    if(row == 0 || row == (int)grid_.size() - 1)
        return true;
    else if(column == 0 || column == (int)grid_.size() - 1)
        return true;
    else if((grid_[row][column - 1] == path_symbol_ || grid_[row][column + 1])
            && (grid_[row - 1][column - 1] == path_symbol_ || grid_[row + 1][column]))
            return true;
    return false;
}

void MazeFromFileParser::parse()
{
    std::ifstream file(filepath_);
    if (!file) {
        throw("File opening error");
    }

    std::string line;
    while (std::getline(file, line)) {
        grid_.emplace_back(line.begin(), line.end());
    }
}
