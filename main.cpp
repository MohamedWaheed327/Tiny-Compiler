#include <bits/stdc++.h>
using namespace std;
#include "NFA_to_DFA.h"
#include "lexical_analyzer.h"
#include "merge_rules.h"

signed main() {
    // cin.tie(0)->sync_with_stdio(0);

    auto nfa = merge_rules();
    auto dfa = NFA_to_DFA(nfa);

    while (true) {
        cout << " >>> ";
        string s;
        getline(cin, s);
        lexer(s, dfa);
        cout << '\n';
    }
}

/* for drawing nfa and dfa */
// https://dreampuf.github.io/GraphvizOnline
