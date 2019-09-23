#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<unordered_map>
#include<list>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
#include<cmath>
using namespace std;

struct node{
    int x1,x2,y1,y2;
    bool operator==(const node a)const {
        if(x1==a.x1 && x2==a.x2 && y1==a.y1 && y2==a.y2)
        return true;
        return false;  //if don't have this statement there will function returns nothing on test1
    }
};

node book[10];
vector<int>vec;
int flag;
int judge(int tmp){
    if(0<=tmp && tmp<=INT32_MAX)
    vec.emplace_back(tmp);
}
bool judge(int i,int j){
    if((i>=book[1].x1 && i<=book[1].x2 && j>=book[1].y1 && j<=book[1].y2)
    || (i>=book[2].x1 && i<=book[2].x2 && j>=book[2].y1 && j<=book[2].y2))
    return true;
    return false;
}

bool solve(){
    if(abs(book[1].y1-book[2].y2)==1 || abs(book[2].y1-book[1].y2)==1 ||
    abs(book[1].x2-book[2].x1)==1 || abs(book[2].x1-book[1].x2)==1){
        return true;
    }
    return false;
}

int main(){
    for(int i=0;i<3;++i)
    cin >> book[i].x1 >> book[i].y1 >> book[i].x2 >> book[i].y2; 
    if(book[0] == book[1]){
        if(book[1]==book[2]){
            cout << "NO\n";
            return 1;
        }
    }
    for(int i=0;i<3;++i){
        for(int d=-1;d<=1;++d){
            int tx1 = book[i].x1+d;
            int ix2 = book[i].x2+d;
            int iy1 = book[i].y1+d;
            int iy2 = book[i].y2+d;
            judge(tx1);judge(ix2);judge(iy1);judge(iy2);
        }
    }
    sort(vec.begin(),vec.end());
    vec.erase(unique(vec.begin(),vec.end()),vec.end());
    int x = vec.size();
    for(int i=0;i<3;++i){
        int a=0,b=0;
        book[i].x1 = (find(vec.begin(),vec.end(),book[i].x1)) - vec.begin();
        book[i].x2 = (find(vec.begin(),vec.end(),book[i].x2)) - vec.begin();
        book[i].y1 = (find(vec.begin(),vec.end(),book[i].y1)) - vec.begin();
        book[i].y2 = (find(vec.begin(),vec.end(),book[i].y2)) - vec.begin();
    }
    for(int i=book[0].x1;i<=book[0].x2;++i){
        for(int j=book[0].y1;j<=book[0].y2;++j){
            if(!judge(i,j)){
                cout << "YES\n";
                return 0;
            }
        }
    }
    if(solve()) //判断矩形是否重合
    cout << "YES\n";
    else
    cout << "NO\n";
    return 0;
}
//这道题我惊了　评测机的问题　不是我的问题