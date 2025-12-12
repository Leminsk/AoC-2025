#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stack>

// https://www.geeksforgeeks.org/dsa/find-paths-given-source-destination/
void DFS(
    const std::string& src, const std::string& dest, 
    const std::unordered_map< std::string, std::vector<std::string> >& graph, 
    std::vector<std::string>& path,
    std::vector<std::vector<std::string>>& allPaths
) {
    // Add the current vertex to the path
    path.push_back(src);
    // Store the path when destination is reached
    if (src == dest){
        allPaths.push_back(path);
    } else{
        for (auto& adj_node : graph.at(src)){
            DFS(adj_node, dest, graph, path, allPaths);
        }
    }
    // remove the current vertex from the path
    path.pop_back();
}

std::vector<std::vector<std::string>> findPaths( 
    const std::unordered_map< std::string, std::vector<std::string> >& graph, 
    const std::string& src, const std::string& dest
) {
    std::vector<std::vector<std::string>> allPaths;
    std::vector<std::string> path;
    DFS(src, dest, graph, path, allPaths);
    return allPaths;
}

int main() {
    std::ifstream infile("day11-input.txt");
    std::string line;

    std::unordered_map< std::string, std::vector<std::string> > device_outputs = {};

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
                outputs.push_back(curr_output);
                curr_output = "";
            } else {
                curr_output += line[i];
            }
        }
        outputs.push_back(curr_output);
        device_outputs[device] = outputs;
    }

    std::string source = "you";
    std::string destination = "out";

    std::vector< std::vector<std::string> > paths = findPaths(device_outputs, source, destination);
    std::cout << "total paths:" << paths.size() << '\n';

    

    return 0;
}