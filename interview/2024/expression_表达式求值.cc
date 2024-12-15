#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <stack>
#include <algorithm>
#include <iterator>
#include <unordered_map>

using namespace std;

int solve(const std::string& expr) {
    if (expr.empty()) {
        throw std::runtime_error("Invalid expression!");
    }

    static std::unordered_map<char, int> op_priority = {
        {'+', 20},
        {'-', 20},
        {'*', 50},
        {'/', 50},
        {'(', 10},  // 左括号优先级最小，但无条件入栈
        {')', 10},
    };

    std::deque<int> oprand;
    std::deque<char> optr;

    auto DEBUG = [&] {
        cout << "oprand: ";
        std::copy(oprand.begin(), oprand.end(), std::ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "optr: ";
        std::copy(optr.begin(), optr.end(), std::ostream_iterator<char>(cout, " "));
        cout << endl;
    };

    auto compute_op = [](int op1, int op2, char op)->int {
        switch(op) {
            case '+': return op1 + op2;
            case '-': return op1 - op2;
            case '*': return op1 * op2;
            case '/': return op1 / op2;
        }
        return 0;
    };

    auto compute_stack = [&](std::function<bool(char)> cond) {
        while (!optr.empty() && cond(optr.back())) {
            // 一般计算流程
            char op = optr.back();
            optr.pop_back();
            if (op == '(' || op == ')')
            { continue; }
            if (oprand.size() < 2)
            { return; }
            int op2 = oprand.back();  // NOTE!!!
            oprand.pop_back();
            int op1 = oprand.back();
            oprand.pop_back();
            int res = compute_op(op1, op2, op);
            oprand.push_back(res);
        }
    };

    int num = 0;
    bool digit = false;  // 是否是数字标志位
    for (char ch : expr) {
        if (::isdigit(ch)) {
            digit = true;
            num = num * 10 + (ch - '0');
        } else {  // is operator
            if (!op_priority.count(ch)) {
                throw std::runtime_error("Invalid expression!");
            }
            if (digit) {
                oprand.push_back(num);
            }
            digit = false;
            num = 0;
            if (ch == '(') {  // 无条件入栈
                optr.push_back(ch);
            } else if (ch == ')') {
                compute_stack([](char op)->bool{ return op != '('; });
                if (optr.back() == '(') { optr.pop_back(); }
            } else {  // + - * /   除括号外一般操作符处理流程
                compute_stack([&](char op)->bool{
                    return op_priority[op] > op_priority[ch];
                });
                optr.push_back(ch);  // 当前optr入栈
            }
        } // non digit
    } // for ch
    // NOTE!!!
    if (digit) { oprand.push_back(num); }

    compute_stack([](char)->bool{ return true; });

    return oprand.back();
}

int main() {
    int result = solve("9+(8*2+3)+1");
    cout << result << endl;

    return 0;
}
