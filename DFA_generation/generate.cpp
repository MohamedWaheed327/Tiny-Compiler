#include <bits/stdc++.h>
using namespace std;
#include "NFA_to_DFA.h"
#include "merge_rules.h"

void write_file(string path, string content) {
    fstream file;
    file.open(path, ios_base::openmode::_S_out | ios_base::openmode::_S_trunc);
    file.write(content.c_str(), content.size());
    file.close();
}

signed main() {
    auto nfa = merge_rules();
    auto a = nfa.generate_graph_code();
    auto dfa = NFA_to_DFA(nfa);
    auto b = dfa.generate_graph_code();
    auto c = dfa.generate_cpp_code();
    write_file("nfa_graphviz_representation.txt", a);
    write_file("dfa_graphviz_representation.txt", b);
    write_file("dfa_representation.txt", c);
}

/* for drawing nfa and dfa */
// https://dreampuf.github.io/GraphvizOnline
