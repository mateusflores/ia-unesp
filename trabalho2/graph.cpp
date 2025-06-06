#include "graph.h"
#include <fstream>
#include <queue>
#include <limits>

using namespace std;
/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------
    Funções de nó
--------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

int node::size(){
    return this->substantivo.size();
}
char node::operator[](int k){
    return this->substantivo[k];
}
bool node::operator==(string S){
    if (this->size()!=S.size())
        return false;
    for (int i=0; i<this->size(); i++){
        if (this->substantivo[i]!=S[i])
            return false;
    }
    return true;
}

/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------
    Funções de grafo
--------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

bool graph::nodeIsIn(string S){
    /*
        Checa se existe algum nó com a string S.
    */
    for (int i=0; i<this->nd.size(); i++){
        if (this->nd[i]==S)
            return true;
    }
    return false;
}

void graph::nodeAppend(string S){
    /*
        Anexa novo nó, caso ainda não tenha sido adicionado.
    */
    if (!this->nodeIsIn(S)){
        node* n = new node;
        n->substantivo = S;
        this->nd.push_back(*n);
        vector<arc> v_arc;
        this->a.push_back(v_arc);
    }
}

void graph::arcAppend(string S1, string V, string S2){
    /*
        Insere novo arco.
        S1 e S2 são os substantivos envolvidos e V é o verbo.
    */
    arc* new_arc = new arc;
    new_arc->verbo = V;
    int pos = 0;
    for (int k=0; k<this->size(); k++){
        if (this->nd[k]==S1){
            pos = k;
            new_arc->from = k;
        }
        if (this->nd[k]==S2){
            new_arc->to = k;
        }
    }
    this->a[pos].push_back(*new_arc);
}

void graph::printRelations(string S){
    /*
        Imprime as relações que partem da string S.
    */
    for (int k=0; k<this->size(); k++){
        if (this->nd[k]==S){
            cout << "Relações para " << this->nd[k].substantivo << ":" << endl;
            cout << this->a[k].size() << endl;
            for (int p=0; p<this->a[k].size(); p++){
                cout << this->nd[this->a[k][p].from].substantivo << " ";
                cout << this->a[k][p].verbo << " ";
                cout << this->nd[this->a[k][p].to].substantivo << endl;
            }
            return;
        }
    }
    cout << "String não encontrada!";
}

void graph::printSubs(){
    /*
        Imprime substantivos.
    */
    for (int k=0; k<this->size(); k++){
        cout << this->nd[k].substantivo << endl;
    }
}

void graph::load(ifstream& F){
    /*
        Carrega uma base de dados para um grafo.
    */
    while (!F.eof()){
        string S1;
        getline(F,S1,' ');
        string V;
        getline(F,V,' ');
        string S2;
        getline(F,S2);
        this->nodeAppend(S1);
        this->nodeAppend(S2);
        this->arcAppend(S1,V,S2);
    }
}

int graph::getNodeIndex(string S) {
    for (int i = 0; i < nd.size(); i++) {
        if (nd[i] == S) return i;
    }
    return -1;
}

void graph::printPath(const vector<int>& path) {
    if (path.empty()) {
        cout << "No path exists!" << endl;
        return;
    }
    
    for (int i = 0; i < path.size() - 1; i++) {
        cout << nd[path[i]].substantivo << " -> ";
    }
    cout << nd[path.back()].substantivo << endl;
}

bool graph::isHierarchicalVerb(const string& verb) {
    return verb == "é";
}

