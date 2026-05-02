#pragma once
#include "lexical_analyzer.h"
#include "production_rules.h"
#include <bits/stdc++.h>
using namespace std;

bool parser(string s, DFA &dfa) {
    auto [lexemes, tokens, line_number] = lexer(s, dfa);

    auto remove_comments = [&]() {
        vector<string> a, b;
        vector<int> c;
        for (int i = 0; i < tokens.size(); ++i) {
            if (tokens[i] != "comment") {
                a.push_back(lexemes[i]);
                b.push_back(tokens[i]);
                c.push_back(line_number[i]);
            }
        }
        lexemes = a;
        tokens = b;
        line_number = c;
    };
    remove_comments();

    int n = tokens.size();
    vector<string> program = tokens;
    vector<bool> terminal(n, true);
    int cnt = 10000;
redo:;
    for (int l = 0; l < program.size(); ++l) {
        for (int r = l; r < program.size(); ++r) {
            for (auto production_rule : production_rules) {
                if (production_rule.match(program, l, r, terminal, l, r)) {
                    program.erase(program.begin() + l, program.begin() + r + 1);
                    terminal.erase(terminal.begin() + l, terminal.begin() + r + 1);
                    program.insert(program.begin() + l, production_rule.name);
                    terminal.insert(terminal.begin() + l, false);
                    if (cnt--) goto redo;
                }
            }
        }
    }

    return program.size() == 1 && program[0] == "S";
}