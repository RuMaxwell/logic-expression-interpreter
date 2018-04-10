/*
 * Project: Logic expression interpreter
 * 
 * expression.h -- The interpreter of lisp-like s-expressions of logic expression
 * Compile s-expressions to executive functions of LogicItpt in logic_core.h
 * (xor (or (and A B) (and C D) E) F) => itp.Xor(itp.Or(itp.And(A, B), itp.And(C, D), E), F)
 */

#pragma once

#include "rumdefs.h"
#include <vector>
#include <string>

_RUM_BEGIN

std::vector<std::string> keywords { "t", "f", "not", "and", "or", "rep", "xor", "xorn", "andn", "orn" };

int calculate(const char expr[])
{
    for (int i = 0; expr[i] != '\0'; i++)
    {
    }
	return 0;
}

_RUM_END