vector<int> graph::bfs(string start, string end) {
    int start_idx = getNodeIndex(start);
    int end_idx = getNodeIndex(end);
    
    if (start_idx == -1 || end_idx == -1) return vector<int>();
    
    vector<bool> visited(nd.size(), false);
    vector<int> parent(nd.size(), -1);
    queue<int> q;
    
    q.push(start_idx);
    visited[start_idx] = true;
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        if (current == end_idx) break;
        
        // Check outgoing edges
        for (const arc& edge : a[current]) {
            if (!visited[edge.to]) {
                visited[edge.to] = true;
                parent[edge.to] = current;
                q.push(edge.to);
            }
        }
        
        // Check incoming edges by scanning all nodes
        for (int i = 0; i < nd.size(); i++) {
            for (const arc& edge : a[i]) {
                if (edge.to == current && !visited[i]) {
                    visited[i] = true;
                    parent[i] = current;
                    q.push(i);
                }
            }
        }
    }
    
    if (parent[end_idx] == -1) return vector<int>();
    
    vector<int> path;
    for (int v = end_idx; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<int> graph::dijkstra(string start, string end) {
    int start_idx = getNodeIndex(start);
    int end_idx = getNodeIndex(end);
    
    if (start_idx == -1 || end_idx == -1) return vector<int>();
    
    vector<int> dist(nd.size(), numeric_limits<int>::max());
    vector<int> parent(nd.size(), -1);
    vector<bool> visited(nd.size(), false);
    
    dist[start_idx] = 0;
    
    for (int i = 0; i < nd.size(); i++) {
        int min_dist = numeric_limits<int>::max();
        int u = -1;
        
        for (int j = 0; j < nd.size(); j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }
        
        if (u == -1) break;
        visited[u] = true;
        
        // Check outgoing edges
        for (const arc& edge : a[u]) {
            int v = edge.to;
            if (!visited[v] && dist[u] != numeric_limits<int>::max() && dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
            }
        }
        
        // Check incoming edges
        for (int j = 0; j < nd.size(); j++) {
            for (const arc& edge : a[j]) {
                if (edge.to == u && !visited[j] && dist[u] != numeric_limits<int>::max() && dist[u] + 1 < dist[j]) {
                    dist[j] = dist[u] + 1;
                    parent[j] = u;
                }
            }
        }
    }
    
    if (dist[end_idx] == numeric_limits<int>::max()) return vector<int>();
    
    vector<int> path;
    for (int v = end_idx; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<int> graph::hierarchicalBFS(string start, string end) {
    int start_idx = getNodeIndex(start);
    int end_idx = getNodeIndex(end);
    
    if (start_idx == -1 || end_idx == -1) return vector<int>();
    
    vector<bool> visited(nd.size(), false);
    vector<int> parent(nd.size(), -1);
    queue<int> q;
    
    q.push(start_idx);
    visited[start_idx] = true;
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        if (current == end_idx) break;
        
        for (const arc& edge : a[current]) {
            if (!visited[edge.to] && isHierarchicalVerb(edge.verbo)) {
                visited[edge.to] = true;
                parent[edge.to] = current;
                q.push(edge.to);
            }
        }
    }
    
    if (parent[end_idx] == -1) return vector<int>();
    
    vector<int> path;
    for (int v = end_idx; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<int> graph::hierarchicalDijkstra(string start, string end) {
    int start_idx = getNodeIndex(start);
    int end_idx = getNodeIndex(end);
    
    if (start_idx == -1 || end_idx == -1) return vector<int>();
    
    vector<int> dist(nd.size(), numeric_limits<int>::max());
    vector<int> parent(nd.size(), -1);
    vector<bool> visited(nd.size(), false);
    
    dist[start_idx] = 0;
    
    for (int i = 0; i < nd.size(); i++) {
        int min_dist = numeric_limits<int>::max();
        int u = -1;
        
        for (int j = 0; j < nd.size(); j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }
        
        if (u == -1) break;
        visited[u] = true;
        
        for (const arc& edge : a[u]) {
            if (isHierarchicalVerb(edge.verbo)) {
                int v = edge.to;
                if (!visited[v] && dist[u] != numeric_limits<int>::max() && dist[u] + 1 < dist[v]) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                }
            }
        }
    }
    
    if (dist[end_idx] == numeric_limits<int>::max()) return vector<int>();
    
    vector<int> path;
    for (int v = end_idx; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}