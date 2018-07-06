

#include "stdafx.h"
#include "ApiDebugger.h"
#include "AdHookApi.h"
#include <tlhelp32.h>
#include <wincrypt.h>
#include "Common.h"

static const char *   gCopyright = "ApiDebugger by arry";
static CAdHookApi     gHooks;

bool gEnableLogOutput = true;

/*
extern "C" APIDEBUGGER const char * ApiDebugger()
{
    return gCopyright;
}
*/

///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI my_IsDebuggerPresent(VOID)
{
    return FALSE;
}

/*
int WINAPI my_CompareStringW(LCID Locale, DWORD dwCmpFlags, PCNZWCH lpString1, int cchCount1, 
                          PCNZWCH lpString2,int cchCount2)
{
    CAdAutoHookApi autoHook(&gHooks, my_CompareStringW);
    logOutput(formatString("ApiDebugger - CompareStringW.\r\n"));
    int ret = CompareStringW(Locale, dwCmpFlags, lpString1, cchCount1, lpString2, cchCount2);
    logOutput(formatString("ApiDebugger - CompareStringW(%S, %S).\r\n", lpString1, lpString2));
    return ret;
}

BOOL WINAPI my_CryptAcquireContextW(HCRYPTPROV *phProv, LPCWSTR szContainer, LPCWSTR szProvider, 
                                 DWORD dwProvType, DWORD dwFlags)
{
    CAdAutoHookApi autoHook(&gHooks, my_CryptAcquireContextW);
    BOOL ret = CryptAcquireContextW(phProv, szContainer, szProvider, dwProvType, dwFlags);
    logOutput(formatString("ApiDebugger - CryptAcquireContextW(0x%08X, %S, %S, 0x%08X, 0x%08X) : %S.\r\n",
        (int)(*phProv),
        (szContainer != NULL) ? szContainer : L"NULL",
        (szProvider != NULL) ? szProvider : L"NULL",
        dwProvType, dwFlags,
        ret ? L"TRUE" : L"FALSE"
        ));

    return ret;
}

BOOL WINAPI my_CryptImportKey(HCRYPTPROV hProv, CONST BYTE *pbData, DWORD dwDataLen, HCRYPTKEY hPubKey,
                           DWORD dwFlags, HCRYPTKEY *phKey)
{
    CAdAutoHookApi autoHook(&gHooks, my_CryptImportKey);

    BOOL ret = CryptImportKey(hProv, pbData, dwDataLen, hPubKey, dwFlags, phKey);

    string hexData = toHexString((const char *)pbData, dwDataLen);
    logOutput(formatString("ApiDebugger - CryptImportKey(0x%08X, %s, 0x%08X, 0x%08X, 0x%08X) : %S.\r\n",
        (int)hProv, hexData.c_str(), (int)hPubKey, dwFlags, (int)(*phKey), 
        ret ? L"TRUE" : L"FALSE"
        ));

    return ret;
}

BOOL WINAPI my_CryptCreateHash(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH *phHash)
{
    CAdAutoHookApi autoHook(&gHooks, my_CryptCreateHash);
    BOOL ret = CryptCreateHash(hProv, Algid, hKey, dwFlags, phHash);
    logOutput(formatString("ApiDebugger - CryptCreateHash(0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X) : %S.\r\n",
        (int)hProv, (int)Algid, (int)hKey, dwFlags, (int)phHash,
        ret ? L"TRUE" : L"FALSE"
        ));
    return ret;
}

BOOL WINAPI my_CryptHashData(HCRYPTHASH hHash, CONST BYTE *pbData, DWORD dwDataLen, DWORD dwFlags)
{
    CAdAutoHookApi autoHook(&gHooks, my_CryptHashData);
    BOOL ret = CryptHashData(hHash, pbData, dwDataLen, dwFlags);
    string hexData = toHexString((const char *)pbData, dwDataLen);
    logOutput(formatString("ApiDebugger - CryptHashData(0x%08X, %s, 0x%08X) : %S.\r\n",
        (int)hHash, hexData.c_str(), dwFlags,
        ret ? L"TRUE" : L"FALSE"
        ));
    return ret;
}

BOOL WINAPI my_CryptDeriveKey(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTHASH hBaseData, DWORD dwFlags, HCRYPTKEY *phKey)
{
    CAdAutoHookApi autoHook(&gHooks, my_CryptDeriveKey);
    BOOL ret = CryptDeriveKey(hProv, Algid, hBaseData, dwFlags, phKey);
    logOutput(formatString("ApiDebugger - CryptDeriveKey(0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X) : %S.\r\n",
        (int)hProv, (int)Algid, (int)hBaseData, dwFlags, (int)phKey,
        ret ? L"TRUE" : L"FALSE"
        ));
    return ret;
}

BOOL WINAPI my_CryptDecrypt(HCRYPTKEY hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags,
                            BYTE *pbData, DWORD *pdwDataLen)
{
    CAdAutoHookApi autoHook(&gHooks, my_CryptDecrypt);

    string hexData1 = toHexString((const char *)pbData, *pdwDataLen);
    writeDataToFile("CryptDec_IN.bin", pbData, *pdwDataLen);
    BOOL ret = CryptDecrypt(hKey, hHash, Final, dwFlags, pbData, pdwDataLen);
    string hexData2 = toHexString((const char *)pbData, *pdwDataLen);
    writeDataToFile("CryptDec_OUT.bin", pbData, *pdwDataLen);

    logOutput(formatString("ApiDebugger - CryptDecrypt(0x%08X, 0x%08X, %S, 0x%08X, %s=>%s) : %S.\r\n",
        (int)hKey, (int)hHash, Final ? L"TRUE" : L"FALSE",
        dwFlags, hexData1.c_str(), hexData2.c_str(), 
        ret ? L"TRUE" : L"FALSE"
        ));
    return ret;
}
*/

