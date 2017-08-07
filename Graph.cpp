//
// Created by wushuang on 7/27/17.
//


//#include <tclDecls.h>



#include "Graph.h"

using namespace std;


Graph::Graph() {

}

Graph::~Graph(){

}


void Graph::addEdge(int src, int dest) {

    //complete adjList data structure
    adjList[src].insert(dest);

}

void Graph::printGraph() {
    int v;
    for (v = 0; v < size; ++v) {
        unordered_set<int> pCrawl = adjList[v];
        cout<<v;
        for (const auto& elem: pCrawl) {
            cout<<"->"<<elem;
        }
        cout<<endl;
    }
}

int Graph::readNode(string nodeFilePath){
    int size = 0;
    string data;
    ifstream myfile(nodeFilePath);

    while (getline(myfile, data)) {
        nodeContent.push_back(data);
        ++size;
    }

    this->size = size;

    //initialize adjList data structure
    adjList = new unordered_set<int> [size];
    for (int i = 0; i < size; ++i) {
        unordered_set<int> us;
        adjList[i] = us;
    }

}

void Graph::readEdge(string edgeFilePath){
    string data;
    ifstream myfile(edgeFilePath);
    while (getline(myfile, data)) {
//        int split1 = data.find(",");
//        int firstNode = stoi(data.substr(0,split1));
//        int split2 = data.substr(split1+1,data.length()).find(",") + split1;
//        int secondNode = stoi(data.substr(split1+1,split2+1));
//        addEdge(firstNode, secondNode);
        int split = data.find(",");
        int firstNode = stoi(data.substr(0,split));
        int secondNode = stoi(data.substr(split+1,data.length()));
        addEdge(firstNode, secondNode);
    }
}

unordered_map<int, unordered_set<int>> Graph::getMaximalRRadiusGraph(int nodeID, int radius) {

    unordered_map<int, unordered_set<int>> rRadiusGraph;

    vector<int> visited(size);

    list<int> queue;
    queue.push_back(nodeID);

    //BFS depth
    int r = 0;

    //counter to control the BFS depth
    int counter1 = 1, counter2 = 0;


    while(!queue.empty() && r < radius) {
        int curNode = queue.front();
        queue.pop_front();
        visited[curNode] = 1;


        for (const auto& elem: adjList[curNode]) {
            if(visited[elem] == 0) {
                visited[elem] = 1;
                queue.push_back(elem);
                counter2++;
            }

            //insert nodeID into rRadius graph
            auto us = rRadiusGraph.find(curNode);
            if(us == rRadiusGraph.end()) {
                unordered_set<int> neighbor;
                neighbor.insert(elem);
                rRadiusGraph.insert({curNode, neighbor});
            } else {
                us->second.insert(elem);
            }

            //deal with special case when at the last level
            if(r == (radius - 1)) {
                if (adjList[elem].find(curNode) != adjList[elem].end()) {
                    unordered_set<int> neighbor;
                    neighbor.insert(curNode);
                    rRadiusGraph.insert({elem, neighbor});
                }
            }

        }

        //set the counter to control the current depth
        counter1--;
        if(counter1 == 0) {
            r++;
            counter1 = counter2;
            counter2 = 0;
        }
    }

    //LEMMA 1: check whether it's a r-radius graph or not

    //get all the node in the r-radius graph
    set<int> keys;
    for (const auto& elem: rRadiusGraph) {
        keys.insert(elem.first);
    }

    //for every node k in the graph
    //if the r neighbor set of k is not the subset of the r-1 neighbor set of curNode
    //then it is a r-radius graph; otherwise just return empty graph
    for (const auto& elem: keys) {
        //all the node that is less than radius-1 away from elem node
        set<int> nodes = getRRadiusNodes(elem, radius - 1);

        if(includes(nodes.begin(), nodes.end(), keys.begin(), keys.end())) {
            unordered_map<int, unordered_set<int>> emptyGraph;
            return emptyGraph;
        }
    }

    //LEMMA 2: find maximal r-radius graph

    for (const auto& elem: keys) {
        //all the node that is less than radius-1 away from elem node
        set<int> nodes = getRRadiusNodes(elem, radius);

        if(keys != nodes) {
            if(!includes(nodes.begin(), nodes.end(), keys.begin(), keys.end())) {
                continue;
            } else {
                unordered_map<int, unordered_set<int>> emptyGraph;
                return emptyGraph;
            }
        }
    }

    //update the graphIndexTable
    int curGraphIndex = graphIndexTable.size();
    graphIndexTable.insert({curGraphIndex, rRadiusGraph});

    //update the keyword-graph table
    for (const auto& elem: keys) {
        string contentOfNode = nodeContent[elem];
        tokenizer<> tok(contentOfNode);
        for(tokenizer<>::iterator it=tok.begin(); it!=tok.end();++it) {
            auto kGT= keywordGraphTable.find(*it);
            if(kGT == keywordGraphTable.end()) {
                unordered_set<int> graphIndexSet;
                graphIndexSet.insert(curGraphIndex);
                keywordGraphTable.insert({*it,graphIndexSet});
            } else {
                kGT->second.insert(curGraphIndex);
            }
        }
    }

    return rRadiusGraph;
}

set<int> Graph::getRRadiusNodes(int nodeID, int radius) {
    set<int> result;
    result.insert(nodeID);

    vector<int> visited(size);

    list<int> queue;
    queue.push_back(nodeID);

    //BFS depth
    int r = 0;

    //counter to control the BFS depth
    int counter1 = 1, counter2 = 0;

    while(!queue.empty() && r < radius) {
        int curNode = queue.front();
        queue.pop_front();
        visited[curNode] = 1;


        for (const auto& elem: adjList[curNode]) {
            if(visited[elem] == 0) {
                visited[elem] = 1;
                queue.push_back(elem);
                counter2++;
            }

            result.insert(elem);
        }

        //set the counter to control the current depth
        counter1--;
        if(counter1 == 0) {
            r++;
            counter1 = counter2;
            counter2 = 0;
        }
    }
    return result;

}

void Graph::printSet(set<int> s) {
    for (const auto& elem: s) {
        cout<<elem<<" ";
    }
    cout<<endl;
}


void Graph::printMap(unordered_map<int, unordered_set<int>> um) {
    for (const auto& elem: um) {
        cout<<elem.first;
        for (const auto& node: elem.second){
            cout<<"->"<<node;
        }
        cout<<endl;
    }
}


vector<unordered_map<int,unordered_set<int>>> Graph::getAllMaximalRRadiusGraph(int radius) {
    vector<unordered_map<int,unordered_set<int>>> rRadiusGraphs;
    for(int i=0; i < size; ++i) {
        unordered_map<int,unordered_set<int>> graph = getMaximalRRadiusGraph(i, radius);
        if(graph.size()!=0) {
            rRadiusGraphs.push_back(graph);
        }
    }
    return rRadiusGraphs;
}

void Graph::printNodeContent() {
    int i = 0;
    for(const auto& elem: nodeContent) {
        cout<<"The "<<i<<"th node content is: "<<elem<<endl;
        ++i;
    }
    cout<<endl;
}

void Graph::printKeywordGraphTable() {
    for(const auto& elem: keywordGraphTable) {
        cout<<elem.first<<": ";
        for(const auto& elem2: elem.second) {
            cout<<elem2<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void Graph::printGraphIndexTable() {
    for(const auto& elem: graphIndexTable) {
        cout<<elem.first<<":\n";
        printMap(elem.second);
        cout<<endl;
    }
    cout<<endl;
}