#pragma once
#include "NFA.h"
#include <bits/stdc++.h>
using namespace std;

class DFA {
private:
public:
    struct node {
        STATE state = STATE::normal;
        map<char, int> next;
        set<pair<int, string>> messages;
    };
    vector<node> v;

    DFA() {
        // v.resize(1);
        // v[0].state = STATE::start;
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

    void add_priority_message(int i, int priority, string message) {
        v[i].messages.insert({priority, message});
    }

    void change_state(int i, STATE new_state) {
        v[i].state = new_state;
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
        v[from].next[c] = to;
    }

    int next_state(int cur, char c) {
        return v[cur].next[c];
    }

    void minimize() {
        if (v.empty()) return;

        // Find start state
        int start = -1;
        for (int i = 0; i < (int)v.size(); ++i) {
            if (v[i].state == STATE::start) {
                start = i;
                break;
            }
        }
        if (start == -1) start = 0;

        // Collect alphabet
        set<char> alphabet_set;
        for (auto &nd : v) {
            for (auto &[ch, _] : nd.next)
                alphabet_set.insert(ch);
        }
        vector<char> alphabet(alphabet_set.begin(), alphabet_set.end());

        // Remove unreachable states
        vector<int> vis(v.size(), 0);
        queue<int> q;
        q.push(start);
        vis[start] = 1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &[ch, to] : v[u].next) {
                if (!vis[to]) {
                    vis[to] = 1;
                    q.push(to);
                }
            }
        }

        vector<int> reachable;
        for (int i = 0; i < (int)v.size(); ++i) {
            if (vis[i]) reachable.push_back(i);
        }
        if (reachable.empty()) return;

        // Initial partition: separate by acceptance + messages
        vector<vector<int>> parts;
        map<pair<int, set<pair<int, string>>>, vector<int>> init_groups;

        for (int s : reachable) {
            int is_final = (v[s].state == STATE::final) ? 1 : 0;
            init_groups[{is_final, v[s].messages}].push_back(s);
        }
        for (auto &[_, group] : init_groups)
            parts.push_back(group);

        // Refine partitions
        while (true) {
            vector<int> cls(v.size(), -1);
            for (int i = 0; i < (int)parts.size(); ++i) {
                for (int s : parts[i])
                    cls[s] = i;
            }

            vector<vector<int>> new_parts;
            bool changed = false;

            for (auto &group : parts) {
                map<vector<int>, vector<int>> buckets;

                for (int s : group) {
                    vector<int> sig;
                    sig.reserve(alphabet.size());

                    for (char ch : alphabet) {
                        auto it = v[s].next.find(ch);
                        if (it == v[s].next.end())
                            sig.push_back(-1);
                        else
                            sig.push_back(cls[it->second]);
                    }

                    buckets[sig].push_back(s);
                }

                if ((int)buckets.size() > 1) changed = true;
                for (auto &[_, bucket] : buckets)
                    new_parts.push_back(bucket);
            }

            parts = new_parts;
            if (!changed) break;
        }

        // Map old state -> partition id
        vector<int> old_to_part(v.size(), -1);
        for (int i = 0; i < (int)parts.size(); ++i) {
            for (int s : parts[i])
                old_to_part[s] = i;
        }

        // Put start partition first (important for code that assumes state 0 is the start)
        int start_part = old_to_part[start];
        vector<int> order;
        order.push_back(start_part);
        for (int i = 0; i < (int)parts.size(); ++i) {
            if (i != start_part) order.push_back(i);
        }

        vector<int> part_to_new(parts.size(), -1);
        for (int i = 0; i < (int)order.size(); ++i) {
            part_to_new[order[i]] = i;
        }

        // Build minimized DFA
        vector<node> nv(order.size());

        for (int new_id = 0; new_id < (int)order.size(); ++new_id) {
            int part_id = order[new_id];
            auto &group = parts[part_id];
            int rep = group[0];

            bool has_start = false;
            bool has_final = false;
            bool all_dead = true;
            set<pair<int, string>> msgs;

            for (int s : group) {
                if (v[s].state == STATE::start) has_start = true;
                if (v[s].state == STATE::final) has_final = true;
                if (v[s].state != STATE::dead) all_dead = false;
                msgs.insert(v[s].messages.begin(), v[s].messages.end());
            }

            if (has_start)
                nv[new_id].state = STATE::start;
            else if (has_final)
                nv[new_id].state = STATE::final;
            else if (all_dead)
                nv[new_id].state = STATE::dead;
            else
                nv[new_id].state = STATE::normal;

            nv[new_id].messages = msgs;

            for (auto &[ch, to] : v[rep].next) {
                nv[new_id].next[ch] = part_to_new[old_to_part[to]];
            }
        }

        v = std::move(nv);
    }

    void test(string s) {
        int cur = 0;
        for (auto c : s) {
            cur = next_state(cur, c);
        }

        cout<<'\n' << s << '\n';
        if (v[cur].state == STATE::final) {
            cout << v[cur].messages.begin()->second;
        }
        else {
            cout << "error";
        }
        cout << "\n--------------------------------------------------------";
    }

    void display() {
        for (int i = 0; i < v.size(); ++i) {
            if (v[i].state == STATE::start) cout << "start" << " " << i << "{";
            if (v[i].state == STATE::normal) cout << "normal" << " " << i << "{";
            if (v[i].state == STATE::final) cout << "final" << " " << i << "{";
            if (v[i].state == STATE::dead) cout << "dead" << " " << i << "{";
            for (auto &[ch, nxt] : v[i].next) {
                cout << "\'" << ch << "\'" << ":" << nxt;
                cout << "| ";
            }
            cout << "}";
            if (v[i].messages.size()) {
                cout << " --> \"" << v[i].messages.begin()->second << "\"";
            }
            cout << '\n';
        }
    }

    void generate_graph_code() {
        cout << "digraph G {\n_[style = invisible];\n0[shape = circle, style = filled];\nend[shape = circle, style = filled];\n_ -> 0;\n";
        int n = v.size();
        cout << n << '\n';
        vector<vector<string>> a(n, vector<string>(n));

        for (int i = 0; i < v.size(); ++i) {
            for (auto &[ch, nxt] : v[i].next) {
                a[i][nxt] += string((ch == '\\' or ch == '\"') ? "\\" : "") + ch + ',';
                // cout << i << " -> " << nxt << " [label=\"" << ((ch == '\\' or ch == '\"') ? "\\" : "") << ch << "\"];";
                // cout << '\n';
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (a[i][j].size()) {
                    cout << i << " -> " << j << " [label=\"" << a[i][j] << "\"];" << '\n';
                }
            }
        }

        cout << "\n}";
    }
};
