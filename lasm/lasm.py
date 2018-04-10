def readfile(filename):
    with open(filename) as f:
        lines = f.readlines()
    return lines


class interpreter:
    def __init__(self):
        self.globalvars = { 'file': '<main>' }
        self.registers = { 'a': 0, 'b': 0, 'c': 0 }
        self.history = []

    def interpret(self, line: str):
        """Interpret one line of instruction"""
        def next_space(i):
            while line[i] not in (' ', '\n'):
                i += 1
            return i

        line = line.strip()
        line += '\n'
        i = 0
        try:
            if line[i] == ';':
                # Comment
                return
            else:
                # Operation
                t = next_space(i)
                opr = line[i: t]
                i = t + 1
                if opr == 'mov':
                    # Bind data to an identifier or move data to register
                    t = next_space(i)
                    iden = line[i: t]
                    i = t + 1
                    t = next_space(i)
                    val = line[i: t]
                    i = t + 1
                    if iden in self.registers:
                        if val in ['f', 't']:
                            self.registers[iden] = 0 if val == 'f' else 1
                        elif val in self.registers:
                            self.registers[iden] = self.registers[val]
                        elif val in self.globalvars:
                            self.registers[iden] = self.globalvars[val]
                        else:
                            print('Error: invalid value binding')
                            return
                    else:
                        if val in ['f', 't']:
                            self.globalvars[iden] = val
                        elif val in self.registers:
                            self.globalvars[iden] = self.registers[val]
                        elif val in self.globalvars:
                            self.globalvars[iden] = self.globalvars[val]
                        elif val == '?':
                            self.globalvars[iden] = -1
                        else:
                            print('Error: invalid value binding')
                            return
                elif opr == 'input':
                    # Input logic constants to registers or identifiers
                    while True:
                        try:
                            t = next_space(i)
                            iden = line[i: t]
                            if iden in self.registers:
                                self.registers[iden] = 0 if input() == 'f' else 1
                            elif iden in self.globalvars:
                                self.globalvars[iden] = 0 if input() == 'f' else 1
                            else:
                                print('Error: unreferenced or improperly used identifier')
                                return
                            i = t + 1
                        except IndexError:
                            break
                elif opr == 'print':
                    if line[i - 1] == '\n':
                        print()
                    while True:
                        try:
                            t = next_space(i)
                            iden = line[i: t]
                            if iden in self.registers:
                                print('f' if self.registers[iden] == 0 else 't')
                            elif iden in self.globalvars:
                                print('f' if self.globalvars[iden] == 0 else 't')
                            else:
                                print('Error: unreferenced or improperly used identifier')
                                return
                            i = t + 1
                        except IndexError:
                            break
                elif opr == 'rep':
                    raise NotImplementedError
                elif opr == 'not':
                    # Not logic: only accept one operand
                    t = next_space(i)
                    iden = line[i: t]
                    if iden in self.registers:
                        self.registers[iden] = 0 if self.registers[iden] == 1 else 1
                    elif iden in self.globalvars:
                        self.globalvars[iden] = 0 if self.globalvars[iden] == 1 else 1
                    else:
                        print('Error: unreferenced or improperly used identifier')
                        return
                    i = t + 1
                elif opr == 'and':
                    # And logic: accept two or three operands (in C++. Here, as many as needed)
                    operands = []
                    t = next_space(i)
                    first = line[i: t]
                    while True:
                        try:
                            t = next_space(i)
                            iden = line[i: t]
                            if iden in self.registers:
                                operands.append(self.registers[iden])
                            elif iden in self.globalvars:
                                operands.append(self.globalvars[iden])
                            elif iden in ['f', 't']:
                                operands.append(0 if iden == 'f' else 1)
                            else:
                                print('Error: unreferenced or improperly used identifier')
                                return
                            i = t + 1
                        except IndexError:
                            break
                        if first in self.registers:
                            self.registers[first] = 0 if 0 in operands else 1
                        elif first in self.globalvars:
                            self.globalvars[first] = 0 if 0 in operands else 1
                elif opr == 'or':
                    operands = []
                    t = next_space(i)
                    first = line[i: t]
                    while True:
                        try:
                            t = next_space(i)
                            iden = line[i: t]
                            if iden in self.registers:
                                operands.append(self.registers[iden])
                            elif iden in self.globalvars:
                                operands.append(self.globalvars[iden])
                            elif iden in ['f', 't']:
                                operands.append(0 if iden == 'f' else 1)
                            else:
                                print('Error: unreferenced or improperly used identifier')
                                return
                            i = t + 1
                        except IndexError:
                            break
                    if first in self.registers:
                        self.registers[first] = 1 if 1 in operands else 0
                    elif first in self.globalvars:
                        self.globalvars[first] = 1 if 1 in operands else 0
                elif opr == 'andn':
                    operands = []
                    t = next_space(i)
                    first = line[i: t]
                    while True:
                        try:
                            t = next_space(i)
                            iden = line[i: t]
                            if iden in self.registers:
                                operands.append(self.registers[iden])
                            elif iden in self.globalvars:
                                operands.append(self.globalvars[iden])
                            elif iden in ['f', 't']:
                                operands.append(0 if iden == 'f' else 1)
                            else:
                                print('Error: unreferenced or improperly used identifier')
                                return
                            i = t + 1
                        except IndexError:
                            break
                    if first in self.registers:
                        self.registers[first] = 1 if 0 in operands else 0
                    elif first in self.globalvars:
                        self.globalvars[first] = 1 if 0 in operands else 0
                elif opr == 'orn':
                    operands = []
                    t = next_space(i)
                    first = line[i: t]
                    while True:
                        try:
                            t = next_space(i)
                            iden = line[i: t]
                            if iden in self.registers:
                                operands.append(self.registers[iden])
                            elif iden in self.globalvars:
                                operands.append(self.globalvars[iden])
                            elif iden in ['f', 't']:
                                operands.append(0 if iden == 'f' else 1)
                            else:
                                print('Error: unreferenced or improperly used identifier')
                                return
                            i = t + 1
                        except IndexError:
                            break
                    if first in self.registers:
                        self.registers[first] = 0 if 1 in operands else 1
                    elif first in self.globalvars:
                        self.globalvars[first] = 0 if 1 in operands else 1
                elif opr == 'xor':
                    # Only two operands are accepted
                    t = next_space(i)
                    oprd1 = line[i: t]
                    i = t + 1
                    t = next_space(i)
                    oprd2 = line[i: t]
                    if oprd1 in self.registers and oprd2 in self.registers:
                        self.registers[oprd1] = 0 if self.registers[oprd1] == self.registers[oprd2] else 1
                    elif oprd1 in self.registers and oprd2 in self.globalvars:
                        self.registers[oprd1] = 0 if self.registers[oprd1] == self.globalvars[oprd2] else 1
                    elif oprd1 in self.globalvars and oprd2 in self.registers:
                        self.globalvars[oprd1] = 0 if self.globalvars[oprd1] == self.registers[oprd2] else 1
                    elif oprd1 in self.globalvars and oprd2 in self.globalvars:
                        self.globalvars[oprd1] = 0 if self.globalvars[oprd1] == self.globalvars[oprd2] else 1
        except IndexError:
            pass
    
    def run(self, lines):
        for line in lines:
            self.interpret(line)


def main():
    from sys import argv, path
    if len(argv) > 1:
        filename = argv[1]
        lines = readfile(filename)
        itp = interpreter()
        itp.run(lines)
    else:
        lines = readfile(path[0] + '\\sample.lasm')
        itp = interpreter()
        itp.run(lines)

if __name__ == '__main__':
    main()
