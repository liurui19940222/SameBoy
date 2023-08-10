#pragma once

#include <vector>
#include <string>
#include "memory.h"
#include "instruction.h"

using namespace std;

struct InstructionDetail
{
	uint16 address;
	string longText;
};

class Disassembler
{
public:
	Disassembler(MemoryBus* bus, InstructionConfigTable* insCfgTable);

	void Decode(vector<InstructionDetail>& list);

private:
	MemoryBus* _bus;

	InstructionConfigTable* _insCfgTable;

	void _fetchParamName(int16 operandStartAddress, Operand& operand, string& name);
};

