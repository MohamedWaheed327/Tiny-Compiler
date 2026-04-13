#pragma once
#include "get_DFA.h"
#include <bits/stdc++.h>
using namespace std;

void lexer(string s, DFA &dfa) {
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
                last_message = dfa.v[cur].message;
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
}