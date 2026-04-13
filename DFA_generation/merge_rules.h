#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "NFA.h"
#include "RE_to_NFA.h"
#include "regular_expressions.h"

NFA merge_rules() {
    vector<int> pref{1};
    vector<NFA> nfas;
    for (auto re : regular_expressions) {
        nfas.push_back(RE_to_NFA(re));
        pref.push_back(nfas.back().v.size());
    }

    partial_sum(pref.begin(), pref.end(), pref.begin());
    
    NFA nfa;
    for (int i = 0; i < nfas.size(); ++i) {
        nfas[i].shift(pref[i]);
        
        int n = nfas[i].v.size();
        for (int j = 0; j < n; ++j) {
            nfa.add_state(nfas[i].v[j]);
            if (j == 0) {
                nfa.v.back().state = STATE::normal;
                nfa.add_edge(0, nfa.v.size() - 1, '_');
            }
        }
    }

    return nfa;
}