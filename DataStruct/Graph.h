//
// Created by zjp on 18-12-19.
//

#ifndef PARALLELPROGRAM_GRAPH_H
#define PARALLELPROGRAM_GRAPH_H

#define INFINITE 99999
#define NO_NEIGHBOR -2
#define DEBUGED

#include <string>
using namespace std;

class Graph{
private:
    int verNum;
    int* edge = nullptr;

public:

    Graph(int vertexNum=10);
    ~Graph();

    int firstNeighbor(int v);
    int nextNeighbor(int v,int w);
    int getVertexNum();
    int getEdge(int i,int j);
    void readFromFile(string file_name);
    void generateRandom(int n);
};
#endif //PARALLELPROGRAM_GRAPH_H
