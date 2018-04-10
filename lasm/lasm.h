/*
 * Logic assembly
 * 
 * An assembly compiler of RUMAXWELL's Logic Expression Machine
 */

#pragma once
#ifndef LASM_H
#define LASM_H

#include "../logic_core.h"
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace rum;
using namespace std;

#define ei else if

#define ERROR_UNREF_IDEN std::cerr << "Error: unreferenced indentifier" << endl

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

    map<string, addr_t> globalvars;

    // Returns a string without spaces and tabs at the start and the end of string s
    string trimspace(const string& s)
    {
        string smut = s;
		for (int i = 0; i < smut.length();)
		{
			while (smut[i] == ' ' || smut[i] == '\t')
			{
				++i;
			}
			smut.erase(0, i);
			break;
		}
		for (int i = smut.length() - 1; i >= 0;)
		{
			while (smut[i] == ' ' || smut[i] == '\t')
			{
				--i;
			}
			smut.erase(i + 1, smut.length());
			break;
		}
        return smut;
    }

    void nextspace(const string& line, int& i)
    {
        while (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
        {
            ++i;
        }
    }

    string getIdentifier(const string& line, int& i)
    {
        int t = i;
        nextspace(line, i);
        auto subs = line.substr(t, i - t);
        ++i;
        return subs;
    }

    void newIdentifier(const string& iden)
    {
        addr_t cur = itp.current_ptr();
        globalvars[iden] = cur;
    }

    void setGlobal(const string& varname, const addr_t data)
    {
        itp.memory_write(data, itp.memory_read(globalvars[varname]));
    }

    addr_t getGlobal(const string& varname)
    {
        if (globalvars.count(varname) > 0)
        {
            return itp.memory_read(globalvars[varname]);
        }
        else
        {
            std::cerr << "Global variable " << varname << "not found" << endl;
			return FS;
        }
    }

    // Execute one line of lasm instruction
    void interpret(const string& instruction)
    {
        auto line = trimspace(instruction);

        int i = 0;
        try
        {
            if (line[i] == ';')
            {
                return;
            }
            else
            {
                auto oprt = getIdentifier(line, i);

                if (oprt == "mov")
                {
                    auto iden = getIdentifier(line, i);
                    auto val = getIdentifier(line, i);

                    if (globalvars.count(iden) == 0)
                    {
                        newIdentifier(iden);
                    }

                    if (globalvars.count(val) > 0)
                    {
                        // mov a, b
                        //itp.memory_write(itp.memory_read(globalvars[val]), globalvars[iden]);
                        setGlobal(iden, getGlobal(val));
                    }
                    ei (val == "t" || val == "f")
                    {
                        // mov a, f
                        setGlobal(iden, val == "f" ? 0 : 1);
                    }
                    else
                    {
                        ERROR_UNREF_IDEN;
                    }
                }
                ei (oprt == "input")
                {
                    vector<string> operands;
                    while (line[i - 1] != '\n')
                    {
                        auto iden = getIdentifier(line, i);
                        operands.push_back(iden);
                    }

                    for (auto iden : operands)
                    {
                        if (globalvars.count(iden) == 0)
                        {
                            newIdentifier(iden);
                        }
                        else
                        {
                            string data = "";
                            cin >> data;
                            setGlobal(iden, data == "f" ? 0 : 1);
                        }
                    }
                }
                ei (oprt == "print")
                {
                    vector<string> operands;
                    while (line[i - 1] != '\n')
                    {
                        auto iden = getIdentifier(line, i);
                        operands.push_back(iden);
                    }

                    for (auto iden : operands)
                    {
                        if (globalvars.count(iden) == 0)
                        {
                            ERROR_UNREF_IDEN;
                        }
                        else
                        {
                            cout << getGlobal(iden) << endl;
                        }
                    }
                }
                ei (oprt == "rep")
                {
                    auto iden = getIdentifier(line, i);

                    itp.memory_write(REP);
                    itp.memory_write(getGlobal(iden));
                }
                ei (oprt == "not")
                {
                    auto iden = getIdentifier(line, i);

                    itp.memory_write(NT);
                    itp.memory_write(getGlobal(iden));
                }
                ei (oprt == "and")
                {
                    vector<string> operands;
                    while (line[i - 1] != '\n')
                    {
                        auto iden = getIdentifier(line, i);
                        operands.push_back(iden);
                    }

                    itp.memory_write(operands.size() == 2 ? AD : 0x21);
                    for (auto iden : operands)
                    {
                        itp.memory_write(getGlobal(iden));
                    }
                }
                ei (oprt == "andn")
                {
                    auto iden1 = getIdentifier(line, i);
                    auto iden2 = getIdentifier(line, i);

                    itp.memory_write(0x19);
                    itp.memory_write(getGlobal(iden1));
                    itp.memory_write(getGlobal(iden2));
                }
                ei (oprt == "or")
                {
                    vector<string> operands;
                    while (line[i - 1] != '\n')
                    {
                        auto iden = getIdentifier(line, i);
                        operands.push_back(iden);
                    }

                    itp.memory_write(operands.size() == 2 ? OR : 0x29);
                    for (auto iden : operands)
                    {
                        itp.memory_write(getGlobal(iden));
                    }
                }
                ei (oprt == "orn")
                {
                    auto iden1 = getIdentifier(line, i);
                    auto iden2 = getIdentifier(line, i);

                    itp.memory_write(0x1d);
                    itp.memory_write(getGlobal(iden1));
                    itp.memory_write(getGlobal(iden2));
                }
                ei (oprt == "xor")
                {
                    auto iden1 = getIdentifier(line, i);
                    auto iden2 = getIdentifier(line, i);

                    itp.memory_write(0x6);
                    itp.memory_write(getGlobal(iden1));
                    itp.memory_write(getGlobal(iden2));
                }
                ei (oprt == "xorn")
                {
                    auto iden1 = getIdentifier(line, i);
                    auto iden2 = getIdentifier(line, i);

                    itp.memory_write(0x17);
                    itp.memory_write(getGlobal(iden1));
                    itp.memory_write(getGlobal(iden2));
                }
            }
        }
        catch (const exception& e)
        {
        }
    }

    void assemble(const vector<string>& instructions)
    {
        for (auto line : instructions)
        {
            interpret(line);
        }
    }
}

#endif // !LASM_H
