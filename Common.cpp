

#include "stdafx.h"
#include "Common.h"

static const char * gLogFile = "ApiDebugger.log";

wstring getAppNameW()
{
	wstring appName;
	wchar_t szPathName[_MAX_PATH];
	memset(szPathName, 0, sizeof(szPathName));
	GetModuleFileNameW(NULL, szPathName, _MAX_PATH);
	appName = szPathName;
	return appName;
}

wstring getAppPath()
{
	wstring appName = getAppNameW();
	int pos = (int)appName.find_last_of(L"\\/");
	return appName.substr(0, pos+1);
}

string getModuleNameA(HMODULE hModule)
{
	string appName;
	char szPathName[_MAX_PATH];
	memset(szPathName, 0, sizeof(szPathName));
	GetModuleFileNameA(hModule, szPathName, _MAX_PATH);
	appName = szPathName;
	return appName;
}

string getAppNameA()
{
    return getModuleNameA(NULL);
}

string getAppPathA()
{
	string appName = getAppNameA();
	int pos = (int)appName.find_last_of("\\/");
	return appName.substr(0, pos+1);
}

string getFileName(const string &filePath)
{
    int pos = (int)filePath.find_last_of("\\/");
    if(pos > 0)
    {
        return filePath.substr(pos+1);
    }
    return filePath;
}

string formatString(const char *format, ...)
{
	const int BUFFER_SIZE = 4097;
	char strbuf[BUFFER_SIZE];
    memset(strbuf, 0, sizeof(strbuf));

	va_list ap;
	va_start (ap, format);
	int result = _vsnprintf(strbuf, BUFFER_SIZE-1, format, ap);
	va_end (ap);

	return string(strbuf, result);
}

string toHexString(const char *data, int size)
{
	string result(size*2, 0);
	static char *chars = "0123456789ABCDEF";
	for(int i = 0; i < size; i ++)
	{
		char ch = data[i];
		result[i*2+0] = chars[(ch & 0xF0) >> 4];
		result[i*2+1] = chars[(ch & 0x0F)];
	}
	return result;
}

string getLogString()
{
    SYSTEMTIME sysTime = { 0 };
    GetLocalTime(&sysTime);
    return formatString("[% 6d]%02d:%02d:%02d.%03d", (int)GetCurrentThreadId(),
        (int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond, (int)sysTime.wMilliseconds);
}

int writeDataToFile(const char *fileName, void *buffer, int size)
{
    string filePath = getAppPathA() + fileName;
    FILE *pFile = fopen(filePath.c_str(), "wb");
    if(pFile != NULL)
    {
        int ret = fwrite(buffer, 1, size, pFile);
        fclose(pFile);
        pFile = NULL;
        return ret;
    }
    return -1;
}

int readDataFromFile(const char *fileName, void *buffer, int size)
{
    string filePath = getAppPathA() + fileName;
    FILE *pFile = fopen(filePath.c_str(), "rb");
    if(pFile != NULL)
    {
        int ret = fread(buffer, 1, size, pFile);
        fclose(pFile);
        pFile = NULL;
        return ret;
    }
    return -1;
}

int appendDataToFile(const char *fileName, void *buffer, int size)
{
    string filePath = getAppPathA() + fileName;
    FILE *pFile = fopen(filePath.c_str(), "ab");
    if(pFile != NULL)
    {
        fseek(pFile, 0, SEEK_END);
        int ret = fwrite(buffer, 1, size, pFile);
        fclose(pFile);
        pFile = NULL;
        return ret;
    }
    return -1;
}

int logOutput(const char *text)
{
    if(!gEnableLogOutput)
    {
        return false;
    }
    string filePath = getAppPathA() + gLogFile;
    FILE *pFile = fopen(filePath.c_str(), "ab");
    if(pFile != NULL)
    {
        fseek(pFile, 0, SEEK_END);
        string tstr = getLogString() + " - ";
        int ret = fwrite(tstr.c_str(), 1, tstr.length(), pFile);
        ret = fwrite(text, 1, strlen(text), pFile);
        fclose(pFile);
        pFile = NULL;
        return ret;
    }
    return -1;
}

int logOutput(const string &text)
{
    return logOutput(text.c_str());
}
