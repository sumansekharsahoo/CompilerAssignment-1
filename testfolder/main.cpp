#include <bits/stdc++.h>
#include <fstream>
using namespace std;

typedef struct NFA
{
    set<int> start;
    set<int> fin;
    map<pair<int, char>, set<int>> mp;
} nfa_wo_E;

typedef struct NFA_t
{
    set<int> start;
    set<int> fin;
    map<int, map<char, vector<int>>> mp;
} nfa;

typedef struct DFA_t
{
    vector<int> start_state;
    map<vector<int>, vector<vector<int>>> dfa;
    set<vector<int>> final_states;
} dfa;

// NFA_withE_functions

nfa character(char c)
{
    nfa n;
    n.start.insert(1);
    n.fin.insert(2);

    char alt;
    if (c == 'a')
        alt = 'b';
    else
        alt = 'a';

    map<char, vector<int>> temp;
    temp[c] = {2};
    temp[alt] = {0};
    temp['E'] = {1};
    n.mp[1] = temp;
    temp.clear();
    temp[c] = {0};
    temp[alt] = {0};
    temp['E'] = {2};
    n.mp[2] = temp;

    return n;
}

nfa offset(nfa n, int offset)
{
    set<int> new_start;
    for (auto it : n.start)
    {
        new_start.insert(it + offset);
    }

    set<int> new_final;
    for (auto it : n.fin)
    {
        new_final.insert(it + offset);
    }

    map<int, map<char, vector<int>>> new_mp;
    for (auto it : n.mp)
    {
        int new_key = it.first + offset;
        map<char, vector<int>> temp_map;
        for (auto it2 : it.second)
        {
            vector<int> temp;
            for (auto it3 : it2.second)
            {
                if (it3 != 0)
                    temp.push_back(it3 + offset);
                else
                    temp.push_back(it3);
            }
            temp_map[it2.first] = temp;
        }

        new_mp[new_key] = temp_map;
    }

    n.start = new_start;
    n.fin = new_final;
    n.mp = new_mp;

    return n;
}

nfa or_union(nfa n1, nfa n2)
{
    int num = n1.mp.size() + n2.mp.size();
    nfa n_one = offset(n1, n2.mp.size());
    num++;
    nfa n;
    n.start.insert(num);

    n.fin = n_one.fin;

    for (auto i : n2.fin)
    {
        n.fin.insert(i);
    }
    n.mp = n_one.mp;
    for (auto it : n2.mp)
    {
        n.mp[it.first] = it.second;
    }
    for (auto i : n_one.start)
    {
        n.mp[num]['E'].push_back(i);
    }

    for (auto i : n2.start)
    {
        n.mp[num]['E'].push_back(i);
    }

    n.mp[num]['E'].push_back(num);
    n.mp[num]['a'] = {0};
    n.mp[num]['b'] = {0};

    return n;
}

nfa closure(nfa n, char c)
{
    if (c == '*')
    {
        for (auto i : n.start)
        {
            n.fin.insert(i);
        }
    }

    for (auto i : n.fin)
    {
        for (auto j : n.start)
        {
            n.mp[i]['E'].push_back(j);
        }
    }
    return n;
}

nfa condition(nfa n)
{
    for (auto i : n.start)
    {
        n.fin.insert(i);
    }
    return n;
}

nfa concatenate(nfa n1, nfa n2)
{
    n1 = offset(n1, n2.mp.size());
    nfa n;
    n.start = n1.start;
    n.fin = n2.fin;
    n.mp = n1.mp;
    for (auto it : n2.mp)
    {
        n.mp[it.first] = it.second;
    }
    for (auto it : n1.fin)
    {
        for (auto it2 : n2.start)
        {
            n.mp[it]['E'].push_back(it2);
        }
    }
    return n;
}

void print_nfa(nfa n)
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
        cout << it.first << ": ";
        for (auto it2 : it.second)
        {
            cout << it2.first << "->";
            for (auto it3 : it2.second)
            {
                cout << it3 << " ";
            }
            cout << endl;
        }
    }
}

nfa build_nfa(string seq)
{
    stack<nfa> st;
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
    return n;
}

// NFA_epsilon -> NFA_wo_epsilon functions

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

// NFA_wo_epsilon -> DFA functions

dfa nfa_to_dfa(nfa_wo_E nfa)
{
    dfa result;
    unordered_set<int> fstates;
    unordered_map<int, vector<vector<int>>> transition;
    for (auto it : nfa.fin)
    {
        fstates.insert(it - 1);
    }
    int startstate;
    for (auto it : nfa.start)
    {
        startstate = it - 1;
    }

    int no_of_states = nfa.mp.size() / 2;

    for (auto it : nfa.mp)
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

    vector<int> cur{startstate};
    vector<vector<int>> discovering;
    discovering.push_back(cur);
    map<vector<int>, vector<vector<int>>> dfa_map;
    set<vector<int>> dfa_fstates;
    for (int i = 0; i < discovering.size(); i++)
    {
        cur = discovering[i];
        if (dfa_map.find(cur) == dfa_map.end())
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
            dfa_map[cur].push_back(v0);
            dfa_map[cur].push_back(v1);
        }
    }
    result.dfa = dfa_map;
    result.start_state.push_back(startstate);
    result.final_states = dfa_fstates;

    return result;
}

