#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<bits/stdc++.h>
using namespace std;
string in_to_post(string seq)
{   
    string post="";
    int n=seq.size();
    stack<char> st;
    for(int i=0;i<n;i++)
    {
        if(seq[i]=='(')
        {
            st.push('(');
        }
        else if(seq[i]==')')
        {
            while(st.top()!='(')
            {
                post+=st.top();
                st.pop();
            }
            st.pop();
        }
        else {           
            st.push(seq[i]);
        }
    }
    return post;
}

int main()
{
    string seq;
    cin>>seq;
    string post=in_to_post(seq);
    cout<<post;
    return 0;
}