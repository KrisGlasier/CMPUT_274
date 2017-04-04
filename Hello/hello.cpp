/* Introducing C++ in terminal */

#include <iostream>

using namespace std;

int main(){ // Still required in all c++ files.
    int a = 4, b = 5;
    cout << "Hello world!!!" << endl << (a+b) << " Hey" << endl;

    cout << "Enter two numbers, please: ";
    int x, y;
    cin >> x >> y;
    cout << "You inputted " << x << " and " << y << endl;
    cout << x << " * " << y << " = " << x*y << endl;

    int q = 0x7fff, r = 0x7fff;

    cout << q << " * " << r << " = " << q*r << endl;
    cout << (0x7ffful) * (0x7ffful) << endl;

    unsigned long long la = 0xfffffffful;
    cout << la << "^2 = " << endl << la * la << endl;
    cout << 0xFFFFFFFFFFFFFFFFull << endl << -1ull << endl;
    return 0;
}
