#include<iostream>
#include<algorithm>
#include<map>
#include<cstring>
#include<unordered_map>
using namespace std;
map<int,int>mp;
unordered_map<int,int>tmp;
int q;
int m;
int book[100000+10];
int flag[100000+10];
int ans;
int main()
{
    scanf("%d",&q);
    while(q--)
    {
        tmp.clear();
        mp.clear();
        memset(book,0,sizeof(book));
        memset(flag,0,sizeof(flag));
        unordered_map<int,int>cep;
        scanf("%d",&m);
        for(int i=0;i<m;i++)
        {
            scanf("%d",&book[i]);
            mp[book[i]]++;
            tmp[book[i]]++;
        }
        for(auto x=mp.rbegin();x!=mp.rend();x++)
        {

            if(x->first==1)
            continue;
            if(x->second>=2)
            {
                mp[x->first-1]+=(x->second/2);
                x->second%=2;
            }
        }
        if(mp[1]>=2)
        printf("Case %d: YES\n",++ans);
        else{
            printf("Case %d: NO\n",++ans);
            continue;
        }
        int tt=2; //因为存在1的话直接比标记一个就好
        if(!tmp[1])
        {
            cep[2]=2;
            while(1)
            {
                if(tmp.find(tt)!=tmp.end())
                {
                    if(cep[tt]-tmp[tt]<=0)
                    {
                        break;
                    }else{
                        cep[tt]-=tmp[tt];
                        cep[tt+1]=cep[tt]*2; //所需的下一个
                        cep[tt]=tmp[tt];//这种数字需要的个数
                        tt++;
                    }
                }else{
                    cep[tt+1]=cep[tt]*2;
                    cep[tt]=0;
                    tt++;
                }
            }
        }   
        else 
        cep[1]=1;
        int number=mp.size();
        for(int i=0;i<m;i++)
        {
            if(!number)
            break;
            else if(cep[book[i]]){
                flag[i]=1;
                cep[book[i]]--;
                if(!cep[book[i]])
                number--;
            }
        }
        for(int i=0;i<m;i++){
            printf("%d",flag[i]);
        }
        putchar('\n');
    }
    return 0;
}