#include <tchar.h>
#include <stdarg.h>
#include <string>
#include <codecvt>
#include <locale>
#include <vector>
#include "../include/win_helper.h"

void showMessageBox(LPCWSTR title, LPCWSTR content)
{
    MessageBox(NULL, content, title, MB_OK);
}

std::string openFileDialog() {
    OPENFILENAME ofn;
    WCHAR szFile[MAX_PATH] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"GB Files (*.gb)\0*.gb\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    if (GetOpenFileName(&ofn))
        return converter.to_bytes(szFile);
    else
        return "";
}

std::string getCurrentDir() {
    DWORD size = GetCurrentDirectory(0, NULL);
    std::vector<wchar_t> buffer(size);
    GetCurrentDirectory(size, &buffer[0]);
    std::wstring wstr(&buffer[0]);
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}