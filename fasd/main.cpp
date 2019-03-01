#include <iostream>

using namespace std;

int main()
{
    int c ;
    cout << &c << endl;
    int a[3];

    for(int i = 0; i < 3; i++){
        cout << &a[i] << ' ';
    }
    cout << '\n';
    char b[3];
    for(int i = 0; i < 3; i++){
        cout << &b[i] << ' ';
    }
    cout << '\n';
    int d;
    cout << &d << endl;
    return 0;
}
// cách nhau 4 bytes
