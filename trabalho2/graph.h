#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class node {
public:
    string substantivo;

    int size();
    char operator[](int k);
    bool operator==(string S);
};

class arc {
public:
    
    string verbo;
    int from;
    int to;
};

class graph {
public:
    vector<node> nd;
    vector< vector<arc> > a;

    int size(){
        return this->nd.size();
    }

    bool nodeIsIn(string S);
    void nodeAppend(string S);
    void arcAppend(string S1, string V, string S2);

    void printRelations(string S);
    void printSubs();

    void load(ifstream& F);

    // Path finding algorithms
    vector<int> bfs(string start, string end);
    vector<int> dijkstra(string start, string end);
    
    // Hierarchical path finding algorithms
    vector<int> hierarchicalBFS(string start, string end);
    vector<int> hierarchicalDijkstra(string start, string end);
    
    // Helper methods
    int getNodeIndex(string S);
    void printPath(const vector<int>& path);
    bool isHierarchicalVerb(const string& verb);
};

