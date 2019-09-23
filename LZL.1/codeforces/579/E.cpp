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
using namespace std;

set<int > se;
int main()
{
    int n;
    cin >> n;
    int a[n];
    for(int i = 0;i<n;i++){
        cin >> a[i];
    }
    sort(a,a+n);

    for(int i = 0;i<n;i++){
        if(a[i] - 1 == 0 && se.count(a[i]) == 0) 
        se.insert(a[i]);
        else if(a[i] - 1 == 0 && se.count(a[i])!=0)
        se.insert(a[i]+1);
        else{
            if(se.count(a[i]-1) == 0) se.insert(a[i]-1);
            else if(se.count(a[i]) == 0) se.insert(a[i]);
            else se.insert(a[i]+1);
        }
    }
    cout << se.size() << endl;
    return 0;
}