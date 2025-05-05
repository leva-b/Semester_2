#include "mazefromfileparser.h"
#include <fstream>
#include <graph.h>
#include <math.h>
#include <QDebug>

MazeFromFileParser::MazeFromFileParser(std::string filepath, char wall_symbol, char path_symbol):
    MazeParser(wall_symbol, path_symbol), filepath_(filepath){parse();}

MazeFromFileParser::MazeFromFileParser(std::vector<std::vector<char>> grid, char wall_symbol, char path_symbol):
    MazeParser(wall_symbol, path_symbol), filepath_(), grid_(grid){}

Graph MazeFromFileParser::buildGraph()
{
    start_index = -1;
    end_index = -1;
    vertices_positions_.clear();
    for(size_t row = 0; row < grid_.size(); row++){             //добавление вершин
        for(size_t column = 0; column < grid_[row].size(); column++){
            if(isVertex(row, column)){
                vertices_positions_.emplace_back(row,column);
                addStartOrEnd(row, column);
            }
        }
    }

    Graph cur_graph(vertices_positions_.size());

    for(size_t i = 0; i < vertices_positions_.size(); i++){        //инициализируем граф
        for(size_t j = i + 1; j < vertices_positions_.size(); j++){
            if(vertices_positions_[i].first == vertices_positions_[j].first){
                if(isClearHorizontalPath(vertices_positions_[i], vertices_positions_[j])) {
                    int weight = abs(vertices_positions_[i].second - vertices_positions_[j].second);
                    cur_graph.addEdge(i, j, weight);
                }
            }
            // Вертикальные связи
            else if(vertices_positions_[i].second == vertices_positions_[j].second) {
                if(isClearVerticalPath(vertices_positions_[i], vertices_positions_[j])) {
                    int weight = abs(vertices_positions_[i].first - vertices_positions_[j].first);
                    cur_graph.addEdge(i, j, weight);
                }
            }


        }
    }

    cur_graph.addStartOrEnd(start_index);
    cur_graph.addStartOrEnd(end_index);
    return cur_graph;
}

bool MazeFromFileParser::isClearHorizontalPath(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    int row = a.first;
    int col_start = std::min(a.second, b.second);
    int col_end = std::max(a.second, b.second);

    for(int col = col_start + 1; col < col_end; col++) {
        if(grid_[row][col] != path_symbol_) {
            return false; // Нашли стену на пути
        }
    }
    return true;
}

bool MazeFromFileParser::isClearVerticalPath(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    int col = a.second;
    int row_start = std::min(a.first, b.first);
    int row_end = std::max(a.first, b.first);

    for(int row = row_start + 1; row < row_end; row++) {
        if(grid_[row][col] != path_symbol_) {
            return false; // Нашли стену на пути
        }
    }
    return true;
}

bool MazeFromFileParser::isVertex(int row, int column){
    if(grid_[row][column] != path_symbol_)return false;
    if(row == 0 || row == grid_.size() - 1){
        return true;
    }else if(column == 0 || column == grid_[row].size() - 1){
        return true;
    }else if((grid_[row][column - 1] == path_symbol_ || grid_[row][column + 1] == path_symbol_)
            && (grid_[row - 1][column] == path_symbol_ || grid_[row + 1][column] == path_symbol_))
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

void MazeFromFileParser::addStartOrEnd(size_t row, size_t column){
    if((row == 0 || column == 0 || row == grid_.size()-1 || column == grid_[0].size()-1)
        && grid_[row][column] == path_symbol_){
        //qDebug()  << (size_t)-1;
        if(start_index == (size_t)-1){
            start_index = vertices_positions_.size() - 1;
            qDebug() << "start"<<start_index;
        }else if(end_index == (size_t)-1){
            end_index = vertices_positions_.size() - 1;
            qDebug() << "end" << end_index;
        }
    }
}

const std::vector<std::pair<int, int>>& MazeFromFileParser::getVerticesPositions() const {
    return vertices_positions_;
}

std::vector<std::vector<char>>& MazeFromFileParser::getMazeData() {
    return grid_;
}

const std::vector<std::vector<char>>& MazeFromFileParser::getMazeConstData() const{
    return grid_;
}
