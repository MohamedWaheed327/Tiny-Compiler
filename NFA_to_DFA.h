#pragma once
#include "DFA.h"
#include "NFA.h"
#include <bits/stdc++.h>
using namespace std;

DFA NFA_to_DFA(NFA nfa) {
    int n = nfa.v.size();
    vector<set<int>> lambda_closure(n);

    set<char> char_set;
    for (int i = 0; i < n; ++i) {
        for (auto [c, _] : nfa.v[i].next) {
            if (c != '_') {
                char_set.insert(c);
            }
        }

        vector<int> vis(n, 0);
        queue<int> q;
        q.push(i);
        vis[i] = true;

        while (q.size()) {
            int node = q.front();
            q.pop();

            if (nfa.v[node].next.count('_')) {
                for (auto nxt : nfa.v[node].next['_']) {
                    if (!vis[nxt]) {
                        vis[nxt] = true;
                        q.push(nxt);
                    }
                }
            }
        }

        for (int j = 0; j < n; ++j) {
            if (vis[j]) {
                lambda_closure[i].insert(j);
            }
        }
    }

    // for (auto &it : char_set) {
    //     cout << it << '\n';
    // }
    // for (int i = 0; i < n; ++i) {
    //     cout << i << ": ";
    //     for (auto &it : lambda_closure[i]) {
    //         cout << it << " ";
    //     }
    //     cout << '\n';
    // }

    vector<pair<set<int>, map<char, set<int>>>> Td;
    auto get = [&](set<int> s, char c) {
        set<int> s2;
        for (auto x : s) {
            for (auto it : nfa.v[x].next[c]) {
                s2.insert(it);
            }
        }

        set<int> s3;
        for (auto x : s2) {
            for (auto it : lambda_closure[x]) {
                s3.insert(it);
            }
        }
        return s3;
    };

    map<set<int>, int> existant_sets;
    existant_sets[lambda_closure[0]] = 0;

    Td.push_back(pair{lambda_closure[0], map<char, set<int>>()});
    for (int i = 0; i < Td.size(); ++i) {
        for (auto c : char_set) {
            Td[i].second[c] = get(Td[i].first, c);
            if (!existant_sets.count(Td[i].second[c])) {
                existant_sets[Td[i].second[c]] = Td.size();
                Td.push_back(pair{Td[i].second[c], map<char, set<int>>()});
            }
        }
    }

    DFA dfa;
    for (int i = 0; i < Td.size(); ++i) {
        if (i == 0) {
            dfa.add_state(STATE::start);
        }
        else {
            dfa.add_state(STATE::normal);
        }

        for (auto [ch, ch_set] : Td[i].second) {
            dfa.add_edge(i, existant_sets[ch_set], ch);
        }
    }

    auto final_ = nfa.get_final_states();
    for (int i = 0; i < Td.size(); ++i) {
        for (auto fs : final_) {
            if (Td[i].first.count(fs)) {
                dfa.change_state(i, STATE::final);
                if (nfa.v[fs].messages.size()) {
                    auto [p, m] = *nfa.v[fs].messages.begin();
                    dfa.add_priority_message(i, p, m);
                }
            }
        }
    }

    for (int i = 0; i < Td.size(); ++i) {
        if (dfa.v[i].state != STATE::final) {
            set<int> s;
            for (auto [_, x] : dfa.v[i].next) {
                s.insert(x);
            }

            if (s.size() == 1 && *s.begin() == i) {
                dfa.change_state(i, STATE::dead);
            }
        }
    }

    return dfa;
}