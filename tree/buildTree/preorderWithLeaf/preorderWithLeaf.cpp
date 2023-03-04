#include <functional>
#include <iostream>
#include <utility>
#include <ostream>
#include <vector>
#include <string>
#include <fstream>

class Node {
public:
    Node* left;
    Node* right;
    char val;
    Node(char val):
        left(nullptr),
        right(nullptr),
        val(val) {}
    Node(char val,Node* left,Node* right):
        left(left),
        right(right),
        val(val) {}
};

std::pair<Node*,std::string::iterator> build_from_inorder(std::string::iterator start,std::string::iterator end) {
    if(start == end) return{ nullptr,end };
    if(*start == '0') return{ nullptr, start+1 };

    auto [left,remainString] = build_from_inorder(start+1,end);
    auto [right,newEnd] = build_from_inorder(remainString,end);
    return {new Node{*start,left,right},newEnd};
}

int max_height(Node* root) {
    if(root == nullptr) {return 0;}
    return std::max(max_height(root->left),max_height(root->right))+1;
}

void inorderTraveral(Node *root) {
    if(!root) return;
    std::cout << root->val << " | ";
    inorderTraveral(root->left);
    inorderTraveral(root->right);
}

int main() {
    std::ifstream fin{"tree.in"};
    std::string preTrave;
    if(fin.is_open()) {
        std::getline(fin,preTrave);
        fin.close();
    }
    auto [root,_]= build_from_inorder(preTrave.begin(),preTrave.end());
    int height = max_height(root);

    std::ofstream fout{"tree.out"};
    std::function<void(Node*,int)> output_upper_nodes = [&](Node* root,int depth) {
        if(root == nullptr) return;
        if(depth < height / 2) output_upper_nodes(root->left, depth + 1);
        if(depth <= height/2) fout << root->val << " ";
        if(depth < height / 2) output_upper_nodes(root->right, depth + 1);
    };
    
    if(fout.is_open()) {
        output_upper_nodes(root,1);
        fout.close();
    }
}

