#include<iostream>
#include<vector>
using namespace std;
class ac
{
    public:
        static vector<int>vec;
        int tt(){cout << vec.size() << endl;}
};
vector<int> dsad(10);
vector<int> ac::vec=dsad;
int main()
{
    ac tt;
    tt.tt();
    return 0;
}