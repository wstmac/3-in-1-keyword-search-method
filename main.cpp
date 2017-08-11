#include <fstream>
#include "Graph.h"
#include <sys/time.h>



inline
unsigned long long getTime() {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    unsigned long long ret = tv.tv_usec;

    /* Adds the seconds after converting them to microseconds (10^-6) */
    ret += (tv.tv_sec * 1000 * 1000);

    return ret;
};


inline
double getInterval(unsigned long long start, unsigned long long end) {
    return (double) (end - start) / 1000.0;
};




int main() {

    unsigned long long start = getTime();


    Graph graph;


    unsigned long long end1 = getTime();

    //graph.readNode("/mnt/hd1/home/wushuang/node.csv");
    graph.readNode("/mnt/hd1/home/wushuang/testNode.csv");


    unsigned long long end2 = getTime();

    cout<<"It uses "<<getInterval(end1,end2)<<"ms to read node"<<endl;

//    graph.readEdge("/mnt/hd1/home/wushuang/edge.csv");
    graph.readEdge("/mnt/hd1/home/wushuang/testEdge.csv");


    //graph.printGraph();




    unsigned long long end3 = getTime();
    cout<<"It uses "<<getInterval(end2,end3)<<"ms to read edge"<<endl;
//
//
//    vector<unordered_map<int,unordered_set<int>>> arr;
//    for(int i=3000; i<4000; i++){
//        unordered_map<int,unordered_set<int>>  test = graph.getMaximalRRadiusGraph(i,2);
//        arr.push_back(test);
//    }
//
//
//    unsigned long long end4 = getTime();
//    cout<<"It uses "<<getInterval(end3,end4)<<"ms to get maximal r-graph 3000-4000"<<endl;
//
//
//
//    int i = 0;
//    for (const auto& elem: arr) {
//        if(elem.size() != 0) {
//            i++;
//        }
//        //graph.printMap(elem);
//    }
//    cout<<"There are "<<i<<" graph returns"<<endl;

//    unordered_map<int, unordered_set<int>> rRadiusMaximalGraph = graph.getMaximalRRadiusGraph(9, 2);
//    graph.printMap(rRadiusMaximalGraph);
//
//    unordered_set<int> contentNode;
//    contentNode.insert(0);
//    contentNode.insert(1);
//    contentNode.insert(6);
//
//    unordered_map<int, unordered_set<int>> steinerGraph = graph.getRRadiusSteinerGraph(rRadiusMaximalGraph, contentNode);
//    cout<<"\nSteiner Graph:\n";
//
//    graph.printMap(steinerGraph);
//
//
//    unsigned long long end = getTime();
//
//
//    cout<<"The program uses "<<getInterval(start, end)<<"ms"<<endl;


//    vector<unordered_map<int, unordered_set<int>>> result = graph.getAllMaximalRRadiusGraph(2);
//    for(const auto& elem: result) {
//        graph.printMap(elem);
//        cout<<endl;
//    }
//
//    graph.printNodeContent();
//
//    graph.printKeywordNodeTable();
//
//    graph.printKeywordGraphTable();
//
//    graph.printGraphIndexTable();
//
//    graph.printGraphIndexTermNumberTable();
//
//    graph.printGraph();

    //test searchRRadiusMaximalGraph function
//    unordered_set<string> keywords = graph.extractKeywords("DISCOVER, Relational, Databases, Papakonstantinou");
//    set<int> answer = graph.searchRRadiusMaximalGraph(keywords);
//
//    cout<<"\nGraph get by keywords\n";
//    for(const auto& elem: answer) {
//        auto g = graph.getGraphByGraphIndex(elem);
//        graph.printMap(g);
//        cout<<endl;
//    }


    //test searchRRadiusSteinerGraph function
//    vector<unordered_map<int, unordered_set<int>>> answer = graph.searchRRadiusSteinerGraph("Shanmugasundaram, Guo, XRANK");
//    cout<<"\nSteiner Graph get by keywords\n";
//    for(const auto& elem: answer) {
//        graph.printMap(elem);
//        cout<<endl;
//    }

    //test computeScoreOfIR function
//    graph.getAllMaximalRRadiusGraph(2);
//    unordered_set<string> keywordSet = graph.extractKeywords("DISCOVER, Relational, Databases, Papakonstantinou");
//    set<int> answer = graph.searchRRadiusMaximalGraph(keywordSet);
//
//    cout<<"\nGraph get by keywords\n";
//    for(const auto& elem: answer) {
//        auto g = graph.getGraphByGraphIndex(elem);
//        graph.printMap(g);
//
//        cout<<"Score Of IR by graph index "<<elem<<" is "<< graph.computeScoreOfIR(keywordSet, g);
//        cout<<endl;
//    }


    //test computeScoreOfDB function
//    vector<unordered_map<int, unordered_set<int>>> result = graph.getAllMaximalRRadiusGraph(2);
//    graph.printMap(result[3]);
//    cout<<endl;
//    unordered_set<string> keywordSet = graph.extractKeywords("DISCOVER, Relational, Databases, Papakonstantinou");
//    cout<<"\n"<<graph.computeScoreOfDB(keywordSet, result[3]);




    //test computeFinalScore
//    vector<unordered_map<int, unordered_set<int>>> result = graph.getAllMaximalRRadiusGraph(2);
//    graph.printMap(result[3]);
//    cout<<endl;
//    unordered_set<string> keywordSet = graph.extractKeywords("DISCOVER, Relational, Databases, Papakonstantinou");
//    cout<<"\n"<<graph.computeFinalScore(keywordSet, result[3]);



    //debug
    graph.getAllMaximalRRadiusGraph(2);

    unordered_set<string> keywordSet = graph.extractKeywords("Relational, Databases");
    set<int> rRadiusMaximalGraphIndexSet = graph.searchRRadiusMaximalGraph(keywordSet);
    for(const auto& elem : rRadiusMaximalGraphIndexSet) {
        graph.printMap(graph.getGraphByGraphIndex(elem));
        cout<<endl;
    }

    cout<<"steiner graph"<<endl;
    auto result = graph.searchRRadiusSteinerGraph("Relational, Databases");
    for(const auto& elem : result) {
        graph.printMap(elem);
        cout<<endl;
    }
    cout<<endl;
    float scoreOfIR1 = graph.computeScoreOfIR("Relational",graph.getGraphByGraphIndex(1));
    float scoreOfIR2 = graph.computeScoreOfIR("Databases",graph.getGraphByGraphIndex(1));
    float scoreOfDB = graph.computeSIM("Relational","Databases",result[0]);

    cout<<"\n"<<scoreOfDB * (scoreOfIR1 + scoreOfIR2)<<endl;

    graph.printMap(graph.getGraphByGraphIndex(1));

    return 0;
}

