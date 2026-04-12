#pragma once
#include <bits/stdc++.h>
using namespace std;

enum STATE {
    start = 1,
    normal = 2,
    final = 4,
    dead = 8,
};

class NFA {
private:
public:
    struct node {
        STATE state = STATE::normal;
        map<char, vector<int>> next;
        // string message = "";
        // int priority = 0;

        set<pair<int, string>> messages;
    };

    vector<node> v;

    NFA() {
        // cout <<
        v.resize(1);
        v[0].state = STATE::start;
    }

    int add_state(STATE state) {
        int num = v.size();
        v.push_back(node());
        v[num].state = state;
        return num;
    }

    int add_state(node state) {
        v.push_back(state);
        return v.size() - 1;
    }

    void change_state(int i, STATE new_state) {
        v[i].state = new_state;
    }

    // void change_message(int i, string new_message) {
    //     v[i].message = new_message;
    // }

    // void change_priority(int i, int new_priority) {
    //     v[i].priority = new_priority;
    // }

    void shift(int sh) {
        for (auto &[state, nxt, messages] : v) {
            for (auto &[f, s] : nxt) {
                for (auto &it : s) {
                    it += sh;
                }
            }
        }

        // while (sh--) {
        //     v.insert(v.begin(), node());
        // }
    }

    vector<int> get_final_states() {
        vector<int> ret;
        for (int i = 0; i < v.size(); ++i) {
            if (v[i].state == STATE::final) {
                ret.push_back(i);
            }
        }
        return ret;
    }

    void add_edge(int from, int to, char c) {
        v[from].next[c].push_back(to);
    }

    void display() {
        for (int i = 0; i < v.size(); ++i) {
            if (v[i].messages.size()) {
                cout << "\"" << v[i].messages.begin()->second << "\"";
            }
            if (v[i].state == STATE::start) cout << "start" << " " << i << "{";
            if (v[i].state == STATE::normal) cout << "normal" << " " << i << "{";
            if (v[i].state == STATE::final) cout << "final" << " " << i << "{";
            for (auto &[ch, nxt] : v[i].next) {
                cout << "\'" << ch << "\'" << ":";
                for (auto x : nxt) {
                    cout << x << ",";
                }
                cout << "| ";
            }
            cout << "}" << '\n';
        }
    }

    void print_graph_code() {
        
    }
};
