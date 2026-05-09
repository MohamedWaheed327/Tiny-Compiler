#pragma once
#include <bits/stdc++.h>
using namespace std;

struct production_rule {
    string name;
    vector<string> pattern;

    production_rule() {}
    production_rule(string name, vector<string> pattern)
        : name(name), pattern(pattern) {}

    bool match(vector<string> a) {
        if (a.size() != pattern.size()) {
            return false;
        }

        for (int i = 0; i < a.size(); ++i) {
            if (a[i] != pattern[i]) {
                return false;
            }
        }
        return true;
    }

    bool match(vector<string> a, int l, int r) {
        return match(vector<string>(a.begin() + l, a.begin() + r + 1));
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
    string path = "../assets/grammer_rules (lr, lf removed).yml";
    fstream istr;
    istr.open(path, ios_base::openmode::_S_in);

    vector<production_rule> ret;
    while (!istr.eof()) {
        string line;
        getline(istr, line);

        if (line.empty()) {
            continue;
        }

        auto a = split(line, ':');
        auto b = split(a[1], '|');

        for (auto rule_piece : b) {
            ret.push_back(production_rule(a[0], split(rule_piece, '.')));
        }
    }
    istr.close();

    return ret;
}

map<vector<string>, vector<string>> get_first() {
    string path = "../assets/first.yml";
    fstream istr;
    istr.open(path, ios_base::openmode::_S_in);

    map<vector<string>, vector<string>> ret;
    while (!istr.eof()) {
        string line;
        getline(istr, line);

        if (line.empty()) {
            continue;
        }

        auto a = split(line, ':');
        auto b = split(a[0], '.');

        a[1] = a[1].substr(1, a[1].size() - 2);

        ret[b] = split(a[1], ',');
    }
    istr.close();

    return ret;
}

map<string, vector<string>> get_follow() {
    string path = "../assets/follow.yml";
    fstream istr;
    istr.open(path, ios_base::openmode::_S_in);

    map<string, vector<string>> ret;
    while (!istr.eof()) {
        string line;
        getline(istr, line);

        if (line.empty()) {
            continue;
        }

        auto a = split(line, ':');
        a[1] = a[1].substr(1, a[1].size() - 2);
        ret[a[0]] = split(a[1], ',');
    }
    istr.close();

    return ret;
}

vector<production_rule> production_rules = get_production_rules();
map<vector<string>, vector<string>> first = get_first();
map<string, vector<string>> follow = get_follow();

map<pair<string, string>, production_rule> construct_parsing_table() {
    map<pair<string, string>, production_rule> parsing_table;

    for (auto pr : production_rules) {
        auto st = set<string>(first[pr.pattern].begin(), first[pr.pattern].end());
        for (auto it : st) {
            if (it != "empty") {
                parsing_table[{pr.name, it}] = pr;
            }
        }
        if (st.count("empty")) {
            auto st = follow[pr.name];
            for (auto it : st) {
                if (parsing_table.count({pr.name, it}) == 0 or pr.pattern != vector<string>{"empty"} or parsing_table[{pr.name, it}].name == "synch")
                    parsing_table[{pr.name, it}] = pr;
            }
        }
        else {
            auto st = follow[pr.name];
            for (auto it : st) {
                if (parsing_table.count({pr.name, it}) == 0)
                    parsing_table[{pr.name, it}] = production_rule("synch", {});
            }
        }
    }

    return parsing_table;
}

map<pair<string, string>, production_rule> parsing_table = construct_parsing_table();

/*

x := (x + (x + (y - x / (x / 5- x * 5))));
read x, y, z;
write x, y, z;

if ((x * x = 5 * 5)) then
    f := 5;
    repeat
    until x = 5;

    x := (x + (x + (y - x / (x / 5- x * 5))));
    read x, y, z;
    write x, y, z;

    if x = x then
        x := (x + (x + (y - x / (x / 5- x * 5))));
        read x, y, z;
        write x, y, z;
    else
        x := (x + (x + (y - x / (x / 5- x * 5))));
        read x, y, z;
        write x, y, z;
    end;
    write "nikfndiofn rfdnfonertif", x, y, "jbjbfdf";
end;

*/