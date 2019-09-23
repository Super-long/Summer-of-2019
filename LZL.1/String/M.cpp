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

char book[200];
char tmp[200];
int Next[200];
int m;

void kmp(){
	Next[0]=0;
	int j=0,i=1;
	while(i<m){
		if(book[j]==book[i]){
			Next[i]=++j;
			i++;
		}
		else if(j==0){
			i++;
		}
		else{
			j=Next[j-1];//从失配位置开始匹配
		}
	}
}


int main()
{
    while(cin>>book)
    {   
        memset(Next,0,sizeof(Next));
        strcpy(tmp,book);
        if(strstr(tmp,"hat")!=NULL)
        {
            m=strlen(book);
            kmp();
/*             for(int i=0;i<m;i++)
            printf("%d ",Next[i]);
            putchar('\n'); */
            if(Next[m-1]==m-3 && m%3==0)
            continue;
            else if(m==3)
            continue;
            else 
            cout << book << endl;
        }else{
            cout << book << endl;
        }
    }
    return 0;
}