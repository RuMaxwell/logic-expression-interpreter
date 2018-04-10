/*
 * Project: Logic expression interpreter
 * 
 * logic_core.h -- The core algorithm and data structure of logic calculation
 * The class uses a memory (array-like) of stacks to calculate logic expression
 */

#pragma once
#ifndef LOGIC_CORE_H
#define LOGIC_CORE_H

#include "rumdefs.h"
#include <iostream>

_RUM_BEGIN

#define MAX_MEMORY 0xfffffu

enum starter
{
    FS, TR, NT, AD, OR, REP
};

typedef unsigned int addr_t;

class LogicItpt
{
public:

//#define DEBUG_TRUE
#ifdef DEBUG_TRUE
public:
#else
protected:
#endif
    int max_memory;     // The size of the memory
    addr_t* memory;     // Memory that stores stack pointers

    int ptr;
    void incptr() { ptr++; if (ptr >= max_memory) { std::cerr << "memory overflow" << std::endl; }}

    bool started_up;
    bool prefunc_loaded;

public:
    LogicItpt(int memory_size = MAX_MEMORY) : max_memory(memory_size)
    {
        // new will initialize every memory unit to 0 (false)
        memory = new addr_t[max_memory];
        ptr = 0;
        started_up = false;
        prefunc_loaded = false;
    }

    ~LogicItpt()
    {
        delete[] memory;
    }

    // Initialize special pointers
    void startup()
    {
        /* 0x00000 */ memory[ptr] = FS;  incptr(); // false
        /* 0x00001 */ memory[ptr] = TR;  incptr(); // true
        /* 0x00002 */ memory[ptr] = NT;  incptr(); // not A
        /* 0x00003 */ memory[ptr] = AD;  incptr(); // and A B
        /* 0x00004 */ memory[ptr] = OR;  incptr(); // or A B
        /* 0x00005 */ memory[ptr] = REP; incptr(); // repeat (rep) A
        // Program entrance: 0x00006
        started_up = true;
    }

    // Load predefined functions, including xor, xor not (xorn), and not (andn), or not (orn), and A B C, or A B C
    // Predefined functions use 0x0000f and 0x00011 to store the first two parameters, and 0x00028 to store the third
    void loadprefunc()
    {
        if (!started_up)
        {
            startup();
        }
        addr_t A = FS;
        addr_t B = FS;
        addr_t C = FS;
        // (xor A B) = (or (and (not A) B) (and A (not B)))
        /* 0x00006 */ memory[ptr] = OR;               incptr();
        /* 0x00007 */ memory[ptr] = ptr + 2;          incptr(); // => 0x00009 (and (not A) B)
        /* 0x00008 */ memory[ptr] = ptr + 0x12 - 0x8; incptr(); // => 0x00012 (and A (not B))
        /* 0x00009 */ memory[ptr] = AD;               incptr();
        /* 0x0000a */ memory[ptr] = ptr + 2;          incptr(); // => 0x0000c (not A)
        /* 0x0000b */ memory[ptr] = ptr + 0x10 - 0xb; incptr(); // => 0x00010 (rep B)
        /* 0x0000c */ memory[ptr] = NT;               incptr();
        /* 0x0000d */ memory[ptr] = ptr + 1;          incptr(); // => 0x0000e (rep A)
        /* 0x0000e */ memory[ptr] = REP;              incptr();
        /* 0x0000f */ memory[ptr] = A;                incptr();
        /* 0x00010 */ memory[ptr] = REP;              incptr();
        /* 0x00011 */ memory[ptr] = B;                incptr();
        /* 0x00012 */ memory[ptr] = AD;               incptr();
        /* 0x00013 */ memory[ptr] = 0xe;              incptr(); // => 0x0000e (rep A)
        /* 0x00014 */ memory[ptr] = ptr + 1;          incptr(); // => 0x00015 (not B)
        /* 0x00015 */ memory[ptr] = NT;               incptr();
		/* 0x00016 */ memory[ptr] = 0x10;             incptr(); // => 0x00010 (rep B)
        // (xorn A B) = (not (xor A B))
        /* 0x00017 */ memory[ptr] = NT;               incptr();
        /* 0x00018 */ memory[ptr] = 0x6;              incptr(); // => 0x00006 (xor A B)
        // (andn A B) = (not (and A B))
        /* 0x00019 */ memory[ptr] = NT;               incptr();
        /* 0x0001a */ memory[ptr] = AD;               incptr();
        /* 0x0001b */ memory[ptr] = 0xe;              incptr(); // => 0x0000e (rep A)
        /* 0x0001c */ memory[ptr] = 0x10;             incptr(); // => 0x00010 (rep B)
        // (orn A B) = (not (or A B))
        /* 0x0001d */ memory[ptr] = NT;               incptr();
        /* 0x0001e */ memory[ptr] = OR;               incptr();
        /* 0x0001f */ memory[ptr] = 0xe;              incptr(); // => 0x0000e (rep A)
        /* 0x00020 */ memory[ptr] = 0x10;             incptr(); // => 0x00010 (rep B)
        // (and A B C) = (and A (and B C))
        /* 0x00021 */ memory[ptr] = AD;               incptr();
        /* 0x00022 */ memory[ptr] = 0xe;              incptr(); // => 0x00010 (rep A)
        /* 0x00023 */ memory[ptr] = ptr + 1;          incptr(); // => 0x00024 (and B C)
        /* 0x00024 */ memory[ptr] = AD;               incptr();
        /* 0x00025 */ memory[ptr] = 0x10;             incptr(); // => 0x00010 (rep B)
        /* 0x00026 */ memory[ptr] = ptr + 1;          incptr(); // => 0x00027 (rep C)
        /* 0x00027 */ memory[ptr] = REP;              incptr();
        /* 0x00028 */ memory[ptr] = C;                incptr();
        // (or A B C) = (or A (or B C))
        /* 0x00029 */ memory[ptr] = OR;               incptr();
        /* 0x0002a */ memory[ptr] = 0xe;              incptr(); // => 0x00010 (rep A)
        /* 0x0002b */ memory[ptr] = ptr + 1;          incptr(); // => 0x00024 (or B C)
        /* 0x0002c */ memory[ptr] = OR;               incptr();
        /* 0x0002d */ memory[ptr] = 0x10;             incptr(); // => 0x00010 (rep B)
        /* 0x0002e */ memory[ptr] = 0x27;             incptr(); // => 0x00027 (rep C)

        prefunc_loaded = true;
    }

