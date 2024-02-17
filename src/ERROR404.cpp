#include <bits/stdc++.h>
#include <fstream>
using namespace std;

typedef struct NFA
{
    int start;
    set<int> fin;
    map<pair<int, char>, set<int>> mp;
} nfa_wo_E;

typedef struct NFA_t
{
    int start;
    set<int> fin;
    map<int, map<char, vector<int>>> mp;
} nfa;

typedef struct DFA_t
{
    vector<int> start_state;
    map<vector<int>, vector<vector<int>>> dfa;
    set<vector<int>> final_states;
} dfa_t;

nfa character(char c)
{
    nfa n;
    n.start = 1;
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
    int new_start = n.start + offset;

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
    n.start = num;

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

    n.mp[num]['E'].push_back(n_one.start);
    n.mp[num]['E'].push_back(n2.start);

    n.mp[num]['E'].push_back(num);
    n.mp[num]['a'] = {0};
    n.mp[num]['b'] = {0};

    int final_intro = ++num;
    for (auto i : n.fin)
    {
        n.mp[i]['E'].push_back(final_intro);
    }

    n.fin.clear();
    n.fin.insert(final_intro);
    n.mp[final_intro]['a'].push_back(0);
    n.mp[final_intro]['b'].push_back(0);

    return n;
}

nfa closure(nfa n, char c)
{
    int num = n.mp.size();
    int start_intro = ++num,
        final_intro = ++num;
    if (c == '*')
        n.mp[start_intro]['E'].push_back(final_intro);

    int start_state = n.start;
    n.mp[start_intro]['E'].push_back(start_state);

    for (auto i : n.fin)
    {
        n.mp[i]['E'].push_back(final_intro);
        n.mp[i]['E'].push_back(start_state);
    }

    n.start = start_intro;
    n.mp[start_intro]['a'].push_back(0);
    n.mp[start_intro]['b'].push_back(0);

    n.fin.clear();
    n.fin.insert(final_intro);
    n.mp[final_intro]['a'].push_back(0);
    n.mp[final_intro]['b'].push_back(0);

    return n;
}

nfa condition(nfa n)
{
    int num = n.mp.size();
    int start_intro = ++num,
        final_intro = ++num;

    int start_state = n.start;
    n.mp[start_intro]['E'].push_back(start_state);

    for (auto i : n.fin)
    {
        n.mp[i]['E'].push_back(final_intro);
    }

    n.start = start_intro;
    n.mp[start_intro]['E'].push_back(final_intro);

    n.fin.clear();
    n.fin.insert(final_intro);

    n.mp[start_intro]['a'].push_back(0);
    n.mp[start_intro]['b'].push_back(0);
    n.mp[final_intro]['a'].push_back(0);
    n.mp[final_intro]['b'].push_back(0);
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
        n.mp[it]['E'].push_back(n2.start);
    }

    int num = n.mp.size();
    int start_intro = ++num,
        final_intro = ++num;

    int start_state = n.start;
    n.mp[start_intro]['E'].push_back(start_state);

    for (auto i : n.fin)
    {
        n.mp[i]['E'].push_back(final_intro);
    }

    n.start = start_intro;

    n.fin.clear();
    n.fin.insert(final_intro);

    n.mp[start_intro]['a'].push_back(0);
    n.mp[start_intro]['b'].push_back(0);

    n.mp[final_intro]['a'].push_back(0);
    n.mp[final_intro]['b'].push_back(0);

    return n;
};

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

    int start_s = n.start;
    set<int> fin_s = n.fin;

    nfa_wo_e.start = start_s;

    for (auto i : fin_s)
    {
        set<int> temp_s = E_closure(n, i);
        for (auto j : temp_s)
            nfa_wo_e.fin.insert(j);
    }

    for (auto i : n.mp)
    {
        set<int> set1 = E_closure(n, i.first);
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

dfa_t nfa_to_dfa(nfa_wo_E nfa)
{
    dfa_t result;
    unordered_set<int> fstates;
    unordered_map<int, vector<vector<int>>> transition;
    for (auto it : nfa.fin)
    {
        fstates.insert(it - 1);
    }

    int startstate = nfa.start - 1;

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

vector<dfa_t> regex_to_dfa(vector<string> regex)
{
    vector<dfa_t> DFA;
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
    vector<dfa_t> regex_dfas = regex_to_dfa(regex_array);
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
                        }
                    }
                }
            }
        }

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
        }
    }

    // for (auto p : ans)
    // {
    //     cout << "< " << p.first << "," << p.second << " > " << endl;
    // }
    ofstream outfile;
    outfile.open("output.txt");
    for (auto p : ans)
    {
        outfile << "<" << p.first << "," << p.second << ">";
    }
    outfile.close();
}