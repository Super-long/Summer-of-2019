//条件不要写死　放一个适配的范围　比如说用>=　代替＝＝

#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
using namespace std;

int m,n;
int tmp;
int tmpa,tmpb;
string temp;
int ans;
vector<pair<int,int>> vec;
int prev_tt,next_tt;
int main()
{
    cin >> m >> n;
    for(int i=0;i<m-n+1;i++)
    {
        cin >> temp;
        if(i==0)
        {
            ans=0;
            for(int j=0;j<n-1;j++)
            {
                if(next_tt>=25){prev_tt+=1;next_tt=0;}
                if(j==0)
                {
                    vec.push_back(make_pair(prev_tt,next_tt));
                    cout << char('A'+prev_tt) << char(('a'+next_tt++));
                    continue;
                }
                vec.push_back(make_pair(prev_tt,next_tt));
                cout << " " <<char('A'+prev_tt) << char(('a'+next_tt++));
            }
            if(temp[0]=='Y')
            {
                vec.push_back(make_pair(prev_tt,next_tt));
                cout << " " <<char('A'+prev_tt) << char(('a'+next_tt++));
            }
            else 
            {
                vec.push_back(make_pair(vec[0].first,vec[0].second));
                cout << " " <<char('A'+vec[0].first) << char(('a'+vec[0].second));
            }
            ans=n;
        }else
        {
            ans++;
            if(next_tt>=25){prev_tt+=1;next_tt=0;}
            if(temp[0]=='Y')
            {
                vec.push_back(make_pair(prev_tt,next_tt));
                cout << " " <<char('A'+prev_tt) << char(('a'+next_tt++));
            }
            else 
            {
                vec.push_back(make_pair(vec[ans-n].first,vec[ans-n].second));
                cout << " " <<char('A'+vec[ans-n].first) << char(('a'+vec[ans-n].second));
            }
        }
    }
    putchar('\n');
    return 0;
}