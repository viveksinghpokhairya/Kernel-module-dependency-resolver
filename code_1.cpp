#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
void topologicalSort(unordered_map<string, vector<string>>& graph, unordered_map<string, int>& inDegree) {
    queue<string> q;
    int printed = 0;
    for (auto& pair : inDegree) {
        if (pair.second == 0) {
            q.push(pair.first);
        }
    }

    cout << "Module load order:\n";
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        cout << current << endl;
        printed++;

        for (const string& neighbor : graph[current]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }
    if (printed != graph.size()) {
        cerr << "Error: Cycle detected in dependencies.\n";
    }
}

int main() {
    ifstream file("sample.dep");
    if (!file) {
        cerr << "Failed to open file.\n";
        return 1;
    }

    unordered_map<string, vector<string>> graph;
    unordered_map<string, int> inDegree;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string module, dep;
        getline(ss, module, ':');
        module.erase(remove_if(module.begin(), module.end(), ::isspace), module.end());

        if (!graph.count(module)) {
            graph[module] = {};
            inDegree[module] = 0;
        }

        while (ss >> dep) {
            graph[dep];  
            graph[dep].push_back(module);  
            inDegree[module]++;
            if (!inDegree.count(dep)) {
                inDegree[dep] = 0;
            }
        }
    }

    topologicalSort(graph, inDegree);
    return 0;
}
