#include <iostream>
using namespace std;
void byteorder()
{
    union
    {
        short value;
        char union_bytes[sizeof(short)];
    } test;
    test.value = 0x0102;
    if (test.union_bytes[0] == 1 && test.union_bytes[1] == 2)
        cout << "big endian" << endl;
    else if (test.union_bytes[0] == 2 && test.union_bytes[1] == 1)
        cout << "little endian" << endl;
    else
        cout << "other" << endl;
}
