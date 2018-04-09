#include "logicexpr.h"
#include <iostream>

using namespace std;
using namespace rum;

int main()
{
    LogicItpt itp;

    itp.startup();
    itp.loadprefunc();

#ifdef DEBUG_TRUE
	auto show_mem = [&itp]()->void
    {
        for (int i = 0; i < 25; i++)
        {
            cout << hex << "0x" << i << "\t" << itp.memory[i] << "\t\t";
            if ((i + 1) % 5 == 0)
            {
                cout << endl;
            }
        }
        cout << endl;
    };
#endif // DEBUG_TRUE

    cout << "Xor 00-11 > ";
    cout << itp.Xor(FS, FS);
#ifdef DEBUG_TRUE
    show_mem();
#endif // DEBUG_TRUE
    cout << itp.Xor(FS, TR);
#ifdef DEBUG_TRUE
    show_mem();
#endif // DEBUG_TRUE
    cout << itp.Xor(TR, FS);
#ifdef DEBUG_TRUE
    show_mem();
#endif // DEBUG_TRUE
    cout << itp.Xor(TR, TR) << endl;
#ifdef DEBUG_TRUE
    show_mem();
#endif // DEBUG_TRUE
    cout << "XorNot 00 > " << itp.XorNot(FS, FS) << endl;
    cout << "And3 111-000 > ";
    cout << itp.And(TR, TR, TR) << itp.And(TR, TR, FS) << itp.And(TR, FS, TR) << itp.And(FS, TR, TR)
         << itp.And(TR, FS, FS) << itp.And(FS, TR, FS) << itp.And(FS, FS, TR) << itp.And(FS, FS, FS) << endl;

    return 0;
}