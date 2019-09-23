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
    int index;
    int w;
    public:
        bool operator<(node& tmp){
            return tmp.w<w;
        }
};

int m,n;
node book[1010];
int main(){
    cin >> m;
    for(int i=0;i<m;++i){
        cin >> book[i].w;
        book[i].index = i+1;
    }
    sort(book,book+m);
    long long ans = 0;
    for(int i=0;i<m;++i){
        ans +=(i*book[i].w+1);
    }
    cout << ans << endl;
    for(int i=0;i<m;++i){
        if(!i) cout << book[i].index;
        else cout << " " << book[i].index;
    }
    putchar('\n');
    return 0;
}