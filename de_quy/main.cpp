#include <iostream>

using namespace std;

int factorial(int x ){
    cout << "x = "  << x << " at " << &x << endl;
    if (x > 1) return x*factorial(x - 1);
    else return 1;

}

int main()
{
    cout << factorial(5);
    return 0;
}
// kích thước của 1 stack frame cho hàm factorial là n x 2.
