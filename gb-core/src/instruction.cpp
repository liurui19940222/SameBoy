#include "../include/instruction.h"
#include "../json/json.h"
#include "../include/tools.h"

void Parse(map<int8, InstructionConfig>& table, Json::Value& node) {
	auto it_0 = node.begin();
	auto end_0 = node.end();
	int16 code;
	Json::Value jsonObj;
	Json::Value::ArrayIndex i = 0;
	while (it_0 != end_0) {
		code = hexText2Int16(it_0.name().c_str());
		jsonObj = node[it_0.name().c_str()];
		InstructionConfig cfg;
		cfg.mnemonic = std::string(jsonObj["mnemonic"].asCString());
		cfg.bytes = (int8)(jsonObj["bytes"].asInt());
		for (i = 0; i < jsonObj["cycles"].size(); ++i) {
			cfg.cycles.push_back((int8)(jsonObj["cycles"][i].asInt()));
		}
		for (i = 0; i < jsonObj["operands"].size(); ++i) {
			Operand operand = {
				string(jsonObj["operands"][i]["name"].asCString()),
				(int8)(jsonObj["operands"][i]["bytes"].asInt()),
				jsonObj["operands"][i]["immediate"].asBool()
			};
			cfg.operands.push_back(operand);
		}
		cfg.immediate = jsonObj["immediate"].asBool();
		for (auto it = jsonObj["flags"].begin(); it != jsonObj["flags"].end(); ++it) {
			cfg.flags.insert({
				string(it.name().c_str()),
				string(jsonObj["flags"][it.name().c_str()].asCString())
				});
		}
		table.insert({ code, cfg });
		it_0++;
	}
}

bool InstructionConfigTable::InitTable(const char* cfgPath) {
	char* text = readAllTexts(cfgPath);
	if (text == NULL) {
		return false;
	}

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(text, root))
	{
		Parse(this->_unprefixedTable, root["unprefixed"]);
		Parse(this->_cbprefixedTable, root["cbprefixed"]);
	}

	free(text);
	return true;
}

bool InstructionConfigTable::GetInstruction(int8 code, bool withCB, InstructionConfig* config) {
	if (withCB) {
		*config = this->_cbprefixedTable[code];
		return this->_cbprefixedTable.count(code) > 0;
	}
	else {
		*config = this->_unprefixedTable[code];
		return this->_unprefixedTable.count(code) > 0;
	}
}