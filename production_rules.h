#pragma once
#include <bits/stdc++.h>
using namespace std;

struct production_rule {
    string name;
    vector<string> pattern;
    vector<bool> terminal;
    int priority;

    production_rule() {}
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

vector<production_rule> production_rules = get_production_rules();