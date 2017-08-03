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
    graph.readNode("/mnt/hd1/home/wushuang/testNode.csv");
    graph.readEdge("/mnt/hd1/home/wushuang/testEdge.csv");


    //graph.printGraph();

//    unordered_map<int, unordered_set<int>> rRadiusGraph = graph.getRRadiusGraph(1, 2);
//    graph.printMap(rRadiusGraph);

//    set<int> nr = graph.getRRadiusNodes(8,3);
//    graph.printSet(nr);

    vector<unordered_map<int,unordered_set<int>>>  test = graph.getAllRRadiusGraph(2);
    for (const auto& elem: test) {
        cout<<"========="<<endl;
        graph.printMap(elem);
    }
    unsigned long long end = getTime();

    cout<<"The program uses "<<getInterval(start, end)<<"ms";
    return 0;
}