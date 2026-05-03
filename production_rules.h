#pragma once
#include <bits/stdc++.h>
using namespace std;

struct production_rule {
    string name;
    vector<string> pattern;
    vector<bool> terminal;
    int priority;

    production_rule() {}
    production_rule(string name, vector<string> pattern, vector<bool> terminal, int priority)
        : name(name), pattern(pattern), terminal(terminal), priority(priority) {}

    bool match(vector<string> a, vector<bool> b) {
        if (a.size() != b.size() or a.size() != pattern.size()) {
            return false;
        }

        for (int i = 0; i < a.size(); ++i) {
            if (a[i] != pattern[i] or b[i] != terminal[i]) {
                return false;
            }
        }
        return true;
    }

    bool match(vector<string> a, int l1, int r1, vector<bool> b, int l2, int r2) {
        return match(vector<string>(a.begin() + l1, a.begin() + r1 + 1), vector<bool>(b.begin() + l2, b.begin() + r2 + 1));
    }
};

/*
    don't convert identifier if
    - followed by assignment_operator 
    - read_keyword before it
    - write_keyword before it
    - comma before it
    - comma after it
*/

vector<production_rule> production_rules = {
    production_rule("MATH_OP",
                    {"plus_operator"},
                    {1}, 0),
    production_rule("MATH_OP",
                    {"minus_operator"},
                    {1}, 0),
    production_rule("MATH_OP",
                    {"multiply_operator"},
                    {1}, 0),
    production_rule("MATH_OP",
                    {"division_operator"},
                    {1}, 0),
    production_rule("BOOL_OP",
                    {"less_than_operator"},
                    {1}, 0),
    production_rule("BOOL_OP",
                    {"equal_to_operator"},
                    {1}, 0),

    production_rule("MATH_EXPRESSION",
                    {"open_bracket_operator", "MATH_EXPRESSION", "close_bracket_operator"},
                    {1, 0, 1}, 1),
    production_rule("MATH_EXPRESSION",
                    {"identifier"},
                    {1}, 1),
    production_rule("MATH_EXPRESSION",
                    {"number"},
                    {1}, 1),
    production_rule("MATH_EXPRESSION",
                    {"MATH_EXPRESSION", "MATH_OP", "MATH_EXPRESSION"},
                    {0, 0, 0}, 1),

    production_rule("BOOL_EXPRESSION",
                    {"MATH_EXPRESSION", "BOOL_OP", "MATH_EXPRESSION"},
                    {0, 0, 0}, 2),
    production_rule("BOOL_EXPRESSION",
                    {"open_bracket_operator", "BOOL_EXPRESSION", "close_bracket_operator"},
                    {1, 0, 1}, 2),
    production_rule("ASSIGNMENT",
                    {"identifier", "assignment_operator", "MATH_EXPRESSION", "semicolon"},
                    {1, 1, 0, 1}, 2),

    production_rule("IDS",
                    {"identifier"},
                    {1}, 3),
    production_rule("IDS",
                    {"IDS", "comma", "IDS"},
                    {0, 1, 0}, 3),

    production_rule("READ",
                    {"read_keyword", "IDS", "semicolon"},
                    {1, 0, 1}, 4),

    production_rule("IDS_AND_LITRALS",
                    {"identifier"},
                    {1}, 5),
    production_rule("IDS_AND_LITRALS",
                    {"string"},
                    {1}, 5),
    production_rule("IDS_AND_LITRALS",
                    {"IDS"},
                    {0}, 5),
    production_rule("IDS_AND_LITRALS",
                    {"IDS_AND_LITRALS", "comma", "IDS_AND_LITRALS"},
                    {0, 1, 0}, 5),

    production_rule("WRITE",
                    {"write_keyword", "IDS_AND_LITRALS", "semicolon"},
                    {1, 0, 1}, 6),

    production_rule("IF_THEN",
                    {"if_keyword", "BOOL_EXPRESSION", "then_keyword", "S", "end_keyword", "semicolon"},
                    {1, 0, 1, 0, 1, 1}, 7),
    production_rule("IF_THEN_ELSE",
                    {"if_keyword", "BOOL_EXPRESSION", "then_keyword", "S", "else_keyword", "S", "end_keyword", "semicolon"},
                    {1, 0, 1, 0, 1, 0, 1, 1}, 7),
    production_rule("REPEAT",
                    {"repeat_keyword", "S", "until_keyword", "BOOL_EXPRESSION", "semicolon"},
                    {1, 0, 1, 0, 1}, 7),
    production_rule("S",
                    {"ASSIGNMENT"},
                    {0}, 7),
    production_rule("S",
                    {"READ"},
                    {0}, 7),
    production_rule("S",
                    {"WRITE"},
                    {0}, 7),
    production_rule("S",
                    {"IF_THEN"},
                    {0}, 7),
    production_rule("S",
                    {"IF_THEN_ELSE"},
                    {0}, 7),
    production_rule("S",
                    {"REPEAT"},
                    {0}, 7),
    production_rule("S",
                    {"S", "S"},
                    {0, 0}, 7),
};