#pragma once
#include "lexical_analyzer.h"
#include "production_rules.h"
#include <bits/stdc++.h>
using namespace std;

string parser(string s, DFA &dfa) {
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
            else {
                a.back() += lexemes[i];
            }
        }
        lexemes = a;
        tokens = b;
        line_number = c;
    };
    remove_comments();

    deque<string> dq;
    dq.push_front("S");

    stringstream ss;
    int last = 1;
    vector<int> lines;

    while (tokens.size()) {

        if (tokens[0] == LexicalError) {
            ss << line_number[0] << " lex" << '\n';
            last = line_number[0];
            tokens.erase(tokens.begin());
            line_number.erase(line_number.begin());
            continue;
        }

        auto front = dq.front();
        dq.pop_front();

        if (front == "empty") continue;

        if (isupper(front[0])) {

            if (parsing_table.count({front, tokens.front()}) == 0) {
                ss << line_number[0] << '\n';
                dq.push_front(front);
                last = line_number[0];
                tokens.erase(tokens.begin());
                line_number.erase(line_number.begin());
                continue;
            }

            auto pr = parsing_table[{front, tokens.front()}];

            if (pr.name == "synch") {
                if (front == "S") {
                    ss << line_number[0] << '\n';
                    dq.push_front(front);
                    last = line_number[0];
                    tokens.erase(tokens.begin());
                    line_number.erase(line_number.begin());
                }
                else {
                    ss << last << '\n';
                    continue;
                }
            }

            dq.insert(dq.begin(), pr.pattern.begin(), pr.pattern.end());
        }
        else {
            if (tokens.front() != front) {
                ss << last << '\n';
                continue;
            }
            last = line_number[0];
            tokens.erase(tokens.begin());
            line_number.erase(line_number.begin());
        }
    }

    while (dq.size() && count(first[{dq.front()}].begin(), first[{dq.front()}].end(), "empty")) {
        dq.pop_front();
    }

    return ss.str().size() ? ss.str() : "ok";

    // return (dq.size() ? "insuccessful" : "successful");

    // int n = tokens.size();
    // vector<string> program = tokens;

    // for (int p = 0; p <= 9; ++p) {
    //     vector<production_rule> production_rules_;
    //     for (auto production_rule : production_rules) {
    //         if (production_rule.priority == p) {
    //             production_rules_.push_back(production_rule);
    //         }
    //     }

    // redo:;
    //     for (int l = 0; l < program.size(); ++l) {
    //         for (int r = l; r < program.size(); ++r) {
    //             for (auto production_rule : production_rules_) {
    //                 if (production_rule.match(program, l, r)) {
    //                     int last_line = line_number[r];
    //                     string temp = accumulate(lexemes.begin() + l, lexemes.begin() + r + 1, string(""), [&](string a, string b) { return a + b; });

    //                     program.erase(program.begin() + l, program.begin() + r + 1);
    //                     line_number.erase(line_number.begin() + l, line_number.begin() + r + 1);
    //                     lexemes.erase(lexemes.begin() + l, lexemes.begin() + r + 1);

    //                     program.insert(program.begin() + l, production_rule.name);
    //                     line_number.insert(line_number.begin() + l, last_line);
    //                     lexemes.insert(lexemes.begin() + l, temp);

    //                     goto redo;
    //                 }
    //             }
    //         }
    //     }
    // }

    // if (program == vector<string>{"S"}) {
    //     return "successful";
    // }
    // else {
    //     stringstream ss;
    //     for (int i = 0; i < program.size(); ++i) {
    //         if (program[i] == LexicalError) {
    //             ss << lexemes[i] << '\n';
    //             ss << LexicalError << " at line " << line_number[i] << '\n';
    //             ss << "--------------------------------------------------------------------\n";
    //         }
    //         else {
    //             if (program[i] != "S") {
    //                 ss << lexemes[i] << '\n';
    //                 ss << SyntaxError << " at line " << line_number[i] << '\n';
    //                 ss << "--------------------------------------------------------------------\n";
    //             }
    //         }
    //     }

    //     return ss.str();
    // }
}