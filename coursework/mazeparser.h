#ifndef MAZEPARSER_H
#define MAZEPARSER_H
#include "graph.h"

class MazeParser
{
public:
    MazeParser(){}
    virtual Graph buildGraph() = 0;
    virtual void parse() = 0;
};

#endif // MAZEPARSER_H
