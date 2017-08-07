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

    vector<unordered_map<int, unordered_set<int>>> rRadiusMaximalGraph = graph.getAllMaximalRRadiusGraph(2);




    unsigned long long end = getTime();

    cout<<"It uses "<<getInterval(end3,end)<<"ms to print graph"<<endl;

    cout<<"The program uses "<<getInterval(start, end)<<"ms"<<endl;

    cout<<"Node Content Table:\n";
    graph.printNodeContent();

    cout<<"Ketword Graph Table\n";
    graph.printKeywordGraphTable();

    cout<<"Graph Index Table\n";
    graph.printGraphIndexTable();




    return 0;
}