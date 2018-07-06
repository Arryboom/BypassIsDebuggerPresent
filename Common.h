

#pragma once

#include <string>
using namespace std;

extern bool gEnableLogOutput;

wstring getAppNameW();
wstring getAppPath();
string  getModuleNameA(HMODULE hModule);
string  getAppNameA();
string  getAppPathA();
string  getFileName(const string &filePath);
string  formatString(const char *format, ...);
string  toHexString(const char *data, int size);
string  getLogString();
int     writeDataToFile(const char *fileName, void *buffer, int size);
int     readDataFromFile(const char *fileName, void *buffer, int size);
int     appendDataToFile(const char *fileName, void *buffer, int size);
int     logOutput(const char *text);
int     logOutput(const string &text);
