#pragma once

#include <string>
#include "datatype.h"

#define MAX_LOG_LEN (1024 * 1024)

std::string format(const char* formatStr, ...);

void log(const char* formatStr, ...);

char* readAllTexts(const char* filename);

int16 hexText2Int16(const char* str);

void setBit(uint8& value, int bitIndex, int bitValue);

bool checkBit(uint8 value, int bitIndex);

inline uint8 bitValue(uint8 value, uint8 bit) { return (value >> bit) & 1; }

inline bool inRange(uint16 value, uint16 min, uint16 max) { return value >= min && value <= max; }