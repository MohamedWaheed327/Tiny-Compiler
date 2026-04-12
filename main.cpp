#include <bits/stdc++.h>
using namespace std;
#include "NFA_to_DFA.h"
#include "merge_rules.h"

signed main() {
    cin.tie(0)->sync_with_stdio(0);

    auto nfa = merge_rules();
    // nfa.display();
    auto dfa = NFA_to_DFA(nfa);
    dfa.minimize();
    // dfa.test("if");
    // dfa.test("hellp");
    // dfa.test("55");
    // dfa.test("5a");
    // dfa.test("end");
    // dfa.test("{\"}\"");
    // dfa.test("{dsfodf.;dsgf.d;fsdfjn}");
    // dfa.test("\"{dsfodf.;dsgf.d;fsdfjn}\"");
    // dfa.display();
    dfa.generate_graph_code(); // https://dreampuf.github.io/GraphvizOnline
}
