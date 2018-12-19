//
// Created by zjp on 18-12-18.
//
/**
 * 工作池机制
 * 集中式工作池
 * mpicxx mpi/GraphSearch.cpp DataStruct/Graph.cpp -I DataStruct -o GraphSearch
 */
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <string>
#include <iostream>
#include "Graph.h"
#include <mpi.h>

using namespace std;

#define DEBUGED

#define REQUEST_TASK_TAG 11    //从进程向主进程请求任务标志
#define ALLOCAT_TASK_TAG 12  //主进程向从进程分配任务标志
#define ALLOCAT_DIST_TAG 13  //主进程向从进程分配任务时发送dist数组标志
#define UPDATE_DIST_TAG 14  //从进程向主进程发送更新后的dist数组
#define INQUEUE_NODE_TAG 15  //从进程向主进程发送需要入队的节点数组

#define TERMINATION_TAG 16  //主进程向从进程发送结束标志

void printGraph(Graph graph);
void printArray(string flag,int array[],int n);
//Moore方法求最短路径
void MooreSearch(Graph graph);


int main(){

    string file_name = "/home/zjp/Documents/Parallel/ParallelProgram/mpi/TSP_Diagram.txt";
    Graph graph;
    graph.readFromFile(file_name);
    //graph.generateRandom(20);
    int n = graph.getVertexNum();
    queue<int> mqueue;
    int *dist = new int[graph.getVertexNum()];
    dist[0] = 0;
    for(int i=1;i<n;i++)
        dist[i] = INFINITE;

    int my_rank,comm_sz;
    int requestBuff = 0;
    int *inQueueNodeBuff = new int[n];  //用于存放需要入队的节点
    //int *local_dist = new int[n];
    MPI_Status status;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    //MooreSearch(graph);
    double start,end;

    if(my_rank==0){ //主进程
        start = MPI_Wtime();
        mqueue.push(0);
        while(!mqueue.empty()){
            MPI_Recv(&requestBuff,0,MPI_INT,MPI_ANY_SOURCE,REQUEST_TASK_TAG,MPI_COMM_WORLD,&status); //获取任务请求
            int source = status.MPI_SOURCE;
            int v = mqueue.front();  //分配任务
            cout << "allocate " << v << " to " << source << endl;
            mqueue.pop();
            MPI_Send(&v,1,MPI_INT,source,ALLOCAT_TASK_TAG,MPI_COMM_WORLD);  //发送节点更新任务
            MPI_Send(dist,n,MPI_INT,source,ALLOCAT_DIST_TAG,MPI_COMM_WORLD); //发送dist数组
            MPI_Recv(dist,n,MPI_INT,source,UPDATE_DIST_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE); //接收更新后的数组
            MPI_Recv(inQueueNodeBuff,n,MPI_INT,source,INQUEUE_NODE_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE); //接收需要入队的节点数组
            cout << "receive update data from " << source << endl;
            for(int i=0;i<n&&inQueueNodeBuff[i]!=-1;i++)  //入队
                mqueue.push(inQueueNodeBuff[i]);
            //cout << "after inqueue data from " << source << endl;
        }
        cout << "finish all the task!" << endl;
        //MPI_Recv(&requestBuff,0,MPI_INT,MPI_ANY_SOURCE,REQUEST_TASK_TAG,MPI_COMM_WORLD,&status); //获取任务请求
        for(int i=1;i<comm_sz;i++)  //发送结束标志
            MPI_Send(&requestBuff,1,MPI_INT,i,TERMINATION_TAG,MPI_COMM_WORLD);
    }else{ //从进程
        int v,i;
        MPI_Send(&requestBuff,0,MPI_INT,0,REQUEST_TASK_TAG,MPI_COMM_WORLD);  //向主进程请求任务
        MPI_Recv(&v,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        while(status.MPI_TAG!=TERMINATION_TAG){
            MPI_Recv(dist,n,MPI_INT,0,ALLOCAT_DIST_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE); //接收dist数组
            //cout << "rank" << my_rank << " receive task : node " << v << endl;
            i = 0;
            for(int neighbor=graph.firstNeighbor(v);neighbor!=NO_NEIGHBOR;neighbor=graph.nextNeighbor(v,neighbor)){
                int newDist = dist[v] + graph.getEdge(v,neighbor);
                if(newDist<dist[neighbor]) {
                    dist[neighbor] = newDist;
                    inQueueNodeBuff[i++] = neighbor;
                }
            }
            inQueueNodeBuff[i] = -1;
            MPI_Send(dist,n,MPI_INT,0,UPDATE_DIST_TAG,MPI_COMM_WORLD); //发送dist数组
            MPI_Send(inQueueNodeBuff,n,MPI_INT,0,INQUEUE_NODE_TAG,MPI_COMM_WORLD);//发送inQueueNodeBuffsh数组
            MPI_Send(&requestBuff,0,MPI_INT,0,REQUEST_TASK_TAG,MPI_COMM_WORLD); //请求新的任务
            //cout << "rank" << my_rank << " finish task " << v << endl;
            MPI_Recv(&v,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        }
    }

    if(my_rank==0) {
        end = MPI_Wtime();
        cout << "time:" << end-start << endl;
        printArray("dist", dist, n);
    }


    MPI_Finalize();


    return 0;
}

void MooreSearch(Graph graph){

    double start,end;
    int n = graph.getVertexNum();
    queue<int> mqueue;
    int *dist = new int[graph.getVertexNum()];

#ifdef DEBUG
    cout << endl;
    cout << graph.getEdge(0,1) << endl;
    //printGraph(graph);
    cout << graph.getEdge(0,1) << endl;
#endif

    //memcpy(dist,graph,n*sizeof(int));
    //memset(dist,INFINITE, sizeof(int));
    dist[0] = 0;
    for(int i=1;i<n;i++)
        dist[i] = INFINITE;
#ifdef DEBUG
    printArray("dist",dist,n);
#endif
    cout << graph.getEdge(0,1) << endl;
    start = MPI_Wtime();
    //printGraph(graph);
    mqueue.push(0);
    while(!mqueue.empty()){
        int currNode = mqueue.front();
        int neighbor;
        mqueue.pop();
        for(neighbor=graph.firstNeighbor(currNode);neighbor!=NO_NEIGHBOR;neighbor=graph.nextNeighbor(currNode,neighbor)){
            int newDist = dist[currNode] + graph.getEdge(currNode,neighbor);
            if(newDist<dist[neighbor]){
                dist[neighbor] = newDist;
                mqueue.push(neighbor);
            }
        }
    }
    end = MPI_Wtime();
    cout << "time:" << end-start << endl;
    printArray("dist",dist,n);
}


void printGraph(Graph graph){
    cout << "Graph:" << endl;
    for(int i=0;i<graph.getVertexNum();i++){
        for(int j=0;j<graph.getVertexNum();j++) {
            cout.width(6);
            cout << graph.getEdge(i, j) << " ";
        }
        cout << endl;
    }
}

void printArray(string flag,int array[],int n){
    cout << flag << ": ";
    for(int i=0;i<n;i++)
        cout << array[i] << " ";
    cout << endl;
}

/**
 * Moore并行算法实际上只能每个进程之间只能是串行工作
 * 因为dist数组迭代之间存在循环相关
 * 所以进程数增加，实际上算法消耗时间是更多的，存在更多的进程通信时间
 * 进程数      时间
 *  1     0.000025034
 *  2     0.000503778
 *  3     0.000691891
 *  4     0.00131655
 */