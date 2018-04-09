/*
 * Logic assembly
 * 
 * An assembly compiler of RUMAXWELL's Logic Expression Machine
 */

#pragma once

#include "../logic_core.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

using namespace rum;
using namespace std;

LogicItpt itp;

namespace lasm
{
    // Initialize logic interpreter
    void initialize()
    {
        itp.startup();
        itp.loadprefunc();
    }

    // Read the target file
    vector<string> readasm(const char* filename)
    {
        char line[256] = "";
        vector<string> txt;

        ifstream asmfile;
        asmfile.open(filename);

        while (asmfile.getline(line, 256))
        {
            string linestr(line);
            linestr.push_back('\n');
            txt.push_back(linestr);
        }

        asmfile.close();

        return txt;
    }

    map<string, int> globalvars;

    void assemble(const vector<string>& instructions)
    {
        for (auto line : instructions)
        {
            //
        }
    }
}
