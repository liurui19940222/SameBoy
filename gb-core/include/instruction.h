#pragma once

#include <string>
#include <map>
#include <vector>
#include "datatype.h"

using namespace std;

struct Operand
{
	string name;
	int8 bytes;
	bool immediate;
};

struct InstructionConfig
{
	string mnemonic;
	int8 bytes;
	vector<int8> cycles;
	vector<Operand> operands;
	map<string, string> flags;
	bool immediate;
};

class InstructionConfigTable
{
public:
	bool InitTable(const char* cfgPath);

	bool GetInstruction(int8 code, bool withCB, InstructionConfig* config);

private:
	map<int8, InstructionConfig> _unprefixedTable;

	map<int8, InstructionConfig> _cbprefixedTable;
};

