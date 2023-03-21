#include <algorithm>
#include <vector>
#include <functional>
#include <fstream>
#include <string>
#include <iostream>

class Node {
public:
    float val;
    Node* left;
    Node* right;
    int seq;

    Node(float val,int seq):
        val(val),
        left(nullptr),
        right(nullptr),
        seq(seq) {}

    Node(float val,Node* left,Node* right):
        val(val),
        left(left),
        right(right),
        seq(0) {}
};


int main() {
    std::ifstream fin{"tree.in"};
    std::vector<Node*> nodes;
    float weight;
    int seq = 1;
    if(fin.is_open()) {
        while(fin >> weight) {
            nodes.push_back(new Node(weight,seq++));
        }
        fin.close();
    }

    auto compare_node = [](Node* lhs,Node* rhs) -> bool { return lhs->val > rhs->val; }; // min heap
    std::make_heap(nodes.begin(),nodes.end(),compare_node);

    auto end = nodes.end();
    while(end - nodes.begin() > 2) {
        Node *left = nodes[0];
        std::pop_heap(nodes.begin(),end--,compare_node);
        Node *right = nodes[0];
        std::pop_heap(nodes.begin(),end,compare_node);
        *(end-1) = new Node{left->val+right->val,left,right};
        std::push_heap(nodes.begin(),end,compare_node);
    }
    Node *root = new Node{nodes[0]->val+nodes[1]->val,nodes[0],nodes[1]};


    std::vector<std::string> codes(nodes.size(),"");
    std::string code{""};
    std::function<void(Node*)> encode = [&](Node *root) {
        if(root == nullptr) {
            code.pop_back();
            return;
        }
        if(root->seq) codes[root->seq-1] = code;
        code.push_back('1'); 
        encode(root->left);
        code.push_back('0'); 
        encode(root->right);
        if(!code.empty()) code.pop_back();
    };

    encode(root);
    std::ofstream fout{"tree.out"};
    if(fout.is_open()) {
        for( auto& code : codes ) fout << code << '\n';
        fout.close();
    }
}
