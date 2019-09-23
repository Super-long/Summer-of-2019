#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstring>
using namespace std;
int m,n;
int book[40];
map<int,int>mp;
int vis;
vector<int>vec;
vector<string>temp;
int tt=0;
void print(int number)
{
    for(auto x=vec.begin();x!=vec.end();x++) 
    {
        if(x==vec.begin())
        cout << *x;
        else 
        cout << "+" << *x ;
    }
    cout << "+" << tt;
    cout << "=" << number << endl;
}


void dfs(int ans,int number)
{
    for(int i=number;i<n;i++)
    {
        if(mp[ans+book[i]] && ans!=0 && mp[book[i]])
        {
            if(book[i]<*(vec.end()-1))   //防止重复情况
            continue;
            tt=book[i];
            vis++;
            print(ans+book[i]);
            vec.push_back(book[i]);
            mp[book[i]]--;
            dfs(ans+book[i],number+1);
            mp[book[i]]++;
            vec.erase(vec.end()-1,vec.end());
        }else if((ans+book[i])>book[n-1])
        {
            break;
        }
        else
        {
            vec.push_back(book[i]);
            mp[book[i]]--;   //防止自己与自己匹配 比如样例三中3+3=6
            dfs(ans+book[i],number+1);
            mp[book[i]]++;
            vec.erase(vec.end()-1,vec.end());
        }
    }
}
int main()
{
    cin >> m;
    while(m--)
    {
        vis=0;
        cin >> n;
        for(int i=0;i<n;i++)
        {
            cin >> book[i];
            mp[book[i]]++;
        }
        sort(book,book+n,
        [](int a,int b){return a<b;});
        dfs(0,0);
        mp.clear();
        if(!vis)
        cout << "Can't find any equations.\n";
    }
    return 0;
}