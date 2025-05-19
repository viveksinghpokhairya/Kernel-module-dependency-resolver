#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <stack>

using namespace std;

// Adjacency list to represent the graph
unordered_map<string, vector<string>> dependencyGraph;

// Stores final load order
vector<string> loadOrder;

// Set to detect visited nodes and avoid cycles
unordered_set<string> visited;
unordered_set<string> visiting;

// Extract only the filename from full path
string getFileName(const string& path) {
    size_t pos = path.find_last_of('/');
    if (pos != string::npos)
        return path.substr(pos + 1);
    return path;
}

// DFS for topological sorting with cycle detection
bool dfs(const string& node) {
    if (visited.count(node)) return true;
    if (visiting.count(node)) {
        cerr << "Cycle detected at: " << node << endl;
        return false;
    }

    visiting.insert(node);
    for (const auto& dep : dependencyGraph[node]) {
        if (!dfs(dep)) return false;
    }
    visiting.erase(node);
    visited.insert(node);
    loadOrder.push_back(node);
    return true;
}

// Reads .dep file and builds the dependency graph
void buildGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t colonPos = line.find(":");
        if (colonPos == string::npos) continue;

        string module = line.substr(0, colonPos);
        string deps = line.substr(colonPos + 1);

        istringstream iss(deps);
        string dep;
        while (iss >> dep) {
            dependencyGraph[module].push_back(dep);
        }
    }
    file.close();
}

int main() {
    string inputFile = "sample.dep";  // Your .dep file

    buildGraphFromFile(inputFile);

    cout << "Resolving dependencies...\n";

    for (const auto& pair : dependencyGraph) {
        const string& module = pair.first;
        if (!visited.count(module)) {
            if (!dfs(module)) {
                cerr << "Aborting due to cycle.\n";
                return 1;
            }
        }
    }

    cout << "\nResolved Load Order:\n";
    for (const string& mod : loadOrder) {
        cout << getFileName(mod) << endl;
    }

    return 0;
}
