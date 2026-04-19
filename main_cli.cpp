#include <bits/stdc++.h>
using namespace std;
#include "get_DFA.h"
#include "lexical_analyzer.h"

signed main() {
    auto dfa = get_DFA();

    while (true) {
        cout << " >>> ";
        string s;
        getline(cin, s);
        cout << lexer(s, dfa) << '\n';
    }
}