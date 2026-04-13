#pragma once
#include "DFA.h"
#include <bits/stdc++.h>
using namespace std;

void lexer(string s, DFA &dfa) {
    vector<string> lexemes, tokens;

    while (s.size()) {
        int last = -1, cur = 0, n = s.size();
        string last_message;

        for (int i = 0; i < n; ++i) {
            cur = dfa.next_state(cur, s[i]);

            if (dfa.v[cur].state == STATE::dead) {
                if (last == -1) {
                    cout << "ERROR (invalid)";
                    return;
                }
                else {
                    cout << s.substr(0, last + 1) << ": " << last_message << '\n';
                    s.erase(s.begin(), s.begin() + last + 1);
                    break;
                }
            }
            else if (dfa.v[cur].state == STATE::final) {
                last_message = dfa.v[cur].messages.begin()->second;
                last = i;

                if (i == n - 1) {
                    cout << s << ": " << last_message << '\n';
                    s.clear();
                }
            }
            else {
                if (i == n - 1) {
                    cout << "ERROR (incomplete)";
                    return;
                }
            }
        }
    }

    for (int i = 0; i < lexemes.size(); ++i) {
        cout << lexemes[i] << ": " << tokens[i] << '\n';
    }
}