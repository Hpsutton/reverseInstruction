#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

struct Instructions {
    string inst;
    int code;
    int functionCode;
};

Instructions instr[5] = {
    {"add", 0, 32},
    {"addi", 8, -1},
    {"sub", 0, 34},
    {"lw", 35, -1},
    {"sw", 43, -1}
};

struct RegTable {
    string reg;
    int address;
};

RegTable registers[6] = {
    {"$t0", 8},
    {"$t1", 9},
    {"$t2", 10},
    {"$0", 0},
    {"$s0", 16},
    {"$s1", 17}
};

string getRegister(int address) {
    for (int i = 0; i < 6; i++) {
        if (registers[i].address == address) {
            return registers[i].reg;
        }
    }
    return "UNKNOWN";
}

string getInstruction(int code, int functionCode) {
    for (int i = 0; i < 5; i++) {
        if (instr[i].code == code) {
            if (code != 0 || instr[i].functionCode == functionCode) {
                return instr[i].inst;
            }
        }
    }
    return "UNKNOWN";
}

void decode(uint32_t w) {
    int opcode = (w >> 26) & 0x3F;
    int rs = (w >> 21) & 0x1F;
    int rt = (w >> 16) & 0x1F;
    int rd = (w >> 11) & 0x1F;
    int functionCode = w & 0x3F;
    int immediate = w & 0xFFFF;

    if (immediate >= 32768) {
        immediate -= 65536;
    }

    string instruction = getInstruction(opcode, functionCode);

    if (instruction == "UNKNOWN") {
        cout << "UNKNOWN" << endl;
        return;
    }
    else if (opcode == 0) {
        cout << instruction << " " 
             << getRegister(rd) << ", " 
             << getRegister(rs) << ", " 
             << getRegister(rt) << endl;
        return;
    }
    else if (opcode == 8) {
        cout << instruction << " " 
             << getRegister(rt)
             << ", " << getRegister(rs) 
             << ", " << immediate << endl;
        return;
    }
    else if (opcode == 35 || opcode == 43) {
        cout << instruction << " " 
             << getRegister(rt) 
             << ", " << immediate 
             << "(" << getRegister(rs) 
             << ")" << endl;
        return;
    }
}

int main() {
    ifstream inputFile("test.txt");
    if (!inputFile) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    ifstream inputFile2("test2.txt");
    if (!inputFile2) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    uint32_t instruction;

    cout << "Instructions from test.txt:" << endl;
    while (inputFile >> hex >> instruction) {
        decode(instruction);
    }

    cout << endl;
    cout << "Instructions from test2.txt:" << endl;
    while (inputFile2 >> hex >> instruction) {
        decode(instruction);
    }

    inputFile.close();
    inputFile2.close();

    return 0;
}