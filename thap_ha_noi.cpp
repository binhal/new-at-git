#include<iostream>
using namespace std;

void tower(int n,int nguon, int tg, int dich)
{


    if(n == 1) {cout << "move disc from " << nguon << "to " << dich << endl;
    return ;}
    tower(n - 1, nguon, dich, tg);
    tower(1, nguon , tg, dich);
    tower(n - 1, tg,nguon , dich);

}

int main()
{
    int n ;
    cin >> n;
    int a = 1;
    int b = 2;
    int c = 3;
    tower(n,a,b,c);
    return 0;
}
