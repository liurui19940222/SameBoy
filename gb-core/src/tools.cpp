#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <codecvt>
#include <locale>
#include <vector>
#include "../include/tools.h"
#include "../include/datatype.h"

using namespace std;

std::string format(const char* formatStr, ...) {
    va_list args;
    va_start(args, formatStr);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), formatStr, args);
    va_end(args);
    return string(buffer);
}

void log(const char* formatStr, ...)
{
    va_list args;
    va_start(args, formatStr);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), formatStr, args);
    printf("%s\n", buffer);
    va_end(args);
}

char* readAllTexts(const char* filename) {
    FILE* textfile;
    char* text;
    long    numbytes;

    textfile = fopen(filename, "r");
    if (textfile == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);

    text = (char*)calloc(numbytes, sizeof(char));
    if (text == NULL)
        return NULL;

    fread(text, sizeof(char), numbytes, textfile);
    fclose(textfile);

    return text;
}

int16 hexText2Int16(const char* str) {
    std::string hex_str = str;
    int num = std::stoi(hex_str, nullptr, 16);
    return (int16)num;
}

void setBit(uint8& value, int bitIndex, int bitValue) {
    if (bitValue != 0) {
        value |= (1 << (uint8)bitIndex);
    }
    else {
        value &= ~(1 << (uint8)bitIndex);
    }
}

bool checkBit(uint8 value, int bitIndex) {
    return (value & (1 << bitIndex)) != 0;
}