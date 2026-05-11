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
    string last_s = "";
    auto add_error = [&](int line_number, string type, string text_in_code) {
        ss << text_in_code << '\n';
        ss << type << " Error at line: " << line_number << '\n';
        ss << "---------------------------------------------------------------\n";
    };

    while (tokens.size()) {
        if (tokens[0] == "Error") {
            add_error(line_number[0], "Lexical", lexemes[0]);
            last = line_number[0], last_s = lexemes[0];
            tokens.erase(tokens.begin());
            lexemes.erase(lexemes.begin());
            line_number.erase(line_number.begin());
            continue;
        }

        if (dq.empty()) {
            add_error(line_number[0], "Syntax", lexemes[0]);
            break;
        }

        auto front = dq.front();
        dq.pop_front();

        if (front == "empty") continue;

        if (isupper(front[0])) {

            if (parsing_table.count({front, tokens.front()}) == 0) {
                add_error(line_number[0], "Syntax", lexemes[0]);
                last = line_number[0], last_s = lexemes[0];

                dq.push_front(front);
                tokens.erase(tokens.begin());
                lexemes.erase(lexemes.begin());
                line_number.erase(line_number.begin());
                continue;
            }

            auto pr = parsing_table[{front, tokens.front()}];

            if (pr.name == "synch") {
                if (front == "S") {
                    add_error(line_number[0], "Syntax", lexemes[0]);
                    last = line_number[0], last_s = lexemes[0];
                    dq.push_front(front);
                    tokens.erase(tokens.begin());
                    lexemes.erase(lexemes.begin());
                    line_number.erase(line_number.begin());
                    continue;
                }
                else {
                    add_error(last, "Syntax", last_s);
                    last = line_number[0], last_s = lexemes[0];
                    continue;
                }
            }

            if (dq.empty() && front == "S" && tokens.size() && pr.pattern == vector<string>{"empty"}) {
                add_error(line_number[0], "Syntax", lexemes[0]);
                last = line_number[0], last_s = lexemes[0];
                dq.push_front(front);
                tokens.erase(tokens.begin());
                lexemes.erase(lexemes.begin());
                line_number.erase(line_number.begin());
                continue;
            }

            dq.insert(dq.begin(), pr.pattern.begin(), pr.pattern.end());
        }
        else {
            if (tokens.front() != front) {
                add_error(last, "Syntax", last_s);
                last = line_number[0], last_s = lexemes[0];
                continue;
            }
            last = line_number[0], last_s = lexemes[0];
            tokens.erase(tokens.begin());
            lexemes.erase(lexemes.begin());
            line_number.erase(line_number.begin());
        }
    }

    while (dq.size() && isupper(dq.front()[0]) && count(first[{dq.front()}].begin(), first[{dq.front()}].end(), "empty")) {
        dq.pop_front();
    }

    if (dq.size()) {
        add_error(last, "Syntax", last_s);
    }

    return ss.str().size() ? ss.str() : "OK";
}