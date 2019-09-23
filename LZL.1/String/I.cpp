#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
#include<cstring>
using namespace std;

unordered_map<string,string>mp;
string str;

int main()
{
    while(getline(cin,str))
    {
        int a=str.find('[');
        int b=str.find(']');
        if(a==string::npos || b==string::npos)
        {
            cout << "what?\n";
            continue;
        }
        mp[str.substr(a+1,b-a-1)]=str.substr(b+1,str.size()-b-1);
        mp[str.]
    }

}