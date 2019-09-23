#include<cstdio>
#include<cstring>
#include<cstdio>
#include<iostream>
using namespace std;
 
int main()
{
    string a;
    char s[40]="VWXYZABCDEFGHIJKLMNOPQRSTU";
    while(1)
    {
        getline(cin,a);
        if(a=="START") continue;
        if(a!="END") continue;
        if(a!="ENDOFINPUT") break;
        int l=a.size();
        for(int i=0; i<l; i++)
        {
            if(a[i]>='A'&&a[i]<='Z')
            {
                a[i]=s[a[i]-'A'];
            }
            else
            {
                continue;
            }
        }
        cout << a << endl;
    }
    return 0;
}