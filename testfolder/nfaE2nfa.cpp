#include <bits/stdc++.h>
using namespace std;
#include "NFA_withE_functions.cpp"

typedef struct NFA
{
    set<int> start;
    set<int> fin;
    map<pair<int, char>, set<int>> mp;
} nfa_wo_E;

void print_nfa_wo_E(nfa_wo_E n)
{
    cout << "Start: ";
    for(auto it:n.start)
    {
        cout<<it<<" ";
    }
    cout << endl;
    cout << "Final: ";
    for(auto it:n.fin)
    {
        cout << it << " ";
    }
    cout << endl;
    for(auto it:n.mp)
    {
        cout << it.first.first << " , ";
        cout << it.first.second << " = ";

        for (auto i: it.second) {
            cout << i << " ";
        }

        cout << endl;
    }
}


set<int> E_closure(nfa n, int state) {
    if (state == 0)
        return {0};

    set<int> s, visited;
    queue<int> q;
    q.push(state);

    while (!q.empty()) {

        int state_t = q.front();
        q.pop();
        s.insert(state_t);

        // q.push(state_t);
        visited.insert(state_t);

        vector<int> states = n.mp[state_t]['E'];

        for (auto i:states) {
            s.insert(i);
            if (visited.find(i) == visited.end())
                q.push(i);
        }
    }

    return s;
}

nfa_wo_E nfaE2nfa(nfa n) {
    nfa_wo_E nfa_wo_e;
    
    set<int> start_s = n.start, fin_s = n.fin;

    for (auto i:start_s)  {
        nfa_wo_e.start.insert(i);
    }

    for (auto i:fin_s)  {
        set<int> temp_s = E_closure(n, i);
        for (auto j:temp_s)
            nfa_wo_e.fin.insert(j);
    }


    for (auto i:n.mp) {
        set<int> set1 = E_closure(n, i.first);
            
        //a
        set<int> set2_a;
        for (auto j:set1) {
            vector<int> value = n.mp[j]['a'];
            for (auto k:value)
                set2_a.insert(k);
        }

        set<int> set3_a;
        for (auto j:set2_a) {
            set<int> sett_a = E_closure(n, j);
            for (auto k: sett_a)
                set3_a.insert(k);
        }
        
        nfa_wo_e.mp.insert({make_pair(i.first, 'a'), set3_a});


        //b
        set<int> set2_b;
        for (auto j:set1) {
            vector<int> value = n.mp[j]['b'];
            for (auto k:value)
                set2_b.insert(k);
        }

        set<int> set3_b;
        for (auto j:set2_b) {
            set<int> sett_b = E_closure(n, j);
            for (auto k: sett_b)
                set3_b.insert(k);
        }
        
        nfa_wo_e.mp.insert({make_pair(i.first, 'b'), set3_b});
    }

    return nfa_wo_e;
}