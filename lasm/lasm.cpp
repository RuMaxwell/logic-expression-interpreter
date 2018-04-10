#include "lasm.h"

using namespace lasm;

int main(int argc, char* argv[])
{
    initialize();
    if (argc > 1)
    {
        auto lines = readasm(argv[1]);
        assemble(lines);
    }
    else
    {
#define DEBUG
#ifdef DEBUG
        auto lines = readasm("C:\\Users\\Surface\\cpp\\logic expression interpreter\\lasm\\sample.lasm");
        assemble(lines);
#else
        std::cerr << "Fatal error: no input file" << endl;
#endif
    }

    return 0;
}