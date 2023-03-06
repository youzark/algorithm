#include <iostream>
#include <fstream>

#include <queue>
#include <vector>
#include <string>
#include <unordered_map>

#include <utility>
#include <algorithm>

class Graph {
public:
    std::vector<std::vector<int>> m_graph;
    int m_size;

    Graph(int size):
        m_graph(size+1,std::vector<int>{}),
        m_size(size) { }

    void add_edge(int tail, int head) {
        m_graph[tail].emplace_back(head);
    }

    auto& all_neighbor(int tail) {
        return m_graph[tail];
    }

};

int main() {
    std::ifstream fin{"graph.in"};
    int node_count;
    int tail, head;
    if(fin.is_open()) fin >> node_count;
    Graph g(node_count);
    if(fin.is_open()) {
        while(fin >> tail >> head) { 
            g.add_edge(tail,head); 
            g.add_edge(head,tail);
        }
    }
    std::vector<bool> visited(node_count+1,false);

    auto is_connected = [&]() -> bool {
        std::queue<int> frontier;
        int first = visited[1] ? 2 : 1;
        frontier.push(first);
        visited[first] = true;
        while(!frontier.empty()) {
            auto cur = frontier.front();
            frontier.pop();
            for( int neighbor : g.all_neighbor(cur) ) {
                if( !visited[neighbor] ) {
                    visited[neighbor] = true;
                    frontier.push(neighbor);
                }
            }
        }
        for( int i = 1; i <= node_count; ++i ) if(!visited[i]) return false;
        return true;
    };

    for( int i = 1; i <= node_count; ++i ) {
        std::fill(visited.begin(),visited.end(),false);
        visited[i] = true;
        if(!is_connected()) {
            std::cout << i << std::endl;
            break;
        }
    }
}

