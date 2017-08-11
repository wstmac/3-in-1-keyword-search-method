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

string Graph::getNodeContentByNodeIndex(int nodeIndex) {
    return nodeContent[nodeIndex].first;
}

int Graph::getNodeTermNumberByNodeIndex(int nodeIndex) {
    return nodeContent[nodeIndex].second;
}

set<int> Graph::getNodeSetByKeyword(string keyword) {
    return keywordNodeTable.find(keyword)->second;
}

set<int> Graph::getGraphIndexByKeyword(string keyword) {
    return keywordGraphTable.find(keyword)->second;
}

unordered_map<int, unordered_set<int>> Graph::getGraphByGraphIndex(int graphIndex) {
    return graphIndexTable.find(graphIndex)->second;
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

        int noTermInNode = 0;

        //update the keywordNodeTable
        tokenizer<> tok(data);
        for(tokenizer<>::iterator it=tok.begin(); it!=tok.end();++it) {
            ++noTermInNode;
            auto kNT= keywordNodeTable.find(*it);
            if(kNT == keywordNodeTable.end()) {
                set<int> nodeIndexSet;
                nodeIndexSet.insert(size);
                keywordNodeTable.insert({*it,nodeIndexSet});
            } else {
                kNT->second.insert(size);
            }
        }

        pair<string, int> curNode = make_pair(data, noTermInNode);
        nodeContent.push_back(curNode);

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

    queue<int> queue;
    queue.push(nodeID);

    //BFS depth
    int r = 0;

    //counter to control the BFS depth
    int counter1 = 1, counter2 = 0;


    while(!queue.empty() && r < radius) {
        int curNode = queue.front();
        queue.pop();
        visited[curNode] = 1;


        for (const auto& elem: adjList[curNode]) {
            if(visited[elem] == 0) {
                visited[elem] = 1;
                queue.push(elem);
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

    int totalTermInGraph = 0;

    for (const auto& elem: keys) {
        //update the keyword-graph table
        string contentOfNode = nodeContent[elem].first;
        tokenizer<> tok(contentOfNode);
        for(tokenizer<>::iterator it=tok.begin(); it!=tok.end();++it) {
            auto kGT= keywordGraphTable.find(*it);
            if(kGT == keywordGraphTable.end()) {
                set<int> graphIndexSet;
                graphIndexSet.insert(curGraphIndex);
                keywordGraphTable.insert({*it,graphIndexSet});
            } else {
                kGT->second.insert(curGraphIndex);
            }
        }

        //update graphIndexTermNumberTable
        totalTermInGraph += nodeContent[elem].second;
    }
    graphIndexTermNumberTable.insert({curGraphIndex, totalTermInGraph});

    return rRadiusGraph;
}

set<int> Graph::getRRadiusNodes(int nodeID, int radius) {
    set<int> result;
    result.insert(nodeID);

    vector<int> visited(size);

    queue<int> queue;
    queue.push(nodeID);

    //BFS depth
    int r = 0;

    //counter to control the BFS depth
    int counter1 = 1, counter2 = 0;

    while(!queue.empty() && r < radius) {
        int curNode = queue.front();
        queue.pop();
        visited[curNode] = 1;


        for (const auto& elem: adjList[curNode]) {
            if(visited[elem] == 0) {
                visited[elem] = 1;
                queue.push(elem);
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

    cout<<"Node Content Table:"<<endl;

    int i = 0;
    for(const auto& elem: nodeContent) {
        cout<<"The "<<i<<"th node content is: "<<elem.first<<" ;\nThere are "<<elem.second<<" term in the node"<<endl;
        ++i;
    }
    cout<<endl;
}

void Graph::printKeywordNodeTable() {

    cout<<"Keyword Node Table:"<<endl;

    for(const auto& elem: keywordNodeTable) {
        cout<<elem.first<<": ";
        for(const auto& elem2: elem.second) {
            cout<<elem2<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void Graph::printKeywordGraphTable() {

    cout<<"Keyword Graph Table:"<<endl;

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

    cout<<"Graph Index Table:"<<endl;

    for(const auto& elem: graphIndexTable) {
        cout<<elem.first<<":\n";
        printMap(elem.second);
        cout<<endl;
    }
    cout<<endl;
}

void Graph::printGraphIndexTermNumberTable() {

    cout<<"Graph Index Term Number Table:"<<endl;

    for(const auto& elem: graphIndexTermNumberTable) {
        cout<<elem.first<<"   :"<<elem.second<<endl;
    }
    cout<<endl;
}

unordered_map<int, unordered_set<int>>
Graph::getRRadiusSteinerGraph(unordered_map<int, unordered_set<int>> rRadiusMaximalGraph, unordered_set<int> contentNodeSet) {

    int numOfContentNode = contentNodeSet.size();

    set<int> finalPSet;

    set<int> orderedContentNodeSet = convertUsToOs(contentNodeSet);

    vector<set<int>> pSet(numOfContentNode);
    int pointer = 0;

    //doing step1 of getting steiner graph
    for(const auto& elem: contentNodeSet) {
        auto temp = contentNodeSet;
        temp.erase(elem);
        pSet[pointer] = getNodeSetP(rRadiusMaximalGraph, temp, elem);
        ++pointer;
    }

    //doing step2 of getting steiner graph
    for(int i = 0; i < numOfContentNode; ++i) {
        for(int j = i + 1; j < numOfContentNode; ++j) {
            set<int> intersect;
            set_intersection(pSet[i].begin(), pSet[i].end(), pSet[j].begin(), pSet[j].end(), inserter(intersect, intersect.begin()));
            set<int> un;
            set_union(intersect.begin(), intersect.end(), finalPSet.begin(), finalPSet.end(), inserter(un, un.begin()));
            finalPSet = un;
        }
    }

    //union with content node
    set<int> un;
    set_union(orderedContentNodeSet.begin(), orderedContentNodeSet.end(), finalPSet.begin(), finalPSet.end(), inserter(un, un.begin()));
    finalPSet = un;

    //get deleted node list
    set<int> deletedNode;
    set<int> allNode;
    for(const auto& elem: rRadiusMaximalGraph) {
        allNode.insert(elem.first);
    }
    set_difference(allNode.begin(), allNode.end(), finalPSet.begin(), finalPSet.end(), inserter(deletedNode, deletedNode.begin()));

    //get final steiner graph
    for(auto cit = rRadiusMaximalGraph.begin(); cit != rRadiusMaximalGraph.end() ;) {

        int key = cit->first;

        auto it = deletedNode.find(key);

        if(it == deletedNode.end()) {
            for(const auto& nodeIndex: deletedNode) {
                cit->second.erase(nodeIndex);
            }
            ++cit;
        } else {
            rRadiusMaximalGraph.erase(cit++);
        }
    }

    return rRadiusMaximalGraph;

}

unordered_map<int, unordered_set<int>>
Graph::getRRadiusSteinerGraph(int rRadiusMaximalGraphIndex, unordered_set<int> contentNodeSet) {
    unordered_map<int, unordered_set<int>> rRadiusMaximalGraph = graphIndexTable.find(rRadiusMaximalGraphIndex)->second;
    return getRRadiusSteinerGraph(rRadiusMaximalGraph, contentNodeSet);
}

set<int> Graph::getNodeSetP(unordered_map<int, unordered_set<int>> rRadiusMaximalGraph, unordered_set<int> contentNodeSet, int nodeIndex) {


    set<int> result;

    //remove content nodes and their corresponding edges
    for(auto cit = rRadiusMaximalGraph.begin(); cit != rRadiusMaximalGraph.end();) {

        int key = cit->first;


        auto it = contentNodeSet.find(key);


        if(it == contentNodeSet.end()) {
            for(const auto& nodeIndex: contentNodeSet) {
                cit->second.erase(nodeIndex);
            }
            ++cit;
        } else {
            rRadiusMaximalGraph.erase(cit++);
        }
    }


    //Run BFS to get the nodes which connect to content node ci


    queue<int> queue;
    queue.push(nodeIndex);


    while(!queue.empty()) {
        int curNode = queue.front();
        result.insert(curNode);
        queue.pop();

        for (const auto& elem: rRadiusMaximalGraph.find(curNode)->second) {
            //if the node is not visited curNode before
            if(result.find(elem) == result.end()) {
                queue.push(elem);
            }
        }
    }

    return result;
}

set<int> Graph::convertUsToOs(unordered_set<int>& us) {
    set<int> os;
    for(const auto& elem: us) {
        os.insert(elem);
    }
    return os;
}



unordered_set<string> Graph::extractKeywords(string input) {
    unordered_set<string> keywords;
    tokenizer<> tok(input);
    for(tokenizer<>::iterator it=tok.begin(); it!=tok.end();++it) {
        keywords.insert(*it);
    }
    return keywords;
}

set<int> Graph::searchRRadiusMaximalGraph(unordered_set<string> keywordSet) {
    set<int> rRadiusMaximalGraphSet;
    int keywordSetSize = keywordSet.size();
    vector<set<int>> graphIndexSets(keywordSetSize);
    int pointer = 0;

    for(const auto& elem: keywordSet) {
        if(keywordGraphTable.find(elem) != keywordGraphTable.end()) {
            graphIndexSets[pointer] = keywordGraphTable.find(elem)->second;
            ++pointer;
        }
    }

    rRadiusMaximalGraphSet = graphIndexSets[0];

    for(int i=1; i<keywordSetSize; ++i) {
        set<int> intersect;
        set_intersection(rRadiusMaximalGraphSet.begin(), rRadiusMaximalGraphSet.end(), graphIndexSets[i].begin(), graphIndexSets[i].end(), inserter(intersect, intersect.begin()));
        rRadiusMaximalGraphSet = intersect;
    }

    return rRadiusMaximalGraphSet;

}

vector<unordered_map<int, unordered_set<int>>> Graph::searchRRadiusSteinerGraph(string input) {

    vector<unordered_map<int, unordered_set<int>>> steinerGraphSet;

    unordered_set<string> keywordSet = extractKeywords(input);
    set<int> rRadiusMaximalGraphIndexSet = searchRRadiusMaximalGraph(keywordSet);

    for(const auto& elem: rRadiusMaximalGraphIndexSet) {
        unordered_set<int> contentNodeSet = findContentNodeInGraph(elem, keywordSet);
        unordered_map<int, unordered_set<int>> steinerGraph = getRRadiusSteinerGraph(elem, contentNodeSet);
        steinerGraphSet.push_back(steinerGraph);
    }

    return steinerGraphSet;
}

unordered_set<int> Graph::findContentNodeInGraph(int graphIndex, unordered_set<string> keywordSet) {

    unordered_set<int> contentNodeSet;

    unordered_map<int, unordered_set<int>> graph = getGraphByGraphIndex(graphIndex);

    for(const auto& elem: graph) {
        int curNodeIndex = elem.first;
        string curNodeContent = nodeContent[curNodeIndex].first;
        if(contains(curNodeContent, keywordSet)) {
            contentNodeSet.insert(curNodeIndex);
        }
    }

    return contentNodeSet;
}

bool Graph::contains(string data, unordered_set<string> keywordSet) {
    for(const auto& elem: keywordSet) {
        if(data.find(elem) != string::npos) {
            return true;
        }
    }
    return false;
}

float Graph::computeNTF(string keyword, int graphIndex) {

    unordered_map<int, unordered_set<int>> graph = getGraphByGraphIndex(graphIndex);
    return computeNTF(keyword, graph);

}

float Graph::computeNTF(string keyword, unordered_map<int, unordered_set<int>> graph) {

    int termFrequency = 0;
    for(const auto& elem: graph) {
        int nodeIndex = elem.first;
        string nodeContent = getNodeContentByNodeIndex(nodeIndex);
        tokenizer<> tok(nodeContent);
        for(tokenizer<>::iterator it=tok.begin(); it!=tok.end();++it) {
            if(*it == keyword) {
                ++termFrequency;
            }
        }
    }

    return (float) (1 + log(1 + log(1 + termFrequency)));
}

float Graph::computeIDF(string keyword) {

    int numOfMaximalRRadiusGraph = graphIndexTermNumberTable.size();
    int graphContainsKeyword = getGraphIndexByKeyword(keyword).size();

    return (float) log(1.0 * (numOfMaximalRRadiusGraph + 1) /( graphContainsKeyword + 1));
}

float Graph::computeNDL(int graphIndex) {
    unordered_map<int, unordered_set<int>> graph = getGraphByGraphIndex(graphIndex);
    return computeNDL(graph);
}

float Graph::computeNDL(unordered_map<int, unordered_set<int>> graph) {

    int totalTermInGraph = 0;
    for(const auto& elem: graph){
        int curNodeTermNumber = getNodeTermNumberByNodeIndex(elem.first);
        totalTermInGraph += curNodeTermNumber;
    }

    if(totalTerm == 0) {
        for(const auto& elem: graphIndexTermNumberTable){
            totalTerm += elem.second;
        }
    }

    float avgTermPerGraph = (float) (totalTerm / graphIndexTermNumberTable.size());

    return (float) (0.8 + 0.2 * totalTermInGraph / avgTermPerGraph);
}

float Graph::computeScoreOfIR(string keyword, int graphIndex) {
    float ntf = computeNTF(keyword, graphIndex);
    float idf = computeIDF(keyword);
    float ndl = computeNDL(graphIndex);

    return (float) (ntf * idf / ndl);
}

float Graph::computeScoreOfIR(string keyword, unordered_map<int, unordered_set<int>> graph) {

    float ntf = computeNTF(keyword, graph);
    float idf = computeIDF(keyword);
    float ndl = computeNDL(graph);

    return (float) (ntf * idf / ndl);
}

float Graph::computeScoreOfIR(unordered_set<string> keywordSet, int graphIndex) {

    float scoreOfIR = 0;

    for(const string& elem: keywordSet) {
        float temp = computeScoreOfIR(elem, graphIndex);
        scoreOfIR += temp;
    }

    return scoreOfIR;
}

float Graph::computeScoreOfIR(unordered_set<string> keywordSet, unordered_map<int, unordered_set<int>> graph) {

    float scoreOfIR = 0;

    for(const string& elem: keywordSet) {
        float temp = computeScoreOfIR(elem, graph);
        scoreOfIR += temp;
    }

    return scoreOfIR;
}

float Graph::computeSIM(int source, int target, int graphIndex) {
    unordered_map<int, unordered_set<int>> sg = getGraphByGraphIndex(graphIndex);
    return computeSIM(source, target, sg);
}

float Graph::computeSIM(int source, int target, unordered_map<int, unordered_set<int>> sg) {

    float sim = 0;

    vector<int> pathLengh = findAllPathLengthBetweenTwoNode(source, target, sg);
    for(const auto&elem: pathLengh) {
        sim += 1 / pow(elem , 2);
    }

    return sim;
}

vector<int>
Graph::findAllPathLengthBetweenTwoNode(int source, int target, unordered_map<int, unordered_set<int>> sg) {

    vector<int> pathLength;//store all the possible path length

    vector<int> path;
    path.push_back(source);

    queue<vector<int>> q;
    q.push(path);


    while(!q.empty()) {
        path=q.front();
        q.pop();

        int lastNodeOfPath=path[path.size()-1];
        if(lastNodeOfPath == target) {
            //cout<<"The Required path is:: ";
            //printPath(path);

            pathLength.push_back(path.size());

        } else {
            //printPath(path);
        }

        for(const auto& elem: sg.find(lastNodeOfPath)->second) {
            if(isNodeNotInThePath(elem, path)) {
                vector<int> new_path(path.begin(),path.end());
                new_path.push_back(elem);
                q.push(new_path);
            }
        }
    }
    return pathLength;
}

void Graph::printPath(vector<int> path) {
    cout<<"[ ";
    for(int i=0;i<path.size();++i)
    {
        cout<<path[i]<<" ";
    }
    cout<<"]"<<endl;
}

bool Graph::isNodeNotInThePath(int node, vector<int> path) {
    for(int i = 0; i < path.size(); ++i) {
        if(path[i] == node) {
            return false;
        }
    }
    return true;
}

float Graph::computeSIM(string source, string target, unordered_map<int, unordered_set<int>> sg) {

    float score = 0;

    //find node that contains these two keyword
    vector<int> sourceSet, targetSet;
    unordered_set<int> unionOfTwoSet;

    for(const auto& elem: sg) {
        int nodeIndex = elem.first;
        string nodeContent = getNodeContentByNodeIndex(nodeIndex);
        tokenizer<> tok(nodeContent);
        for(tokenizer<>::iterator it=tok.begin(); it!=tok.end();++it) {
            if(*it == source) {
                sourceSet.push_back(nodeIndex);
                unionOfTwoSet.insert(nodeIndex);
            }
            if(*it == target) {
                targetSet.push_back(nodeIndex);
                unionOfTwoSet.insert(nodeIndex);
            }
        }
    }

    //for each pair of node, compute the similarity
    for(int i = 0; i < sourceSet.size(); ++i) {
        for(int j = 0; j < targetSet.size(); ++j) {
            score += computeSIM(sourceSet[i], targetSet[j], sg);
        }
    }

    //compute the union of two vector
    return score / (unionOfTwoSet.size());
}

float Graph::computeScoreOfDB(unordered_set<string> keywordSet, int graphIndex) {
    unordered_map<int, unordered_set<int>> sg = getGraphByGraphIndex(graphIndex);
    return computeScoreOfDB(keywordSet, sg);
}

float Graph::computeScoreOfDB(unordered_set<string> keywordSet, unordered_map<int, unordered_set<int>> sg) {
    float scoreOfDB = 0;

    for(unordered_set<string>::iterator i = keywordSet.begin(); i != keywordSet.end(); ++i) {
        for(unordered_set<string>::iterator j = next(i,1); j != keywordSet.end(); ++j) {
            //cout<<"keywords are :"<<*i<<" and "<<*j<<endl;
            scoreOfDB += computeSIM(*i, *j, sg);
        }
    }

    return scoreOfDB;

}

float Graph::computeFinalScore(unordered_set<string> keywordSet, unordered_map<int, unordered_set<int>> sg) {
    float score = 0;

    for(unordered_set<string>::iterator i = keywordSet.begin(); i != keywordSet.end(); ++i) {
        for(unordered_set<string>::iterator j = next(i,1); j != keywordSet.end(); ++j) {
            float scoreOfIR1 = computeScoreOfIR(*i, sg);
            float scoreOfIR2 = computeScoreOfIR(*j, sg);
            float scoreOfDB = computeSIM(*i, *j, sg);
            score += scoreOfDB * (scoreOfIR1 + scoreOfIR2);
        }
    }
    return score;

}















