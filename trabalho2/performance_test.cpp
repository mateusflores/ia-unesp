#include "graph.h"
#include <chrono>
#include <random>
#include <cmath>
#include <fstream>

using namespace std::chrono;

struct TestResult {
    double mean;
    double std_dev;
};

TestResult measurePerformance(graph& G, const vector<pair<string, string>>& queries,
                            vector<int> (graph::*algorithm)(string, string)) {
    vector<double> times;
    
    for (const auto& query : queries) {
        auto start = high_resolution_clock::now();
        (G.*algorithm)(query.first, query.second);
        auto end = high_resolution_clock::now();
        
        duration<double, std::milli> duration = end - start;
        times.push_back(duration.count());
    }
    
    // Calculate mean
    double sum = accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();
    
    // Calculate standard deviation
    double sq_sum = 0;
    for (double time : times) {
        sq_sum += (time - mean) * (time - mean);
    }
    double std_dev = sqrt(sq_sum / (times.size() - 1));
    
    return {mean, std_dev};
}

vector<pair<string, string>> generateRandomQueries(const graph& G, int count) {
    vector<pair<string, string>> queries;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, G.nd.size() - 1);
    
    for (int i = 0; i < count; i++) {
        int start_idx = dis(gen);
        int end_idx = dis(gen);
        queries.push_back({G.nd[start_idx].substantivo, G.nd[end_idx].substantivo});
    }
    
    return queries;
}

void runPerformanceTests(graph& G, ofstream& outFile) {
    const int NUM_QUERIES = 1000;
    vector<pair<string, string>> queries = generateRandomQueries(G, NUM_QUERIES);
    
    TestResult bfs_result = measurePerformance(G, queries, &graph::bfs);
    outFile << "BFS: " << bfs_result.mean << " " << bfs_result.std_dev << endl;
    
    TestResult dijkstra_result = measurePerformance(G, queries, &graph::dijkstra);
    outFile << "DIJ: " << dijkstra_result.mean << " " << dijkstra_result.std_dev << endl;
    
    TestResult h_bfs_result = measurePerformance(G, queries, &graph::hierarchicalBFS);
    outFile << "HBFS: " << h_bfs_result.mean << " " << h_bfs_result.std_dev << endl;
    
    TestResult h_dijkstra_result = measurePerformance(G, queries, &graph::hierarchicalDijkstra);
    outFile << "HDIJ: " << h_dijkstra_result.mean << " " << h_dijkstra_result.std_dev << endl;
    
    outFile << endl;
}

int main() {  
    string outputFile = "results.dat";
    ofstream outFile(outputFile);
    
    if (!outFile.is_open()) {
        cout << "Error: Could not create " << outputFile << endl;
        return 1;
    }
    
    vector<string> test_files = {
        "data_10.txt",
        "data_20.txt",
        "data_30.txt",
        "data_40.txt",
        "data_50.txt"
    };
    
    for (const string& file : test_files) {
        ifstream F(file);
        if (!F.is_open()) {
            cout << "Could not open " << file << endl;
            continue;
        }
        
        graph G;
        G.load(F);
        F.close();
        
        outFile << "File: " << file << endl;
        runPerformanceTests(G, outFile);
        cout << "Testing " << file << "..." << endl;
    }
    
    outFile.close();
    cout << "Results saved to " << outputFile << endl;
    
    return 0;
} 