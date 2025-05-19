#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

string getFilenameOnly(const string& path) {
    size_t pos = path.find_last_of('/');
    return (pos != string::npos) ? path.substr(pos + 1) : path;
}

void topologicalSort(
    unordered_map<string, vector<string>>& graph,
    unordered_map<string, int>& inDegree
) {
    queue<string> q;

    for (const auto& entry : inDegree) {
        if (entry.second == 0)
            q.push(entry.first);
    }

    cout << "\nModule Load Order with Dependencies:\n\n";
    while (!q.empty()) {
        string module = q.front();
        q.pop();
        // cout << getFilenameOnly(module);
        cout << getFilenameOnly(module);
        for (const string& dep : graph[module]) {
            cout << " ---> " << getFilenameOnly(dep);
            inDegree[dep]--;
            if (inDegree[dep] == 0)
                q.push(dep);
        }
        cout << endl;
    }
}

int main() {
    ifstream file("sample.dep");
    if (!file.is_open()) {
        cerr << "Error: Could not open sample.dep file.\n";
        return 1;
    }

    unordered_map<string, vector<string>> graph;
    unordered_map<string, int> inDegree;
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string module, dep;
        getline(iss, module, ':');
        vector<string> dependencies;

        while (iss >> dep) {
            dependencies.push_back(dep);
            inDegree[dep]++;
        }

        if (!inDegree.count(module)) inDegree[module] = 0;

        graph[module] = dependencies;
    }
    for(auto ele : graph){
        for(auto itr : ele){
            cout << itr << " ";
        } 
        cout << endl;
    }
    file.close();

    topologicalSort(graph, inDegree);

    return 0;
}
