#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>

class Edge {
public:
    int tail;
    int head;
    int weight;
    Edge(int tail, int head, int weight):
        tail(tail),
        head(head),
        weight(weight) {}
    friend bool operator<(Edge const& lhs,Edge const& rhs) {return lhs.weight < rhs.weight;}
};

class Graph {
public:
    std::vector<std::vector<Edge>> graph;

    Graph(int size):
        graph(std::vector(size,std::vector<Edge>{})) {};

    Graph& add_edge(int tail, int head, int weight) {
        graph[tail].push_back(Edge{tail,head,weight});
        return *this;
    }

    std::vector<Edge>& all_edges(int tail) {
        return graph[tail];
    }
};

class Union_find {
private:
    std::vector<int> m_tree;
public:
    Union_find(int size):
        m_tree(std::vector<int>(size,0)) {
            for(int i = 0; i < size; ++i) m_tree[i] = i;
        }

    int find(int node) {
        int cur = node;
        while( cur != m_tree[cur] ) cur = m_tree[cur];
        return cur;
    }

    void merge(int lhs, int rhs) {
        int lparent = find(lhs);
        int rparent = find(rhs);
        m_tree[lparent] = rparent;
    }
};

int main() {

    // read in the graph representation
    std::ifstream fin{"graph.in"};
    int nodes_count;
    int tail, head, weight;

    // minimum spanning tree edges
    std::vector<Edge> mst{};
    // use multiset so that all edges sorted according to weight
    std::multiset<Edge> all_edges;
    // disjoint set used to determine if adding an edge will lead to a cycle

    if(fin.is_open()) { fin >> nodes_count; }
    Graph g(nodes_count);
    Union_find connect_cmp{nodes_count};
    if(fin.is_open()) {
        while(fin >> tail >> head >> weight) { 
            g.add_edge(tail,head,weight); 
            all_edges.insert(Edge{tail,head,weight});
        }
        fin.close();
    }

    for( auto& edge : all_edges ) {
        /* std::cout << edge.tail << " -- " << edge.head << " : " << edge.weight << std::endl; */
        if(mst.size() == nodes_count-1) break;
        if(connect_cmp.find(edge.tail) == connect_cmp.find(edge.head)) continue;
        mst.emplace_back(edge.tail,edge.head,edge.weight);
        connect_cmp.merge(edge.tail,edge.head);
    }

    std::ofstream fout{"graph.out"};
    if(fout.is_open()) {
        for( auto& edge : mst ) {
            fout << edge.tail << " -- " << edge.head << " : " << edge.weight << "\n";
        }
        fout.close();
    }
}
