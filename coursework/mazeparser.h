#ifndef MAZEPARSER_H
#define MAZEPARSER_H
#include "graph.h"

class MazeParser
{
protected:
    const char wall_symbol_;
    const char path_symbol_;
public:
    MazeParser(char wall_symbol, char path_symbol): wall_symbol_(wall_symbol), path_symbol_(path_symbol){}
    virtual Graph buildGraph() = 0;
    virtual ~MazeParser(){}
};

#endif // MAZEPARSER_H
