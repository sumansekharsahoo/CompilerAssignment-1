#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<bits/stdc++.h>
using namespace std;

int preceed(char c) 
{
    if(c=='|')
    return 1;

    if(c=='.')
    return 2;

    if((c=='+')|| (c=='?'))
    return 3;

    if(c=='*')
    return 4;  

    return 0;
}
 
string in_to_post(string seq)
{   
    string post="";
    cin>>seq;
    int n=seq.size();
    stack<char> st;

    for(int i=0;i<n;i++)
    {
        if((seq[i]=='a') || (seq[i]=='b'))
        {
            post+=seq[i];
            //cout<<seq[i];
        }

        else if(seq[i]=='(')
        {
            //cout<<seq[i];
            st.push('(');
        }
        
        
        else if(seq[i]==')')
        {
            while(st.top()!='(')
            {
                post+=st.top();
                //cout<<st.top();
                st.pop();
            }

            st.pop();
        }

        else {
            while (!st.empty() && preceed(seq[i]) <= preceed(st.top()) ) 
            {
                post += st.top();
                st.pop();
            }
            st.push(seq[i]);
        }



    }

    //not required for well paranthesised expressions
    while(!st.empty())
    {
        post+=st.top();
        st.pop();
    }

    return post;

}

int main()
{
    string seq;
    cin>>seq;
    post=in_to_post(seq);
    cout<<post;
    return 0;
}