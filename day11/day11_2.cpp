#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <deque>

int64_t findPaths(std::map<std::string, std::vector<std::string>>& graph, std::string src, std::string dest) {
    std::map<std::string, int> indegree;

    for(auto& [vertex, _] : graph) { indegree[vertex] = 0; }
    for(auto& [vertex, neighbors] : graph) {
        for(auto& n : neighbors) { indegree[n] += 1; }
    }

    std::deque<std::string> q;
    for(auto& u : graph) {
        if(indegree[u.first] == 0) { q.push_front(u.first); }
    }

    std::vector<std::string> topo = {};

    while(!q.empty()) {
        std::string u = q.front(); q.pop_front();
        topo.push_back(u);
        for(auto& v : graph[u]) {
            indegree[v] -= 1;
            if(indegree[v] == 0) {
                q.push_back(v);
            }
        }
    }

    std::map<std::string, int64_t> dp;
    for(auto& [vertex, _] : graph) { indegree[vertex] = 0; }
    dp[src] = 1;

    for(auto& u : topo) {
        for(auto& v : graph[u]) {
            dp[v] += dp[u];
        }
    }
    
    return dp[dest];
}

int main() {
    std::ifstream infile("day11-input.txt");
    std::string line;

    std::map< std::string, std::vector<std::string> > device_outputs = {};

    while(std::getline(infile, line)) {
        std::string device = "";
        std::vector<std::string> outputs = {};
        int i=0;
        for(; i<line.size(); ++i) {
            if(line[i] == ':') { break; }
            device += line[i];
        }
        i+=2;
        std::string curr_output = "";
        for(; i<line.size(); ++i) {
            if(line[i] == ' ') {
                outputs.push_back( curr_output );
                curr_output = "";
            } else {
                curr_output += line[i];
            }
        }
        outputs.push_back( curr_output );
        device_outputs[ device ] = outputs;
    }
    device_outputs["out"] = {};

    int64_t svr_dac = findPaths(device_outputs, "svr", "dac");
    int64_t dac_fft = findPaths(device_outputs, "dac", "fft");
    int64_t fft_out = findPaths(device_outputs, "fft", "out");

    int64_t svr_fft = findPaths(device_outputs, "svr", "fft");
    int64_t fft_dac = findPaths(device_outputs, "fft", "dac");
    int64_t dac_out = findPaths(device_outputs, "dac", "out");
    
    std::cout << "svr -> ... -> dac -> ... -> fft -> ... -> out: " << svr_dac * dac_fft * fft_out << '\n';
    std::cout << "svr -> ... -> fft -> ... -> dac -> ... -> out: " << svr_fft * fft_dac * dac_out << '\n';


    return 0;
}