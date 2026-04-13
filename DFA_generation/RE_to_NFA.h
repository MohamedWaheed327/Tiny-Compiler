#pragma once
#include "NFA.h"
#include "regular_expression.h"
#include <bits/stdc++.h>
using namespace std;

NFA RE_to_NFA(regular_expression re) {
    int n = re.expression.size(), cnt = 0, break_idx1 = -1, break_idx2 = -1, break_idx3 = -1;

    if (n == 1) {
        NFA nfa;
        nfa.add_state(NFA::node{state : STATE::final, messages : set{pair{re.priority, re.message}}});
        nfa.add_edge(0, 1, re.expression[0]);
        return nfa;
    }

    for (int i = 0; i < n; ++i) {
        if (re.is_char[i]) continue;

        if (re.expression[i] == '(') {
            cnt++;
        }
        else if (re.expression[i] == ')') {
            cnt--;
        }

        if (cnt == 0) {
            if (re.expression[i] == '|') {
                if (!~break_idx1) break_idx1 = i;
            }
            else if (re.expression[i] == '.') {
                if (!~break_idx2) break_idx2 = i;
            }
            else if (re.expression[i] == '*') {
                if (!~break_idx3) break_idx3 = i;
            }
        }
    }

    // cout << break_idx1 << " " << break_idx2 << " " << break_idx3 << '\n';
    NFA nfa;
    // cout << "hi1";
    if (~break_idx1) { // |
        auto [a, b] = re.break_at(break_idx1);
        auto nfa_a = RE_to_NFA(a);
        auto nfa_b = RE_to_NFA(b);
        auto final_a = nfa_a.get_final_states();
        auto final_b = nfa_b.get_final_states();

        // nfa_a += 1
        nfa_a.shift(1);
        // nfa_b += nfa_a.v.size() + 1
        nfa_b.shift(nfa_a.v.size() + 1);

        for (auto state : nfa_a.v) {
            nfa.add_state(state);
        }
        for (auto state : nfa_b.v) {
            nfa.add_state(state);
        }
        nfa.add_state(STATE::final);

        nfa.add_edge(0, 1, '_');
        nfa.add_edge(0, nfa_a.v.size() + 1, '_');

        nfa.v[1].state = STATE::normal;
        nfa.v[(int)nfa_a.v.size() + 1].state = STATE::normal;

        for (auto i : final_a) {
            nfa.add_edge(i + 1, nfa.v.size() - 1, '_');
            nfa.v[i + 1].state = STATE::normal;
            nfa.v.back().messages.insert(nfa.v[i + 1].messages.begin(), nfa.v[i + 1].messages.end());
            nfa.v[i + 1].messages.clear();
        }
        for (auto i : final_b) {
            nfa.add_edge(nfa_a.v.size() + i + 1, nfa.v.size() - 1, '_');
            nfa.v[nfa_a.v.size() + i + 1].state = STATE::normal;
            nfa.v.back().messages.insert(nfa.v[nfa_a.v.size() + i + 1].messages.begin(), nfa.v[nfa_a.v.size() + i + 1].messages.end());
            nfa.v[nfa_a.v.size() + i + 1].messages.clear();
        }
    }
    else if (~break_idx2) { // .
        // cout << "hi3";
        auto [a, b] = re.break_at(break_idx2);
        // cout << break_idx2 << '\n';
        // a.display();
        // b.display();

        auto nfa_a = RE_to_NFA(a);
        auto nfa_b = RE_to_NFA(b);
        auto final_a = nfa_a.get_final_states();
        auto final_b = nfa_b.get_final_states();

        // nfa_a += 1
        nfa_a.shift(1);
        // nfa_b += nfa_a.v.size() + 1
        nfa_b.shift(nfa_a.v.size() + 1);

        for (auto state : nfa_a.v) {
            nfa.add_state(state);
        }
        for (auto state : nfa_b.v) {
            nfa.add_state(state);
        }
        nfa.add_state(STATE::final);

        nfa.add_edge(0, 1, '_');
        // nfa.add_edge(0, nfa_a.v.size() + 1, '_');

        nfa.v[1].state = STATE::normal;
        nfa.v[(int)nfa_a.v.size() + 1].state = STATE::normal;

        for (auto i : final_a) {
            nfa.add_edge(i + 1, nfa_a.v.size() + 1, '_');
            nfa.v[i + 1].state = STATE::normal;
            nfa.v[i + 1].messages.clear();
        }
        for (auto i : final_b) {
            nfa.add_edge(nfa_a.v.size() + i + 1, nfa.v.size() - 1, '_');
            nfa.v[nfa_a.v.size() + i + 1].state = STATE::normal;
            nfa.v.back().messages.insert(nfa.v[nfa_a.v.size() + i + 1].messages.begin(), nfa.v[nfa_a.v.size() + i + 1].messages.end());
            nfa.v[nfa_a.v.size() + i + 1].messages.clear();
        }
    }
    else if (~break_idx3) { // *
        re.pop_back();
        auto nfa_base = RE_to_NFA(re);
        auto final_base = nfa_base.get_final_states();

        // nfa_a += 1
        nfa_base.shift(1);

        for (auto state : nfa_base.v) {
            nfa.add_state(state);
        }
        nfa.add_state(STATE::final);

        nfa.add_edge(0, nfa.v.size() - 1, '_');
        nfa.add_edge(nfa.v.size() - 1, 1, '_');

        nfa.v[1].state = STATE::normal;
        for (auto i : final_base) {
            nfa.add_edge(i + 1, nfa.v.size() - 1, '_');
            nfa.v[i + 1].state = STATE::normal;
            nfa.v.back().messages.insert(nfa.v[i + 1].messages.begin(), nfa.v[i + 1].messages.end());
            nfa.v[i + 1].messages.clear();
        }
    }
    else {
        re.pop_front();
        re.pop_back();
        return RE_to_NFA(re);
    }

    return nfa;
}
