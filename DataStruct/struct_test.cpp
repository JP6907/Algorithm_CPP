//
// Created by zjp on 18-12-26.
//
#include <catch.h>
#include <iostream>
using namespace std;

#define LOSER_TREE

#ifdef LOSER_TREE

#include <LoserTree.h>
TEST_CASE("LoserTree") {
    int d[] = {INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX};
    int data[] = {10,9,20,6,12};
    LoserTree* loserTree = new LoserTree(5,d);
    cout << loserTree->getMin()[0] << " " << loserTree->getMin()[1] << endl;
    for(int i=0;i<5;i++)
        loserTree->setData(i,data[i]);

    cout << loserTree->getMin()[0] << " " << loserTree->getMin()[1] << endl;

    loserTree->setData(3,13);
    cout << loserTree->getMin()[0] << " " << loserTree->getMin()[1] << endl;

    loserTree->setData(3,13);
    cout << loserTree->getMin()[0] << " " << loserTree->getMin()[1] << endl;
}

#endif
