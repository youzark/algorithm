#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <ostream>
#include <iostream>

class Token {
private:
    union token_ {
        char op;
        int num;

        token_(char op):op(op) {};
        token_(int num):num(num) {};
    };
public:
    token_ token;
    bool isOp;
};

std::vector<Token> tokenize(std::string expression) {
    std::string ops = {"+-*/()"};
    std::vector<Token> tokens;
    auto cur = expression.begin(),end = expression.end();
    while(cur != end) {
        auto opPos = std::find_first_of(cur,end,ops.begin(),ops.end());
        if(opPos == cur) {
            tokens.push_back(Token{*opPos,true});
            cur = opPos+1;
        } else {
            tokens.push_back(Token{std::stoi(expression.substr(std::distance(expression.begin(),cur),std::distance(cur,opPos))),false});
            cur = opPos;
        }
    }
    return tokens;
}

std::ostream& operator<<(std::ostream& stream, std::vector<Token> tokens) {
    for( auto token : tokens ) {
        if(token.isOp) stream << token.token.op << " ";
        else stream << token.token.num << " ";
    }
    return stream;
}

class parser {
private:
    std::unordered_map<char,int> precedence_{{'(',0},{'+',1},{'-',1},{'*',2},{'/',2},{'#',0}};
public:
    std::string operator()(std::vector<Token> tokens) {
        std::ostringstream rpn;
        std::stack<char> opStack;
        opStack.push('#');
        auto cur = tokens.begin(),end = tokens.end();
        while(cur != end) {
            if(cur->isOp) {
                auto& op = cur->token.op;
                if(op == '(') {
                    opStack.push(op);
                } else if(op == ')') {
                    while(opStack.top() != '(') {
                        rpn << opStack.top() << ' ';
                        opStack.pop();
                    }
                    opStack.pop();
                } else {
                    while(precedence_[opStack.top()] >= precedence_[op] ) {
                        rpn << opStack.top() << ' ';
                        opStack.pop();
                    }
                    opStack.push(op);
                }
            } else {
                rpn << cur->token.num << ' ';
            }
            ++cur;
        }
        while( opStack.top() != '#' ) {
            rpn << opStack.top() << ' ';
            opStack.pop();
        }
        return rpn.str();
    }
};

int main() {
    parser p{};
    std::string input = "10*10+(1+8)/2*2 + 10";
    /* std::cout << tokenize(input); */
    std::cout << p(tokenize(input));
}
