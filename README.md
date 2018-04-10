# logic-expression-interpreter
A stack-oriented machine for logical calculation, an interpreter of lisp-like expression for this machine, an infix-to-prefix expression translator, and a based-on assembly tool.



### Document List

| filename                | explanation                                                  |
| ----------------------- | ------------------------------------------------------------ |
| rumdefs.h               | Namespace definition                                         |
| logic_core.h            | Core part of the project, a stack-oriented machine simulator |
| expression.h            | An interpreter using lisp-like expression to express logic computations |
| itos.h                  | A translator for infix-expression to prefix-expression (also, s-expression) |
| logicexpr.h             | Make expression.h support infix-expression                   |
| lasm/lasm.py            | A Python instance of lasm (Logic Assembly) compiler          |
| lasm/lasm.h && lasm.cpp | A C++ instance of lasm compiler                              |