typedef int (__cdecl *sub_4026B0_func)(BYTE *pbData);

// 004026B0 ; 
static int my_sub_4026B0(BYTE *pbData)
{
    CAdAutoHookApi autoHook(&gHooks, my_sub_4026B0);
    sub_4026B0_func sub_4026B0 = (sub_4026B0_func)(0x004026B0);
    string hexData1 = toHexString((const char *)pbData, strlen((const char *)pbData));
    int ret = sub_4026B0(pbData);
    string hexData2 = toHexString((const char *)pbData, strlen((const char *)pbData));

    logOutput(formatString("ApiDebugger - sub_4026B0(%s=>%s)",
        hexData1.c_str(), hexData2.c_str()));

    return ret;
}

///////////////////////////////////////////////////////////////////////////////


void ApiDebugferShutdown()
{
    gHooks.EndAll();

    logOutput("ApiDebugger Shutdown.\r\n");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        {
       gHooks.Add(_T("KERNEL32.DLL"),      "IsDebuggerPresent",        my_IsDebuggerPresent);
//            gHooks.Add(_T("KERNEL32.DLL"),      "CompareStringW",           my_CompareStringW);
 /*           gHooks.Add(_T("ADVAPI32.DLL"),      "CryptAcquireContextW",     my_CryptAcquireContextW);
            gHooks.Add(_T("ADVAPI32.DLL"),      "CryptImportKey",           my_CryptImportKey);
            gHooks.Add(_T("ADVAPI32.DLL"),      "CryptCreateHash",          my_CryptCreateHash);
            gHooks.Add(_T("ADVAPI32.DLL"),      "CryptHashData",            my_CryptHashData);
            gHooks.Add(_T("ADVAPI32.DLL"),      "CryptDeriveKey",           my_CryptDeriveKey);
            gHooks.Add(_T("ADVAPI32.DLL"),      "CryptDecrypt",             my_CryptDecrypt);
			*/
/*
            const BYTE verifyData[] = { 0x55, 0x8B, 0xEC, 0x81, 0xEC, 0x2C, 0x01, 0x00, 0x00 };
            void *addr = (void *)0x004026B0;
            if(gHooks.Add(addr, my_sub_4026B0, verifyData, sizeof(verifyData), 0) != NULL)
            {
                logOutput(formatString("ApiDebugger - hook sub_4026B0 ok.\r\n"));
            }
            else
            {
                logOutput(formatString("ApiDebugger - hook sub_4026B0 failed.\r\n"));
            }
*/
            gHooks.BeginAll();

            logOutput(formatString("ApiDebugger - %s.\r\n", gCopyright));
            logOutput("ApiDebugger Loaded.\r\n");
       }
        break ;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
        {
        }
        break ;
	case DLL_PROCESS_DETACH:
        {
            ApiDebugferShutdown();
            logOutput("ApiDebugger Unloaded.\r\n");
        }
		break;
	}
	return TRUE;
}

