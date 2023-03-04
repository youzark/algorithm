#include <iostream>
#include <ostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <sstream>
#include <functional>

class Node {
public:
    Node* left;
    Node* right;
    char val;
    Node(char val):
        val(val),
        left(nullptr),
        right(nullptr) {}
    Node(char val,Node* left, Node* right):
        val(val),
        left(left),
        right(right) {}
};

using iter = std::vector<char>::iterator;
Node* buildSubtree(iter in_begin,iter in_end,iter post_begin,iter post_end) {
    if(in_begin == in_end) return nullptr;
    auto root = new Node(*(post_end - 1));
    auto in_cur = in_begin;
    for(; in_cur != in_end; ++in_cur) { if(*in_cur == root->val) { break; } }
    auto post_cur = post_begin + std::distance(in_begin,in_cur);
    root->left = buildSubtree(in_begin,in_cur,post_begin,post_cur);
    root->right = buildSubtree(in_cur+1,in_end,post_cur,post_end-1);
    return root;
}

std::ostream& operator<<(std::ostream &stream, std::vector<char> tokens) {
    for( auto& token : tokens ) {
        stream << token << " | ";
    }
    stream << std::endl;
    return stream;
}

int main() {
    std::vector<char> inorder;
    std::vector<char> postorder;
    std::ifstream fin{"tree.in"};
    if(fin.is_open()) {
        std::string line;
        std::getline(fin,line);
        std::istringstream ss{line};
        char nodeVal;
        while(ss >> nodeVal) inorder.push_back(nodeVal);
        std::getline(fin,line);
        std::istringstream oss{line};
        while(oss >> nodeVal) postorder.push_back(nodeVal);
        fin.close();
    }

    std::ofstream fout{"tree.out"};
    std::function<void(Node*)> preOrderTraversal = [&](Node* root) ->void {
        if(root == nullptr) return;
        fout << root->val << " ";
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);
    };

    if(fout.is_open()) {
        auto root = buildSubtree(inorder.begin(),inorder.end(),postorder.begin(),postorder.end());
        preOrderTraversal(root);
        fout.close();
    }
}
