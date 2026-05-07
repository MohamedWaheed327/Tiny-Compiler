#pragma once
#include <bits/stdc++.h>
using namespace std;

struct production_rule {
    string name;
    vector<string> pattern;
    vector<bool> terminal;
    int priority;

    production_rule() {}
    production_rule(string name, vector<string> pattern) : name(name), pattern(pattern) {
    }

    production_rule(string name, vector<string> pattern, vector<bool> terminal, int priority)
        : name(name), pattern(pattern), terminal(terminal), priority(priority) {}

    bool match(vector<string> a, vector<bool> b) {
        if (a.size() != b.size() or a.size() != pattern.size()) {
            return false;
        }

        for (int i = 0; i < a.size(); ++i) {
            if (a[i] != pattern[i] or b[i] != terminal[i]) {
                return false;
            }
        }
        return true;
    }

    bool match(vector<string> a, int l1, int r1, vector<bool> b, int l2, int r2) {
        return match(vector<string>(a.begin() + l1, a.begin() + r1 + 1), vector<bool>(b.begin() + l2, b.begin() + r2 + 1));
    }
};

vector<string> split(string s, char c) { // spaces trimmed
    s = c + s;
    vector<string> ret;
    for (auto it : s) {
        if (it == c) {
            ret.push_back("");
        }
        else {
            if (it != ' ') {
                ret.back().push_back(it);
            }
        }
    }
    return ret;
}

vector<production_rule> get_production_rules() {
    string path = "../assets/grammer_rules.yml";
    fstream istr;
    istr.open(path, ios_base::openmode::_S_in);

    int priority = 0;
    vector<production_rule> ret;
    while (!istr.eof()) {
        string line;
        getline(istr, line);

        if (line.empty()) {
            priority++;
            continue;
        }

        auto a = split(line, ':');
        auto b = split(a[1], '|');

        for (auto rule_piece : b) {
            auto c = split(rule_piece, '.');
            vector<bool> d;
            for (auto it : c) {
                d.push_back(islower(it[0]));
            }
            ret.push_back(production_rule(a[0], c, d, priority));
        }
    }
    istr.close();

    return ret;
}

vector<production_rule> remove_left_recursion() {
    auto ret = get_production_rules();

    set<string> left_recursion;
    for (int i = 0; i < ret.size(); ++i) {
        if (ret[i].name == ret[i].pattern[0]) {
            left_recursion.insert(ret[i].name);
        }
    }

    for (int i = 0; i < ret.size(); ++i) {
        if (left_recursion.count(ret[i].name)) {
            if (ret[i].name == ret[i].pattern[0]) {
                ret[i].pattern.push_back(ret[i].name + '_');
                ret[i].pattern.erase(ret[i].pattern.begin());
                ret[i].name += '_';
            }
            else {
                ret[i].pattern.push_back(ret[i].name + '_');
            }
        }
    }

    for (auto lr : left_recursion) {
        ret.push_back(production_rule(lr + '_', vector<string>{string("empty")}, vector<bool>{true}, 0));
    }
    return ret;
}

map<vector<string>, set<string>> compute_first(vector<production_rule> prs) {
    map<string, vector<production_rule>> mp;
    for (auto pr : prs) {
        mp[pr.name].push_back(pr);
    }

    map<vector<string>, set<string>> dp;
    function<set<string>(vector<string>)> get_first = [&](const vector<string> rule) {
        if (dp.count(rule)) return dp[rule];
        if (islower(rule[0][0])) return dp[rule] = set<string>{rule[0]};

        set<string> ret;
        for (auto pr : mp[rule[0]]) {
            auto get = get_first(pr.pattern);
            ret.insert(get.begin(), get.end());
        }

        if (ret.count("empty")) {
            auto new_rule = rule;
            new_rule.erase(new_rule.begin());
            if (new_rule.size()) {
                ret.erase("empty");
                auto get = get_first(new_rule);
                ret.insert(get.begin(), get.end());
            }
        }

        return dp[rule] = ret;
    };

    for (auto pr : prs) {
        get_first({pr.name});
    }

    return dp;
}

map<string, set<string>> compute_follow(vector<production_rule> prs) {
    map<string, vector<production_rule>> rules;
    set<string> non_terminals;

    for (auto pr : prs) {
        rules[pr.name].push_back(pr);
        non_terminals.insert(pr.name);
    }

    map<string, set<string>> first;

    for (auto pr : prs) {
        for (auto x : pr.pattern) {
            if (islower(x[0])) {
                first[x].insert(x);
            }
            else {
                non_terminals.insert(x);
            }
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;

        for (auto pr : prs) {
            bool all_empty = true;

            for (auto x : pr.pattern) {
                int before = first[pr.name].size();

                for (auto y : first[x]) {
                    if (y != "empty") {
                        first[pr.name].insert(y);
                    }
                }

                if (!first[x].count("empty")) {
                    all_empty = false;
                    break;
                }

                if (first[pr.name].size() != before) {
                    changed = true;
                }
            }

            if (all_empty) {
                int before = first[pr.name].size();
                first[pr.name].insert("empty");
                if (first[pr.name].size() != before) {
                    changed = true;
                }
            }
        }
    }

    auto first_of_sequence = [&](vector<string> seq) {
        set<string> ret;

        if (seq.empty()) {
            ret.insert("empty");
            return ret;
        }

        bool all_empty = true;

        for (auto x : seq) {
            for (auto y : first[x]) {
                if (y != "empty") {
                    ret.insert(y);
                }
            }

            if (!first[x].count("empty")) {
                all_empty = false;
                break;
            }
        }

        if (all_empty) {
            ret.insert("empty");
        }

        return ret;
    };

    map<string, set<string>> follow;

    for (auto nt : non_terminals) {
        follow[nt];
    }

    if (!prs.empty()) {
        follow[prs[0].name].insert("$");
    }

    changed = true;
    while (changed) {
        changed = false;

        for (auto pr : prs) {
            for (int i = 0; i < pr.pattern.size(); ++i) {
                if (isupper(pr.pattern[i][0])) {
                    string B = pr.pattern[i];

                    vector<string> beta;
                    for (int j = i + 1; j < pr.pattern.size(); ++j) {
                        beta.push_back(pr.pattern[j]);
                    }

                    auto first_beta = first_of_sequence(beta);

                    int before = follow[B].size();

                    for (auto x : first_beta) {
                        if (x != "empty") {
                            follow[B].insert(x);
                        }
                    }

                    if (first_beta.count("empty")) {
                        for (auto x : follow[pr.name]) {
                            follow[B].insert(x);
                        }
                    }

                    if (follow[B].size() != before) {
                        changed = true;
                    }
                }
            }
        }
    }

    return follow;
}

auto rules = remove_left_recursion();
auto first = compute_first(rules);
auto follow = compute_follow(rules);

map<pair<string, string>, production_rule> construct_parsing_table() {
    map<pair<string, string>, production_rule> parsing_table;

    for (auto pr : rules) {
        auto st = first[pr.pattern];
        for (auto it : st) {
            parsing_table[{pr.name, it}] = pr;
        }
        if (st.count("empty")) {
            auto st = follow[pr.name];
            for (auto it : st) {
                if (parsing_table.count({pr.name, it}) == 0 or pr.pattern != vector<string>{"empty"})
                    parsing_table[{pr.name, it}] = pr;
            }
        }
    }

    return parsing_table;
}

auto pt = construct_parsing_table();

vector<production_rule> production_rules = get_production_rules();