#include <fstream>
#include <iostream>

#include <limits>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stack>

#include <algorithm>
#include <utility>
#include <ranges>

class Graph {
public:
    using Edge = std::pair<int,int>;
    std::vector<std::vector<Edge>> m_graph;
    int m_size;

    Graph(int size):
        m_graph(size+1,std::vector<Edge>{}), // 1 based offset
        m_size(size) { }

    void add_edge(int tail, int head, int weight) { m_graph[tail].emplace_back(head,weight); }

    void add_bi_edge(int tail, int head, int weight) { 
        add_edge(tail,head,weight);
        add_edge(head,tail,weight);
    }

    std::vector<Edge>& all_edge(int tail) {
        return m_graph[tail];
    }
};

int main() {

    // Read in the graph
    std::ifstream fin{"graph.in"};
    int node_count, edge_count, start, destination;
    int tail, head, weight;
    if(fin.is_open()) fin >> node_count >> edge_count >> start >> destination;
    Graph g{node_count};
    if(fin.is_open()) {
        while(fin >> tail >> head >> weight) { g.add_bi_edge(tail, head, weight); }
    }

    using dist = std::pair<int,int>; // vectex(head: the next reachable node to include) : path_length
    std::vector<int> dists(node_count+1,std::numeric_limits<int>::max());
    dists[start] = 0;
    std::vector<int> pre_node(node_count+1,-1);
    std::unordered_set<int> visited{};
    auto longer_dist = [](dist lhs, dist rhs) {return lhs.second > rhs.second;};
    std::priority_queue<dist,std::vector<dist>,decltype(longer_dist)> distanceQ{longer_dist};
    distanceQ.push({start,0});

    while(!distanceQ.empty() && visited.find(distanceQ.top().first) == visited.end()) {
        if(visited.find(destination) != visited.end()) break;
        auto [cur_node,cur_distance] = distanceQ.top();
        distanceQ.pop();
        visited.insert(cur_node);

        // relax distances to all neighbor nodes
        for(auto& edge : g.all_edge(cur_node)) {
            if( cur_distance + edge.second < dists[edge.first]) {
                // relaxation on edge
                dists[edge.first] = cur_distance + edge.second;
                pre_node[edge.first] = cur_node;
                distanceQ.push({edge.first,cur_distance + edge.second});
            }
        }
    }

    if(pre_node[destination] == -1) { std::cout << "can't arrive" << std::endl; }
    std::stack<int> nodes_track{};
    if(pre_node[destination] != -1) for( auto cur = destination; cur != -1; cur = pre_node[cur] ) nodes_track.push(cur);
    std::cout << dists[destination] << std::endl;
    while(!nodes_track.empty()) {
        std::cout << nodes_track.top() << " | ";
        nodes_track.pop();
    }
}
