#include <sstream>
#include <iomanip>
#include "../include/disassembler.h"
#include "../include/datatype.h"
#include "../include/tools.h"

const char* get(vector<string>& list, int index) {
	return index < (int)list.size() ? list[index].c_str() : "  ";
}

Disassembler::Disassembler(MemoryBus* bus, InstructionConfigTable* insCfgTable)
{
	this->_bus = bus;
	this->_insCfgTable = insCfgTable;
}

void Disassembler::Decode(vector<InstructionDetail>& list)
{
	int index;
	InstructionConfig ins;
	bool rightIns;

	uint16 address = 0x0;
	int16 operandStartAddress;
	std::string tmpStr0;
	std::string tmpStr1;
	uint8 firstCode, secondCode;

	InstructionDetail detail;

	vector<string> bytes;
	while (address < 0x8000) {
		firstCode = _bus->ReadByte(address);
		secondCode = -1;
		if (firstCode == 0xCB) {
			secondCode = _bus->ReadByte(address + 1);
			rightIns = _insCfgTable->GetInstruction(secondCode, true, &ins);
			operandStartAddress = address + 2;
		}
		else {
			rightIns = _insCfgTable->GetInstruction(firstCode, false, &ins);
			operandStartAddress = address + 1;
		}
		if (!rightIns) {
			break;
		}
		
		detail.address = address;

		bytes.clear();
		for (index = 0; index < ins.bytes - 1; ++index) {
			bytes.push_back(format("%02x", _bus->ReadByte(address + index)));
		}

		switch (ins.operands.size()) {
		case 0:
			detail.longText = format("%04x\t%02s\t%02s\t%-10s\n", address, get(bytes, 0), get(bytes, 1), ins.mnemonic.c_str());
			break;
		case 1:
			_fetchParamName(operandStartAddress, ins.operands[0], tmpStr0);
			detail.longText = format("%04x\t%02s\t%02s\t%-10s\t%s\n", address, get(bytes, 0), get(bytes, 1), ins.mnemonic.c_str(), tmpStr0.c_str());
			break;
		case 2:
			_fetchParamName(operandStartAddress, ins.operands[0], tmpStr0);
			_fetchParamName(operandStartAddress + ins.operands[0].bytes, ins.operands[1], tmpStr1);
			detail.longText = format("%04x\t%02s\t%02s\t%-10s\t%s, %s\n", address, get(bytes, 0), get(bytes, 1), ins.mnemonic.c_str(), tmpStr0.c_str(), tmpStr1.c_str());
			break;
		}

		list.push_back(detail);

		address += ins.bytes;
	}
}

void Disassembler::_fetchParamName(int16 operandStartAddress, Operand& operand, string& name) {
	stringstream ss;

	if (operand.bytes == 0) {
		name = operand.name;
	}
	else if (operand.bytes == 1) {
		ss.clear();
		uint8 value = _bus->ReadByte(operandStartAddress);
		uint16 v = (uint16)value;
		ss << std::hex << setw(4) << setfill('0') << v;
		name = ss.str();
	}
	else if (operand.bytes == 2) {
		ss.clear();
		uint16 value = _bus->Read2Byte(operandStartAddress);
		ss << std::hex << setw(4) << setfill('0') << value;
		name = ss.str();
	}
	else {
		name = "error";
	}
	if (!operand.immediate) {
		name = "(" + name + ")";
	}
}