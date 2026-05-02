#pragma once
#include "get_DFA.h"
#include <bits/stdc++.h>
using namespace std;

vector<int> get_line_numbers(const string &s) {
    int n = s.size();
    vector<int> LN(n);
    int cur = 1;
    for (int i = 0; i < n; ++i) {
        LN[i] = cur;
        cur += (s[i] == '\n');
    }
    return LN;
}

/// @brief
/// @param s
/// @param dfa
/// @return {length, message, first_dead_index}
tuple<int, string, int> longest_valid_token(const string &s, DFA &dfa) {
    int length = 0, first_dead_index = -1;
    string message = "";

    for (int i = 0, cur = 0; i < s.size(); ++i) {
        cur = dfa.next_state(cur, s[i]);
        if (dfa.v[cur].state == STATE::dead && first_dead_index != -1) {
            first_dead_index = i;
        }
        if (dfa.v[cur].state == STATE::final) {
            message = dfa.v[cur].message;
            length = i + 1;
        }
    }

    return tuple{length, message, first_dead_index};
}

/// @brief 
/// @param s 
/// @param dfa 
/// @return {lexemes, tokens, line_number}
tuple<vector<string>, vector<string>, vector<int>> lexer(string s, DFA &dfa) {
    auto s_line_number = get_line_numbers(s);

    vector<string> lexemes, tokens;
    vector<int> line_number;

    /*
    * used to add first substring in s with the input length as lexeme
    * message parameter will be the token of this lexeme
    * erase this lexeme from s, also erase from s_line_number to keep track of lines
    */
    auto resolve = [&](int length, string message, bool print_last = true) {
        if (message.size()) {
            lexemes.push_back(s.substr(0, length - !print_last));
            tokens.push_back(message);
            line_number.push_back(s_line_number[length - !print_last - 1]);
        }
        s.erase(s.begin(), s.begin() + length);
        s_line_number.erase(s_line_number.begin(), s_line_number.begin() + length);
    };

    while (s.size()) {
        auto [length, message, first_dead_index] = longest_valid_token(s, dfa);

        if (length != 0) {
            resolve(length, message);
        }
        else {
            if (first_dead_index != -1) {
                if (s[first_dead_index] != '\n' and s[first_dead_index] != ' ') {
                    resolve(first_dead_index + 1, "Lexical Error (not a valid tiny token).");
                }
                else {
                    if (0 < first_dead_index) {
                        resolve(first_dead_index + 1, "Lexical Error (not a valid tiny token).", false);
                    }
                    else {
                        resolve(first_dead_index + 1, "");
                    }
                }
            }
            else {
                resolve(s.size(), "Lexical Error (not a valid tiny token).");
            }
        }
    }

    return tuple{lexemes, tokens, line_number};
}
