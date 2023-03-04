#include <sstream>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <vector>
#include <ostream>
#include <iostream>

class Token {
public:
    union {
        char op;
        int num;
    };
    bool is_op;
    Token(int num):
        num(num),
        is_op(false) {}
    Token(char op):
        op(op),
        is_op(true) {}
};

std::vector<Token> tokenize(std::string expression) {
    auto start = expression.begin(),end = expression.end();
    std::vector<Token> tokens;
    std::string operas{"+-*/()"};
    for( auto cur = start; cur != end; ) {
        auto next_op = std::find_first_of(cur,end,operas.begin(),operas.end());
        int num;
        std::istringstream ss;
        if( cur != next_op ) {
            ss.str(std::string{cur,next_op});
            while( ss >> num ) tokens.emplace_back(num);
        }         
        if( next_op != end ) {
            tokens.emplace_back(*next_op);
            cur = next_op + 1;
        } else {
            cur = end;
        }
    }
    return tokens;
}

int precedence(char op) {
    switch(op) {
        case '+': return 1;
        case '-': return 1;
        case '*': return 2;
        case '/': return 2;
        default: return 0;
    }
}

int apply_operator(int lhs, int rhs, char op) {
    switch(op) {
        case '+': return lhs + rhs;
        case '-': return lhs - rhs;
        case '*': return lhs * rhs;
        case '/': return lhs / rhs;
        default: return lhs;
    };
}

int evaluate(std::string expression) {
    std::stack<int> operands;
    auto tokens = tokenize(expression);

    for( auto& token : tokens ) {
        if(token.is_op) {
            int rhs = operands.top();
            operands.pop();
            operands.top() = apply_operator(operands.top(),rhs,token.op);
        } else {
            operands.push(token.num);
        }
    }
    return operands.top();
}


std::ostream& operator<<(std::ostream &stream, std::vector<Token> tokens) {
    for( auto & token : tokens ) {
        if(token.is_op) {
            stream << token.op << " ,";
        } else {
            stream << token.num << " ,";
        }
    }
    stream << std::endl;
    return stream;
}

int main() {
    std::string input{"10 10 + 2 / 18 *"};
    std::cout << evaluate(input);
}
