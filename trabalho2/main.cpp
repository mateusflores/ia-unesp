#include "graph.h"
#include <iostream>
#include <set>
#include <fstream>
#include <string>

void printUsage(const char* programName) {
    cout << "Usage: " << programName << " <dataset_file> <algorithm>" << endl;
    cout << "Available dataset files:" << endl;
    cout << "  data_10.txt  - 10 edges" << endl;
    cout << "  data_20.txt  - 20 edges" << endl;
    cout << "  data_30.txt  - 30 edges" << endl;
    cout << "  data_40.txt  - 40 edges" << endl;
    cout << "  data_50.txt  - 50 edges" << endl;
    cout << "\nAvailable algorithms:" << endl;
    cout << "  bfs      - Breadth-First Search" << endl;
    cout << "  dijkstra - Dijkstra's Algorithm" << endl;
    cout << "\nResults will be saved to: results_<dataset>_<algorithm>.txt" << endl;
}

void exportHierarchicalRelationships(graph& G, const string& algorithm, const string& dataFile) {
    // Create output filename based on input file and algorithm
    string outputFile = "results_" + dataFile.substr(0, dataFile.find_last_of('.')) + "_" + algorithm + ".txt";
    ofstream outFile(outputFile);
    
    if (!outFile.is_open()) {
        cout << "Error: Could not create output file: " << outputFile << endl;
        return;
    }
    
    outFile << "Hierarchical Relationships (First -> Last) using " << algorithm << "\n";
    outFile << "==========================================================\n\n";
    
    // Store unique relationships to avoid duplicates
    set<pair<string, string>> relationships;
    
    // For each pair of nodes
    for (int i = 0; i < G.nd.size(); i++) {
        for (int j = 0; j < G.nd.size(); j++) {
            if (i != j) {  // Don't check path to itself
                string start = G.nd[i].substantivo;
                string end = G.nd[j].substantivo;
                
                vector<int> path;
                if (algorithm == "bfs") {
                    path = G.hierarchicalBFS(start, end);
                } else {
                    path = G.hierarchicalDijkstra(start, end);
                }
                
                if (!path.empty()) {
                    relationships.insert({start, end});
                }
            }
        }
    }
    
    // Write all unique relationships to file
    for (const auto& rel : relationships) {
        outFile << rel.first << " é " << rel.second << "\n";
    }
    
    outFile << "\nTotal number of hierarchical relationships: " << relationships.size() << "\n";
    outFile.close();
    
    cout << "Results have been saved to: " << outputFile << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    string dataFile = argv[1];
    string algorithm = argv[2];

    // Validate algorithm choice
    if (algorithm != "bfs" && algorithm != "dijkstra") {
        cout << "Invalid algorithm. Please use 'bfs' or 'dijkstra'" << endl;
        printUsage(argv[0]);
        return 1;
    }

    ifstream F(dataFile);
    if (!F.is_open()) {
        cout << "Could not open data file: " << dataFile << endl;
        printUsage(argv[0]);
        return 1;
    }

    graph G;
    G.load(F);
    F.close();

    cout << "Loaded graph from " << dataFile << " with " << G.size() << " nodes" << endl;
    exportHierarchicalRelationships(G, algorithm, dataFile);

    return 0;
}