#include <iostream>
using namespace std;
int main(){
    int t;
    int ans = 0;
    cin >> t;
    while(t--){
        int n,k;
        cin >> n >> k;
        printf("Case %d: ",++ans);
        if(n&1==1 && k==1)
            cout << "first\n";
        else if(k>=n)
            cout << "first\n";
        else
            cout << "second\n";
    }
    return 0;
}