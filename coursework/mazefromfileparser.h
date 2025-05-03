#ifndef MAZEFROMFILEPARSER_H
#define MAZEFROMFILEPARSER_H
#include "mazeparser.h"
#include <string>
#include <vector>

class   MazeFromFileParser : public MazeParser
{
private:
    std::string filepath_;
    std::vector<std::vector<char>> grid_;

    bool isVertex(int row, int column);

public:
    MazeFromFileParser(std::string filepath, char wall_symbol, char path_symbol);
    MazeFromFileParser(std::vector<std::vector<char>> grid, char wall_symbol, char path_symbol);
    Graph buildGraph() override;
    void parse();
    std::vector<std::vector<char>>& getMazeData();

    const std::vector<std::vector<char>>& getMazeConstData() const;
};

#endif // MAZEFROMFILEPARSER_H
