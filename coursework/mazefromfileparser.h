#ifndef MAZEFROMFILEPARSER_H
#define MAZEFROMFILEPARSER_H
#include "mazeparser.h"
#include <string>
#include <vector>

class   MazeFromFileParser : public MazeParser
{
private:
    size_t start_index = -1;
    size_t end_index = -1;
    std::string filepath_;
    std::vector<std::vector<char>> grid_;
    std::vector<std::pair<int, int>> vertices_positions_;
    bool isVertex(int row, int column);
    void addStartOrEnd(size_t row, size_t column);
    bool isClearVerticalPath(const std::pair<int,int>& a, const std::pair<int,int>& b);
    bool isClearHorizontalPath(const std::pair<int,int>& a, const std::pair<int,int>& b);

public:
    MazeFromFileParser(std::string filepath, char wall_symbol, char path_symbol);
    MazeFromFileParser(std::vector<std::vector<char>> grid, char wall_symbol, char path_symbol);
    Graph buildGraph() override;
    void parse();
    std::vector<std::vector<char>>& getMazeData();
    const std::vector<std::vector<char>>& getMazeConstData() const;
    const std::vector<std::pair<int, int>>& getVerticesPositions() const;
};
#endif // MAZEFROMFILEPARSER_H
