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
#include <queue>
#include <set>
#include <algorithm>
#include<boost/tokenizer.hpp>


using namespace std;
using namespace boost;

class Graph {
private:

    int size;//how many nodes in the complete graph

    int totalTerm = 0;

    unordered_set<int>* adjList;

    //first element in pair is node content, second is its term number
    vector<pair<string, int>> nodeContent;

    void addEdge(int src, int dest);

    unordered_map<string, set<int>> keywordNodeTable;

    unordered_map<string, set<int>> keywordGraphTable;

    unordered_map<int, unordered_map<int,unordered_set<int>>> graphIndexTable;

    unordered_map<int, int> graphIndexTermNumberTable;


public:
    Graph();

    ~Graph();

    string getNodeContentByNodeIndex(int nodeIndex);

    int getNodeTermNumberByNodeIndex(int nodeIndex);

    set<int> getNodeSetByKeyword(string keyword);

    set<int> getGraphIndexByKeyword(string keyword);

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

    void printGraphIndexTermNumberTable();

    unordered_map<int, unordered_set<int>> getRRadiusSteinerGraph(unordered_map<int, unordered_set<int>> rRadiusMaximalGraph, unordered_set<int> contentNodeSet);

    unordered_map<int, unordered_set<int>> getRRadiusSteinerGraph(int rRadiusMaximalGraphIndex, unordered_set<int> contentNodeSet);

    set<int> getNodeSetP(unordered_map<int, unordered_set<int>> rRadiusMaximalGraph, unordered_set<int> contentNodeSet, int nodeIndex);

    set<int> convertUsToOs(unordered_set<int> &us);

    unordered_set<string> extractKeywords(string input);

    set<int> searchRRadiusMaximalGraph(unordered_set<string> keywordSet);

    vector<unordered_map<int, unordered_set<int>>> searchRRadiusSteinerGraph(string input);

    unordered_set<int> findContentNodeInGraph(int graphIndex, unordered_set<string> keywordSet);

    bool contains(string data, unordered_set<string> keywordSet);

    //TF*IDF-based IR Ranking computation
    float computeNTF(string keyword, int graphIndex);

    float computeNTF(string keyword, unordered_map<int, unordered_set<int>> graph);

    float computeIDF(string keyword);

    float computeNDL(int graphIndex);

    float computeNDL(unordered_map<int, unordered_set<int>> graph);

    float computeScoreOfIR(string keyword, int graphIndex);

    float computeScoreOfIR(string keyword, unordered_map<int, unordered_set<int>> graph);

    float computeScoreOfIR(unordered_set<string> keywordSet, int graphIndex);

    float computeScoreOfIR(unordered_set<string> keywordSet, unordered_map<int, unordered_set<int>> graph);

    //Structural Comopactness-based DB Ranking
    float computeSIM(int source, int target, int graphIndex);

    float computeSIM(int source, int target, unordered_map<int, unordered_set<int>> sg);

    vector<int> findAllPathLengthBetweenTwoNode(int source, int target,

                                                      unordered_map<int, unordered_set<int>> sg);

    void printPath(vector<int> path);

    bool isNodeNotInThePath(int node, vector<int> path);

    float computeSIM(string source, string target, unordered_map<int, unordered_set<int>> sg);

    float computeScoreOfDB(unordered_set<string> keywordSet, int graphIndex);

    float computeScoreOfDB(unordered_set<string> keywordSet, unordered_map<int, unordered_set<int>> graph);

    float computeFinalScore(unordered_set<string> keywordSet, unordered_map<int, unordered_set<int>> sg);

};





#endif //INC_3_IN_1_GR_H
