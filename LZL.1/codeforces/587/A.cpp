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

int m,n;
char book[200000+10];
int tmpa,tmpb;
vector<char>vec;
int judge(char a,char b){
    vec.push_back(a);
    vec.push_back(b);
}
int main(){
    cin >> m;
    for(int i=1;i<=m;i+=2){
        cin >> book[i];
        cin >> book[i+1];
        if(book[i] != book[i+1]){
            judge(book[i],book[i+1]);
        }else{
            judge('a','b');
            ++tmpa;
        }
    }
    cout << tmpa << endl;
    for(auto x : vec){
        cout << x;
    }
    putchar('\n');
    return 0;
}