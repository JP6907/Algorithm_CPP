//
// Created by zjp on 18-12-19.
//
#include "Graph.h"
#include <stdio.h>
#include <iostream>

using namespace std;

Graph::Graph(int vertexNum){
    verNum = vertexNum;
    edge = new int[verNum*verNum]();
};

Graph::~Graph() {
    delete []edge;
}

int Graph::firstNeighbor(int v){
    int i;
    for(i=0;i<verNum;i++){
        if(i!=v&&edge[v*verNum+i]!=INFINITE)
            return i;
    }
    return NO_NEIGHBOR;
}

int Graph::nextNeighbor(int v,int w){
    int i;
    for(i=w+1;i<verNum;i++){
        if(i!=v&&edge[v*verNum+i]!=INFINITE)
            return i;
    }
    return NO_NEIGHBOR;
}

int Graph::getVertexNum(){
    return verNum;
}

int Graph::getEdge(int i,int j){
    return edge[i*verNum+j];
}

//file_name必须用绝对路径
void Graph::readFromFile(string file_name){
    FILE* digraph_file = fopen(file_name.data(),"rb");
    if(digraph_file== nullptr){
        cout << "open file failed!" << endl;
        exit(-1);
    }
    int i, j;
    fscanf(digraph_file, "%d", &verNum);
#ifdef DEBUG
    cout << "verNum: " << verNum << endl;
#endif
    if (verNum <= 0)
    {
        cout << "<readFromFile> Number of nodes = %d\n" << endl;
        exit(1);
    }
    int *digraph = new int[verNum*verNum]();
    for (i = 0; i < verNum; i++)
    {
        for (j = 0; j < verNum; j++)
        {
            fscanf(digraph_file, "%d", &digraph[i*verNum+j]);
            if(digraph[i*verNum+j]==-1) //文件中-1代表无穷，不存在路径
                digraph[i*verNum+j] = INFINITE;
#ifdef DEBUG
            cout.width(6);
            cout << digraph[i*verNum+j] << " ";
#endif
        }
#ifdef DEBUG
        cout << endl;
#endif
    }
    fclose(digraph_file);

    delete []edge;
    edge = digraph;
}

void Graph::generateRandom(int n){
    verNum = n;
    int* graph = new int[verNum*verNum]();
    for(int i=0;i<verNum;i++){
        for(int j=0;j<verNum;j++){
            if(i==j)
                graph[i*verNum+j] = 0;
            else{
                if(rand()%10<2)
                    graph[i*verNum+j] = INFINITE;
                else
                    graph[i*verNum+j] = rand()%50;
            }
            cout.width(6);
            cout << graph[i*verNum+j] << " ";
        }
        cout << endl;
    }
    if(edge!= nullptr)
        delete []edge;
    edge = graph;
}