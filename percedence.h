#pragma once
#include <bits/stdc++.h>
using namespace std;

// c is not normal char
int percedence(char c) {
    switch (c) {
    case '(':
        return 5;
    case ')':
        return 4;
    case '*':
        return 3;
    case '.':
        return 2;
    case '|':
        return 1;
    default:
        return 0;
    }
}