#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>
#include <stack>
#include <functional>

class Graph {
public:
    std::vector<std::vector<int>> m_graph;
    Graph(): m_graph{} {};

    int newNode() {
        m_graph.push_back({});
        return m_graph.size()-1;
    }
    void add_edge(int tail, int head) {m_graph[tail].push_back(head);}
    std::vector<int>& all_neighbors(int node) {return m_graph[node];}
    /* void print() { */ 
    /*     for(int i = 0; i < m_graph.size(); ++i) { */
    /*         std::cout << "Node" << i << ": "; */
    /*         for(auto& nei: m_graph[i]) std::cout << nei << " | "; */
    /*         std::cout << std::endl; */
    /*     } */ 
    /* } */ 
};

std::string to_name(int i) {
    return "Task" + std::to_string(i);
}

using string_iter = std::string::iterator;
int to_num(string_iter begin, string_iter end) {
    return std::stoi(std::string(begin+4,end));
}

int main() {
    std::ifstream fin{"graph.in"};
    Graph g;
    if(fin.is_open()) {
        for(std::string line; std::getline(fin,line);) {
            int node = g.newNode();
            auto cur = std::find(line.begin()+1,line.end(),'T');
            while(cur != line.end()) {
                auto next = std::find(cur+1,line.end(),'T');
                g.add_edge(node,to_num(cur,next-1));
                cur = next;
            }
        }
        fin.close();
    }

    int size = g.m_graph.size();
    std::stack<int> topo_nodes{};
    std::vector<int> visited(size,0);
    std::function<void(int)> topo_sort = [&](int node) {
        if(visited[node]) return;
        visited[node] = true;
        for(auto neighbor : g.all_neighbors(node)) if(!visited[neighbor]) topo_sort(neighbor);
        topo_nodes.push(node);
    };
    for( int i = 0; i < size; ++i ) { if(!visited[i]) topo_sort(i); }

    std::ofstream fout{"graph.out"};
    if(fout.is_open()) {
        while(!topo_nodes.empty()) {
            fout << to_name(topo_nodes.top()) << " ";
            topo_nodes.pop();
        }
    }

    while(!topo_nodes.empty()) {
        std::cout << topo_nodes.top() << " | ";
        topo_nodes.pop();
    }
}

