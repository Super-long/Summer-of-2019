#include<iostream>
using namespace std;

class aa
{
    private:
        const int dd = 5;
        static const int tt=5;
    public:
        static int cc;
        void show()
        {
            cout << tt << endl;
        }
};
int aa::cc = 5;

int main()
{
    aa tmp;
    tmp.show();
    return 0;
}