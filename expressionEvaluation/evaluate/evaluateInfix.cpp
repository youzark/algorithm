#include <pthread.h>
#include <vector>
#include <stack>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <ostream>

class Token {
public:
    union {
        char op;
        int num;
    };
    bool isOp;
    Token(int num):
        num(num),
        isOp(false)
    {}
    Token(char op):
        op(op),
        isOp(true)
    {}
};

std::vector<Token> tokenize(std::string expression) {
    std::string delimiter{"()+-*/"};
    std::vector<Token> tokens;
    auto start = expression.begin(), end = expression.end();
    for( auto cur = start; cur != end;) {
        auto next_op = std::find_first_of(cur,end,delimiter.begin(),delimiter.end());
        if(next_op != cur) {
            std::istringstream ss{std::string{cur,next_op}};
            int num;
            if (ss >> num)
                tokens.emplace_back(num);
        }
        if(next_op == end) { cur = end; }
        else {
            cur = next_op + 1;
            tokens.emplace_back(*next_op);
        }
    }
    return tokens;
}

int precedence(char op) {
    switch (op) {
        case '+': return 1;
        case '-': return 1;
        case '*': return 2;
        case '/': return 2;
        default: return 0;
    }
}

int applyOp(int lhs, int rhs, char op) {
    switch (op) {
        case '+': return lhs + rhs;
        case '-': return lhs - rhs;
        case '*': return lhs * rhs;
        case '/': return lhs / rhs;
        default: return rhs;
    }
}

int evaluate(std::string expression) {
    std::vector<Token> tokens = tokenize(expression);
    std::stack<char> opStack;
    opStack.push('#');
    std::stack<int> operands;

    auto calStack = [&](){
        int rhs = operands.top();
        operands.pop();
        int lhs = operands.top();
        operands.pop();
        operands.push(applyOp(lhs,rhs,opStack.top()));
        opStack.pop();
    };

    for( auto& token : tokens ) {
        if(token.isOp) {
            if(token.op == '(') {
                opStack.push('(');
            } else if (token.op == ')') {
                while(opStack.top() != '(')  calStack();
                opStack.pop();
            } else {
                while(precedence(token.op) <= precedence(opStack.top())) calStack();
                opStack.push(token.op);
            }
        } else {
            operands.push(token.num);
        }
    }
    while(opStack.top() != '#') calStack();
    return operands.top();
}

std::ostream& operator<<(std::ostream &stream, std::vector<Token> const& tokens) {
    for( auto& token : tokens ) {
        token.isOp ? 
            stream << token.op << " ,":
            stream << token.num << " ,";
    }
    stream << std::endl;
    return stream;
}

int main() {
    std::string input{"( 10 +10  ) / 5 *9 + 120 / (30 - 10)"};
    std::cout << evaluate(input);
}