    void restart()
    {
        clear();
        startup();
    }

    int Not(int signal)
    {
        return signal ? 0 : 1;
    }

    int And(int in1, int in2)
    {
        return in1 && in2 ? 1 : 0;
    }

    int Or(int in1, int in2)
    {
        return in1 || in2 ? 1 : 0;
    }

    int Rep(int signal)
    {
        return signal;
    }

    // Reads memory for a unit and processes
    int readproc(int& i)
    {
        if (!started_up)
        {
            startup();
        }

        if (i >= max_memory) { throw ""; }
        else
        {
            if (memory[i] == FS)
            {
                return FS;
            }
            else if (memory[i] == TR)
            {
                return TR;
            }
            else if (memory[i] == NT)
            {
                return Not(readproc(++i));
            }
            else if (memory[i] == AD)
            {
                int i_2 = i + 2;
                int res1 = readproc(++i);
                i = i_2;
                int res2 = readproc(i);
                return And(res1, res2);
            }
            else if (memory[i] == OR)
            {
                int i_2 = i + 2;
                int res1 = readproc(++i);
                i = i_2;
                int res2 = readproc(i);
                return Or(res1, res2);
            }
            else if (memory[i] == REP)
            {
                return Rep(readproc(++i));
            }
            else
            {
                i = memory[i];
                return readproc(i);
            }
        }
    }

    int run(int a, int b, int c, int begin)
    {
        memory[0xf] = a;
        memory[0x11] = b;
        memory[0x28] = c;
        int i = begin;
        return readproc(i);
    }

    int Xor(int in1, int in2)
	{
		return run(in1, in2, FS, 6);
	}

    int XorNot(int in1, int in2)
    {
        return run(in1, in2, FS, 0x17);
    }

    int AndNot(int in1, int in2)
    {
        return run(in1, in2, FS, 0x19);
    }

    int OrNot(int in1, int in2)
    {
        return run(in1, in2, FS, 0x1d);
    }

    int And(int in1, int in2, int in3)
    {
        return run(in1, in2, in3, 0x21);
    }

    int Or(int in1, int in2, int in3)
    {
        return run(in1, in2, in3, 0x29);
    }

    // Write data to a memory unit
    // Data can only be writen to address >= 6 and <= ptr
    // Return 1 if ptr++, 0 if ptr remains, -1 if error
    int memory_write(addr_t data, addr_t address = 0)
    {
        if (address == 0)
        {
            memory[ptr] = data; incptr();
            return 1;
        }
        else if (address < 6)
        {
            std::cerr << "memory access error: write to readonly space" << std::endl;
            return -1;
        }
        else
        {
            if (address > ptr)
            {
                std::cerr << "memory access error: pointer to space that not been used" << std::endl;
                return -1;
            }
            memory[address] = data;
            if (address == ptr)
            {
                incptr();
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    addr_t memory_read(addr_t address)
    {
        if (address >= max_memory)
        {
            std::cerr << "memory access error: pointer to space that not been used" << std::endl;
            return FS;
        }
        return memory[address];
    }

    addr_t current_ptr()
    {
        return ptr;
    }

    void memory_erase()
    {
        if (ptr <= 6)
        {
            std::cerr << "memory access error: cannot erase 0x05" << std::endl;
            return;
        }
        memory[ptr] = 0; ptr--;
    }

protected:
    void clear()
    {
        for (int i = 0; i < max_memory; i++)
        {
            memory[i] = 0;
        }
        started_up = false;
        prefunc_loaded = false;
    }
};

_RUM_END

#endif // ! LOGIC_CORE_H
