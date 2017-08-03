//
// Created by wushuang on 7/27/17.
//

#ifndef INC_3_IN_1_GR_H
#define INC_3_IN_1_GR_H
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

using namespace std;

class Graph {
private:
    int size;//how many nodes in graph

    unordered_set<int>* adjList;

    vector<string> nodeContent;

    void addEdge(int src, int dest);

public:
    Graph();

    ~Graph();

    void printGraph();

    int readNode(std::string nodeFilePath);

    void readEdge(std::string edgeFilePath);

    unordered_map<int, unordered_set<int>> getRRadiusGraph(int nodeID, int radius);

    void printMap(unordered_map<int, unordered_set<int>> um);

    set<int> getRRadiusNodes(int nodeID, int r);

    void printSet(set<int> s);

    vector<unordered_map<int, unordered_set<int>>> getAllMaximalRRadiusGraph(int radius);
};



#endif //INC_3_IN_1_GR_H
