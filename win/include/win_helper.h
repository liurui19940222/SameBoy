#define UNICODE

#include <Windows.h>
#include <string.h>

void showMessageBox(LPCWSTR title, LPCWSTR content);

std::string openFileDialog();

std::string getCurrentDir();
