#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;
unordered_map<char,int>mp;
string tmp;
int ans;
int main(){
	cin >> tmp;
	int len = tmp.size();
	bool flag = true;
	for(int i = 0; i < len; i++){
		if(!flag)
			break;
		mp[tmp[i]] = 1;
		for(int j = 0; j < tmp[i]-'a'; j++){
			if(mp['a'+j] == 0){
				flag = false;
				break;
			}
		}
	}
	if(flag)
		cout<<"YES"<<endl;
	else
		cout<<"NO"<<endl;
	return 0;
}
