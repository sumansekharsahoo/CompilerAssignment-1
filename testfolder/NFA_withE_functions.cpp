#include<iostream>
#include<bits/stdc++.h>
using namespace std;

typedef struct NFA_t
{
    set<int> start;
    set<int> fin;
    map<int,map<char,vector<int>>> mp;
} nfa;

nfa character(char c)
{
    nfa n;
    n.start.insert(1);
    n.fin.insert(2);

    char alt;
    if(c=='a')
    alt='b';
    else
    alt='a';

    map<char,vector<int>> temp;
    temp[c]={2};
    temp[alt]={0};
    temp['E']={1};
    n.mp[1]=temp;
    temp.clear();
    temp[c]={0};
    temp[alt]={0};
    temp['E']={2};
    n.mp[2]=temp;

    return n;
}

nfa offset(nfa n,int offset)
{
    set<int> new_start;
    for(auto it:n.start)
    {
        new_start.insert(it+offset);
    }

    set<int> new_final;
    for(auto it:n.fin)
    {
        new_final.insert(it+offset);
    }

    map<int,map<char,vector<int>>> new_mp;
    for(auto it:n.mp)
    {
        int new_key = it.first + offset;
        map<char,vector<int>> temp_map;
        for(auto it2:it.second)
        {
            vector<int> temp;
            for(auto it3:it2.second)
            {
                if(it3!=0)
                temp.push_back(it3+offset);
                else temp.push_back(it3);
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

nfa or_union(nfa n1,nfa n2)
{
    int num=n1.mp.size()+n2.mp.size();
    nfa n_one=offset(n1,n2.mp.size());
    num++;
    nfa n;
    n.start.insert(num);

    n.fin = n_one.fin;

    for(auto i:n2.fin)
    {
        n.fin.insert(i);
    }
    n.mp = n_one.mp;
    for(auto it:n2.mp)
    {
        n.mp[it.first] = it.second;
    }
    for(auto i:n_one.start)
    {
        n.mp[num]['E'].push_back(i);
    }
    

    for(auto i:n2.start)
    {
        n.mp[num]['E'].push_back(i);
    }
    
    n.mp[num]['E'].push_back(num);
    n.mp[num]['a']={0};
    n.mp[num]['b']={0};

    return n;
}

nfa closure(nfa n,char c)
{
    if(c=='*')
    {
        for(auto i:n.start)
        {
            n.fin.insert(i);
        }
    }

    for(auto i:n.fin)
    {
        for(auto j:n.start)
        {
            n.mp[i]['E'].push_back(j);
        }
    }
    return n;
}

nfa condition(nfa n)
{
    for(auto i:n.start)
    {
        n.fin.insert(i);
    }
    return n;
}

nfa concatenate(nfa n1,nfa n2)
{
    n1 = offset(n1,n2.mp.size());
    nfa n;
    n.start = n1.start;
    n.fin = n2.fin;
    n.mp = n1.mp;
    for(auto it:n2.mp)
    {
        n.mp[it.first] = it.second;
    }
    for(auto it:n1.fin)
    {
        for(auto it2:n2.start)
        {
            n.mp[it]['E'].push_back(it2);
        }
       
    }
    return n;
}

void print_nfa(nfa n)
{
    cout<<"Start: ";
    for(auto it:n.start)
    {
        cout<<it<<" ";
    }
    cout<<endl;
    cout<<"Final: ";
    for(auto it:n.fin)
    {
        cout<<it<<" ";
    }
    cout<<endl;
    for(auto it:n.mp)
    {
        cout<<it.first<<": ";
        for(auto it2:it.second)
        {
            cout<<it2.first<<"->";
            for(auto it3:it2.second)
            {
                cout<<it3<<" ";
            }
            cout<<endl;
        }
    }
}

int main()
{
    return 0;
}