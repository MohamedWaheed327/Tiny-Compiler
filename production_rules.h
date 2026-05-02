#pragma once
#include <bits/stdc++.h>
using namespace std;

struct production_rule {
    string name;
    vector<string> pattern;
    vector<bool> terminal;

    production_rule() {}
    production_rule(string name, vector<string> pattern, vector<bool> terminal)
        : name(name), pattern(pattern), terminal(terminal) {}

    bool match(vector<string> a, vector<bool> b) {
        if (a.size() != b.size() or a.size() != pattern.size() - (pattern.back().back()=='_')) {
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

vector<production_rule> production_rules = {
    production_rule("IDS",
                    {"identifier", "IDS_"},
                    {1, 0}),
    production_rule("IDS_",
                    {"comma", "IDS", "IDS_"},
                    {1, 0, 0}),
    production_rule("IDS_",
                    {"empty"},
                    {1}),
    production_rule("IDS_AND_LITRALS",
                    {"identifier", "IDS_AND_LITRALS_"},
                    {1, 0}),
    production_rule("IDS_AND_LITRALS",
                    {"string", "IDS_AND_LITRALS_"},
                    {1, 0}),
    production_rule("IDS_AND_LITRALS_",
                    {"comma", "IDS_AND_LITRALS", "IDS_AND_LITRALS_"},
                    {1, 0, 0}),
    production_rule("IDS_AND_LITRALS_",
                    {"empty"},
                    {1}),
    production_rule("MATH_OP",
                    {"plus_operator"},
                    {1}),
    production_rule("MATH_OP",
                    {"minus_operator"},
                    {1}),
    production_rule("MATH_OP",
                    {"multiply_operator"},
                    {1}),
    production_rule("MATH_OP",
                    {"division_operator"},
                    {1}),
    production_rule("BOOL_OP",
                    {"less_than_operator"},
                    {1}),
    production_rule("BOOL_OP",
                    {"equal_to_operator"},
                    {1}),
    production_rule("MATH_EXPRESSION",
                    {"open_bracket_operator", "MATH_EXPRESSION", "close_bracket_operator", "MATH_EXPRESSION_"},
                    {1, 0, 1, 0}),
    production_rule("MATH_EXPRESSION",
                    {"identifier", "MATH_EXPRESSION_"},
                    {1, 0}),
    production_rule("MATH_EXPRESSION",
                    {"number", "MATH_EXPRESSION_"},
                    {1, 0}),
    production_rule("MATH_EXPRESSION_",
                    {"MATH_OP", "MATH_EXPRESSION", "MATH_EXPRESSION_"},
                    {0, 0, 0}),
    production_rule("MATH_EXPRESSION_",
                    {"empty"},
                    {1}),
    production_rule("BOOL_EXPRESSION",
                    {"MATH_EXPRESSION", "BOOL_OP", "MATH_EXPRESSION"},
                    {0, 0, 0}),
    production_rule("BOOL_EXPRESSION",
                    {"open_bracket_operator", "BOOL_EXPRESSION", "close_bracket_operator"},
                    {1, 0, 1}),
    production_rule("ASSIGNMENT",
                    {"identifier", "assignment_operator", "MATH_EXPRESSION", "semicolon"},
                    {1, 1, 0, 1}),
    production_rule("READ",
                    {"read_keyword", "IDS", "semicolon"},
                    {1, 0, 1}),
    production_rule("WRITE",
                    {"write_keyword", "IDS_AND_LITRALS", "semicolon"},
                    {1, 0, 1}),
    production_rule("IF_THEN",
                    {"if_keyword", "BOOL_EXPRESSION", "then_keyword", "S", "end_keyword", "semicolon"},
                    {1, 0, 1, 0, 1, 1}),
    production_rule("IF_THEN_ELSE",
                    {"if_keyword", "BOOL_EXPRESSION", "then_keyword", "S", "else_keyword", "S", "end_keyword", "semicolon"},
                    {1, 0, 1, 0, 1, 0, 1, 1}),
    production_rule("REPEAT",
                    {"repeat_keyword", "S", "until_keyword", "BOOL_EXPRESSION", "semicolon"},
                    {1, 0, 1, 0, 1}),
    production_rule("S",
                    {"ASSIGNMENT", "S_"},
                    {0, 0}),
    production_rule("S",
                    {"READ", "S_"},
                    {0, 0}),
    production_rule("S",
                    {"WRITE", "S_"},
                    {0, 0}),
    production_rule("S",
                    {"IF_THEN", "S_"},
                    {0, 0}),
    production_rule("S",
                    {"IF_THEN_ELSE", "S_"},
                    {0, 0}),
    production_rule("S",
                    {"REPEAT", "S_"},
                    {0, 0}),
    production_rule("S_",
                    {"S", "S_"},
                    {0, 0}),
    production_rule("S_",
                    {"empty"},
                    {1}),
};