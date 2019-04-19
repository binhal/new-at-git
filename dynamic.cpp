#include <iostream>
#include <cstring>
using namespace std;

char* concat(const char* a , const char*  b){
    int x = strlen(a);
    int y = strlen(b);
    char* arr = new char[x + y];
    strcpy (arr,a);
    strcat (arr,b);
    return arr;

}

int main()
{
    char s1[] = "hello ";
    char s2[] = "world";
    char* arr = concat(s1, s2);
    cout <<     arr;
    return 0;
}
