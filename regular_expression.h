#pragma once
#include <bits/stdc++.h>
using namespace std;

class regular_expression {
private:
public:
    string expression;
    vector<bool> is_char;
    string message = "";
    int priority = 1E9;

    regular_expression() {}
    regular_expression(string expression, vector<bool> is_char, string message = "", int priority = 1E9)
        : expression(expression), is_char(is_char), message(message), priority(priority) {
        // if (is_char.size() < expression.size()) {
        //     cout << expression << '\n';
        //     for (int i = 0; i < is_char.size(); ++i) {
        //         cout << is_char[i];
        //     }
        //     exit(0);
        // }
        refine();
    }

    void refine() {
    done:;
        for (int i = 0; i + 1 < expression.size(); ++i) {
            if (is_char[i] and is_char[i + 1]) {
                insert(i + 1);
                goto done;
            }

            if ((!is_char[i] and expression[i] == '*') and (is_char[i + 1] or expression[i] == '(')) {
                insert(i + 1);
                goto done;
            }
        }
    }

    void insert(int i) {
        expression.insert(expression.begin() + i, '.');
        is_char.insert(is_char.begin() + i, false);
    }

    void pop_back() {
        expression.pop_back();
        is_char.pop_back();
    }

    void pop_front() {
        expression.erase(expression.begin());
        is_char.erase(is_char.begin());
    }

    pair<regular_expression, regular_expression> break_at(int i) {
        regular_expression a, b;
        a.expression = string(expression.begin(), expression.begin() + i);
        a.is_char = vector<bool>(is_char.begin(), is_char.begin() + i);
        b.expression = string(expression.begin() + i + 1, expression.end());
        b.is_char = vector<bool>(is_char.begin() + i + 1, is_char.end());
        b.priority = priority;
        b.message = message;
        return {a, b};
    }

    void display() {
        cout << expression << '\n';
    }
};