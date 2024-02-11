#include <bits/stdc++.h>
#include <fstream>
using namespace std;
int main()
{
    ifstream myfile("input.txt");
    string str;
    unordered_set<int> fstates;
    unordered_map<int, vector<vector<int>>> transition;
    int no_of_states;
    if (myfile.is_open())
    {
        getline(myfile, str);
        no_of_states = stoi(str);
        getline(myfile, str);
        for (int i = 0; i < str.size(); i++)
        {
            if (isdigit(str[i]))
            {
                fstates.insert((int)(str[i] - '0'));
            }
        }
        while (myfile)
        {
            getline(myfile, str);
            string st_str;
            int ptr = 0;
            while (isdigit(str[ptr]) && ptr < str.size())
            {
                st_str.push_back(str[ptr]);
                ptr++;
            }
            int state = stoi(st_str);

            if (transition.find(state) == transition.end())
            {
                int ct = 0;
                transition[state].push_back(vector<int>{});
                transition[state].push_back(vector<int>{});
                while (ptr < str.size())
                {
                    if (isdigit(str[ptr]))
                    {
                        string tst;
                        while (isdigit(str[ptr]))
                        {
                            tst.push_back(str[ptr]);
                            ptr++;
                        }
                        transition[state][ct].push_back(stoi(tst));
                    }
                    if (str[ptr] == '}')
                    {
                        ct++;
                    }
                    ptr++;
                }
            }
        }
    }
    myfile.close();
    // set<vector<int>> visited;
    vector<int> cur{0};
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
}