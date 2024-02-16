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
    for (auto it : n.start)
    {
        cout << it << " ";
    }
    cout << endl;
    cout << "Final: ";
    for (auto it : n.fin)
    {
        cout << it << " ";
    }
    cout << endl;
    for (auto it : n.mp)
    {
        cout << it.first.first << " , ";
        cout << it.first.second << " = ";

        for (auto i : it.second)
        {
            cout << i << " ";
        }

        cout << endl;
    }
}

set<int> E_closure(nfa n, int state)
{
    if (state == 0)
        return {0};

    set<int> s, visited;
    queue<int> q;
    q.push(state);

    while (!q.empty())
    {

        int state_t = q.front();
        q.pop();
        s.insert(state_t);

        // q.push(state_t);
        visited.insert(state_t);

        vector<int> states = n.mp[state_t]['E'];

        for (auto i : states)
        {
            s.insert(i);
            if (visited.find(i) == visited.end())
                q.push(i);
        }
    }

    return s;
}

nfa_wo_E nfaE2nfa(nfa n)
{
    nfa_wo_E nfa_wo_e;

    set<int> start_s = n.start, fin_s = n.fin;

    for (auto i : start_s)
    {
        nfa_wo_e.start.insert(i);
    }

    for (auto i : fin_s)
    {
        set<int> temp_s = E_closure(n, i);
        for (auto j : temp_s)
            nfa_wo_e.fin.insert(j);
    }

    for (auto i : n.mp)
    {
        set<int> set1 = E_closure(n, i.first);

        // a
        set<int> set2_a;
        for (auto j : set1)
        {
            vector<int> value = n.mp[j]['a'];
            for (auto k : value)
                set2_a.insert(k);
        }

        set<int> set3_a;
        for (auto j : set2_a)
        {
            set<int> sett_a = E_closure(n, j);
            for (auto k : sett_a)
                set3_a.insert(k);
        }

        nfa_wo_e.mp.insert({make_pair(i.first, 'a'), set3_a});

        // b
        set<int> set2_b;
        for (auto j : set1)
        {
            vector<int> value = n.mp[j]['b'];
            for (auto k : value)
                set2_b.insert(k);
        }

        set<int> set3_b;
        for (auto j : set2_b)
        {
            set<int> sett_b = E_closure(n, j);
            for (auto k : sett_b)
                set3_b.insert(k);
        }

        nfa_wo_e.mp.insert({make_pair(i.first, 'b'), set3_b});
    }

    return nfa_wo_e;
}

int main()
{
    stack<nfa> st;
    string seq;
    cin >> seq;
    int x = seq.size();
    for (int i = 0; i < x; i++)
    {
        if (seq[i] == 'a' || seq[i] == 'b')
            st.push(character(seq[i]));

        else if ((seq[i] == '*') || (seq[i] == '+') || (seq[i] == '?'))
        {
            nfa n = st.top();
            st.pop();
            if (seq[i] == '?')
            {
                n = condition(n);
                st.push(n);
            }

            else
            {
                n = closure(n, seq[i]);
                st.push(n);
            }
        }

        else if (seq[i] == '.' || (seq[i] == '|'))
        {
            nfa n2 = st.top();
            st.pop();
            nfa n1 = st.top();
            st.pop();

            if (seq[i] == '|')
            {
                nfa n = or_union(n1, n2);
                st.push(n);
            }

            else
            {
                nfa n = concatenate(n1, n2);
                st.push(n);
            }
        }
    }

    nfa n = st.top();
    st.pop();
    nfa_wo_E ans = nfaE2nfa(n);

    unordered_set<int> fstates;
    unordered_map<int, vector<vector<int>>> transition;
    for (auto it : ans.fin)
    {
        fstates.insert(it - 1);
    }
    int start_state;
    for (auto it : ans.start)
    {
        start_state = it - 1;
    }

    int no_of_states = ans.mp.size() / 2;

    for (auto it : ans.mp)
    {
        vector<int> v;
        for (auto t_states : it.second)
        {
            if (t_states)
            {
                v.push_back(t_states - 1);
            }
        }
        transition[it.first.first - 1].push_back(v);
    }
    vector<int> cur{start_state};
    vector<vector<int>> discovering;
    discovering.push_back(cur);
    map<vector<int>, vector<vector<int>>> dfa;
    set<vector<int>> dfa_fstates;
    for (int i = 0; i < discovering.size(); i++)
    {
        cur = discovering[i];
        if (dfa.find(cur) == dfa.end())
        {
            set<int> s0, s1;
            vector<int> v0, v1;
            for (int j = 0; j < cur.size(); j++)
            {
                for (int k = 0; k < transition[cur[j]][0].size(); k++)
                {
                    s0.insert(transition[cur[j]][0][k]);
                }
                for (int k = 0; k < transition[cur[j]][1].size(); k++)
                {
                    s1.insert(transition[cur[j]][1][k]);
                }
            }
            for (auto it : s0)
            {
                v0.push_back(it);
            }
            for (auto it : s1)
            {
                v1.push_back(it);
            }

            discovering.push_back(v0);
            discovering.push_back(v1);

            for (auto it : fstates)
            {
                if (s0.find(it) != s0.end())
                {
                    dfa_fstates.insert(v0);
                    break;
                }
            }
            for (auto it : fstates)
            {
                if (s1.find(it) != s1.end())
                {
                    dfa_fstates.insert(v1);
                    break;
                }
            }
            dfa[cur].push_back(v0);
            dfa[cur].push_back(v1);
        }
    }
    // print_nfa(n);
    // print_nfa_wo_E(ans);
    // for (auto it : transition)
    // {
    //     cout << it.first << " - [";
    //     for (int i = 0; i < it.second[0].size(); i++)
    //     {
    //         cout << it.second[0][i] << " ";
    //     }
    //     cout << "] [";
    //     for (int i = 0; i < it.second[1].size(); i++)
    //     {
    //         cout << it.second[1][i] << " ";
    //     }
    //     cout << "] \n";
    // }
    // cout << start_state;

    for (auto it : dfa)
    {
        cout << "[";
        for (int i = 0; i < it.first.size(); i++)
        {
            cout << it.first[i] << " ";
        }
        cout << "] -> {";
        for (int i = 0; i < it.second[0].size(); i++)
        {
            cout << it.second[0][i] << " ";
        }
        cout << "} - {";
        for (int i = 0; i < it.second[1].size(); i++)
        {
            cout << it.second[1][i] << " ";
        }
        cout << "}" << endl;
    }
    cout << "final states:" << endl;
    for (auto it : dfa_fstates)
    {
        cout << "{";
        for (int i = 0; i < it.size(); i++)
        {
            cout << it[i] << " ";
        }
        cout << "} ";
    }
    cout << endl;
    string inp;
    cin >> inp;
    // string inp = "aabaaaaaaab";
    for (int i = 0; i < inp.size(); i++)
    {
        inp[i] = (char)(inp[i] - 'a' + '0');
    }

    vector<int> dfacur{start_state};
    for (int i = 0; i < inp.size(); i++)
    {
        dfacur = dfa[dfacur][inp[i] - '0'];
    }
    if (dfa_fstates.find(dfacur) != dfa_fstates.end())
    {
        cout << "accepted" << endl;
    }
    else
    {
        cout << "rejected" << endl;
    }
}
