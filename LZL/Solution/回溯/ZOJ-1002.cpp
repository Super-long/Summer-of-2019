//学到的东西 可以把一个二维数组抽象成线性 用整除和取余获取坐标
#include<iostream>
#include<algorithm>
#include<string>
using namespace std;
int bestn;
char map[10][10];
int n;

int canput(int row,int col)
{
  int i;
  for(i=row-1;i>=0;i--)
  {
     if(map[i][col]=='X')
     {
         break; 
     } 
     if(map[i][col]=='K')
     {
         return 0; 
     }
  } 
  for(i=col-1;i>=0;i--)
  {
     if(map[row][i]=='X')
     {
        break; 
     }
     if(map[row][i]=='K')
     {
        return 0; 
     }
  }
  return 1;
}

int judge(int k,int current)
{
    if(k==n*n)
    {
        bestn=max(bestn,current);
        return 0;
    }else
    {
        int x=k/n;
        int y=k%n;
        if(canput(x,y) && map[x][y]=='.')
        {
            map[x][y]='K';
            judge(k+1,current+1);
            map[x][y]='.';
        }
        judge(k+1,current);
    }
}

void init()
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        map[i][j]='.';
    }
}
int main()
{
    while(cin >> n)
    {
        bestn=0;
        if(!n)break;
        init();
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                cin >> map[i][j];
            }
        }
        judge(0,0);
        cout << bestn << endl;
    }
}   