void print_dfa(dfa mydfa)
{
    for (auto it : mydfa.dfa)
    {
        cout << "{";
        for (auto nums : it.first)
        {
            cout << nums << ",";
        }
        cout << "} -> {";
        for (auto nums : it.second[0])
        {
            cout << nums << ",";
        }
        cout << "} - {";
        for (auto nums : it.second[1])
        {
            cout << nums << ",";
        }
        cout << "}" << endl;
    }
    cout << "final states: " << endl;
    for (auto it : mydfa.final_states)
    {
        cout << "{";
        for (auto nums : it)
        {
            cout << nums << ",";
        }
        cout << "}, ";
    }
    cout << endl;
}

string in_to_post(string seq)
{
    string post = "";
    int n = seq.size();
    stack<char> st;
    for (int i = 0; i < n; i++)
    {
        if (seq[i] == '(')
        {
            st.push('(');
        }
        else if (seq[i] == ')')
        {
            while (st.top() != '(')
            {
                post += st.top();
                st.pop();
            }
            st.pop();
        }
        else
        {
            st.push(seq[i]);
        }
    }
    return post;
}

string dotAdder(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (i != s.size() - 1 and s[i] == ')' and s[i + 1] == '(')
        {
            s.insert(i + 1, ".");
            i++;

            int left = i - 1, right = i + 1,
                countleft = 0, countright = 0;

            while (left >= 0)
            {
                if (s[left] == ')')
                    countleft++;
                else if (s[left] == '(')
                    countleft--;

                if (countleft == 0)
                {
                    s.insert(left, "(");
                    break;
                }
                left--;
            }

            right += 1;

            while (right < s.size())
            {
                if (s[right] == '(')
                    countright++;
                else if (s[right] == ')')
                    countright--;

                if (countright == 0)
                {
                    s.insert(right, ")");
                    break;
                }
                right++;
            }
        }
    }

    return s;
}

vector<dfa> regex_to_dfa(vector<string> regex)
{
    // vector<nfa_wo_E> NFA;
    vector<dfa> DFA;
    for (auto r : regex)
    {
        string wdot = dotAdder(r);
        string seq = in_to_post(wdot);
        nfa n = build_nfa(seq);
        nfa_wo_E nfawo = nfaE2nfa(n);
        DFA.push_back(nfa_to_dfa(nfawo));
    }
    return DFA;
}

int main()
{
    ifstream myfile("input.txt");
    string filestr;
    string input_string;
    vector<string> regex_array;

    if (myfile.is_open())
    {
        getline(myfile, filestr);
        input_string = filestr;
        while (myfile)
        {
            getline(myfile, filestr);
            regex_array.push_back(filestr);
        }
    }
    myfile.close();
    regex_array.pop_back();
    vector<dfa> regex_dfas = regex_to_dfa(regex_array);
    // print_dfa(regex_dfas[1]);
    // cout << regex_dfas[1].start_state[0];

    vector<pair<string, int>> ans;

    int start = 0;
    for (int i = start; i < input_string.size();)
    {
        int maxLen = -1e7;
        int maxIndex = 0;
        string t = "";
        for (int k = 0; k < regex_dfas.size(); k++)
        {
            vector<int> curr = regex_dfas[k].start_state;
            for (int j = i; j < input_string.size(); j++)
            {

                curr = regex_dfas[k].dfa[curr][input_string[j] - 'a'];
                for (auto f : regex_dfas[k].final_states)
                {
                    if (curr == f)
                    {
                        int len = j - i + 1;
                        if (len > maxLen)
                        {
                            maxIndex = k + 1;
                            maxLen = len;
                            // cout << "Rule" << k + 1 << " Max:  " << maxLen << endl;

                            // cout << "Token:" << t << endl;
                        }
                    }
                }
            }
        }
        // cout<<"hi"<<endl;

        if (maxLen == -1e7)
        {
            string c = "";
            c += input_string[i];
            ans.push_back(make_pair(c, 0));
            i++;
        }
        else
        {
            t = input_string.substr(i, maxLen);
            ans.push_back({t, maxIndex});
            i += maxLen;
            // cout << "i" << i;
        }
    }

    for (auto p : ans)
    {
        cout << "< " << p.first << "," << p.second << " >" << endl;
    }
}
