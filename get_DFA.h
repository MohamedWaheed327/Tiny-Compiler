#pragma once
#include <bits/stdc++.h>
using namespace std;

enum STATE {
    start,
    normal,
    final,
    dead,
};

class DFA {
public:
    struct node {
        STATE state = STATE::normal;
        map<char, int> next;
        string message;
    };
    vector<node> v;

    int add_state(node state) {
        v.push_back(state);
        return v.size() - 1;
    }

    int next_state(int cur, char c) {
        if (v[cur].next.count(c) == 0) c = '#';
        return v[cur].next[c];
    }
};

DFA get_DFA() {
    DFA dfa;
    fstream istr;
    istr.open("../DFA_construction.txt", ios_base::openmode::_S_in);

    while (!istr.eof()) {
        string line;
        getline(istr, line);

        istringstream iss(line);

        int state;
        string msg;
        map<char, int> mp;
        char c;
        int nxt;

        iss >> state >> msg;
        while (iss >> c >> nxt) {
            mp[c] = nxt;
        }

        if (msg.empty()) break;
        DFA::node node{state : (STATE)state, next : mp, message : msg};
        dfa.add_state(node);
    }
    istr.close();
    return dfa;
}