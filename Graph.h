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
#include<boost/tokenizer.hpp>


using namespace std;
using namespace boost;

class Graph {
private:

    int size;//how many nodes in the complete graph

    int totalTerm;//total terms in all maximal r-radius graph

    unordered_set<int>* adjList;

    vector<string> nodeContent;

    void addEdge(int src, int dest);

    unordered_map<string, set<int>> keywordNodeTable;

    unordered_map<string, set<int>> keywordGraphTable;

    unordered_map<int, unordered_map<int,unordered_set<int>>> graphIndexTable;

    unordered_map<int, int> graphIndexTermNumberTable;


public:
    Graph();

    ~Graph();

    string getNodeContentByNodeIndex(int nodeIndex);

    set<int> getNodeSetBykeyword(string keyword);

    set<int> getGraphIndexBykeyword(string keyword);

    unordered_map<int,unordered_set<int>> getGraphByGraphIndex(int graphIndex);

    void printGraph();

    int readNode(std::string nodeFilePath);

    void readEdge(std::string edgeFilePath);

    unordered_map<int, unordered_set<int>> getMaximalRRadiusGraph(int nodeID, int radius);

    void printMap(unordered_map<int, unordered_set<int>> um);

    set<int> getRRadiusNodes(int nodeID, int r);

    void printSet(set<int> s);

    vector<unordered_map<int, unordered_set<int>>> getAllMaximalRRadiusGraph(int radius);

    void printNodeContent();

    void printKeywordNodeTable();

    void printKeywordGraphTable();

    void printGraphIndexTable();

    unordered_map<int, unordered_set<int>> getRRadiusSteinerGraph(unordered_map<int, unordered_set<int>> rRadiusMaximalGraph, unordered_set<int> contentNodeSet);

    unordered_map<int, unordered_set<int>> getRRadiusSteinerGraph(int rRadiusMaximalGraphIndex, unordered_set<int> contentNodeSet);

    set<int> getNodeSetP(unordered_map<int, unordered_set<int>> rRadiusMaximalGraph, unordered_set<int> contentNodeSet, int nodeIndex);

    set<int> convertUsToOs(unordered_set<int> &us);

    unordered_set<string> extractKeywords(string input);

    set<int> searchRRadiusMaximalGraph(unordered_set<string> keywordSet);

    vector<unordered_map<int, unordered_set<int>>> searchRRadiusSteinerGraph(string input);

    unordered_set<int> findContentNodeInGraph(int graphIndex, unordered_set<string> keywordSet);

    bool contains(string data, unordered_set<string> keywordSet);
};



#endif //INC_3_IN_1_GR_H
