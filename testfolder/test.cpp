#include <iostream>
#include <bits/stdc++.h>
#include "NFA_withE_functions.cpp"
using namespace std;

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
    print_nfa(n);

    return 0;
}