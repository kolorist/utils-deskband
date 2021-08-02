#include <Windows.h>
#include <strsafe.h>
#include <olectl.h>
#include <shlobj.h>

#include "ClassFactory.h"

// {2EFF11C4-0FE2-47AC-A602-36E5ADF81774}
CLSID CLSID_UtilsDeskBand                       = { 0x2eff11c4, 0xfe2, 0x47ac, { 0xa6, 0x2, 0x36, 0xe5, 0xad, 0xf8, 0x17, 0x74 } };
HINSTANCE g_hInstance                           = NULL;
long g_DllRefCount                              = 0;

// ============================================================================

HRESULT RegisterServer()
{
    WCHAR szCLSID[MAX_PATH];
    StringFromGUID2(CLSID_UtilsDeskBand, szCLSID, ARRAYSIZE(szCLSID));

    WCHAR szSubkey[MAX_PATH];
    HKEY hKey;

    HRESULT hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
    if (SUCCEEDED(hr))
    {
        hr = E_FAIL;
        if (ERROR_SUCCESS == RegCreateKeyExW(HKEY_CLASSES_ROOT,
                                             szSubkey,
                                             0,
                                             NULL,
                                             REG_OPTION_NON_VOLATILE,
                                             KEY_WRITE,
                                             NULL,
                                             &hKey,
                                             NULL))
        {
            WCHAR const szName[] = L"Utils DeskBand";
            if (ERROR_SUCCESS == RegSetValueExW(hKey,
                                                NULL,
                                                0,
                                                REG_SZ,
                                                (LPBYTE) szName,
                                                sizeof(szName)))
            {
                hr = S_OK;
            }

            RegCloseKey(hKey);
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s\\InprocServer32", szCLSID);
        if (SUCCEEDED(hr))
        {
            hr = HRESULT_FROM_WIN32(RegCreateKeyExW(HKEY_CLASSES_ROOT, szSubkey,
                 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL));
            if (SUCCEEDED(hr))
            {
                WCHAR szModule[MAX_PATH];
                if (GetModuleFileNameW(g_hInstance, szModule, ARRAYSIZE(szModule)))
                {
                    DWORD cch = lstrlen(szModule);
                    hr = HRESULT_FROM_WIN32(RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE) szModule, cch * sizeof(szModule[0])));
                }

                if (SUCCEEDED(hr))
                {
                    WCHAR const szModel[] = L"Apartment";
                    hr = HRESULT_FROM_WIN32(RegSetValueExW(hKey, L"ThreadingModel", 0,  REG_SZ, (LPBYTE) szModel, sizeof(szModel)));
                }

                RegCloseKey(hKey);
            }
        }
    }

    return hr;
}

HRESULT RegisterComCat()
{
    ICatRegister *pcr;
    HRESULT hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pcr));
    if (SUCCEEDED(hr))
    {
        CATID catid = CATID_DeskBand;
        hr = pcr->RegisterClassImplCategories(CLSID_UtilsDeskBand, 1, &catid);
        pcr->Release();
    }
    return hr;
}

// ============================================================================

STDAPI_(BOOL) DllMain(HINSTANCE hInstance, DWORD dwReason, void *)
{
    OutputDebugString(TEXT("DllMain...\n"));
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        OutputDebugString(TEXT("DllMain attach to process\n"));
        g_hInstance = hInstance;
        // Disables the DLL_THREAD_ATTACH and DLL_THREAD_DETACH
        DisableThreadLibraryCalls(g_hInstance);
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    OutputDebugString(TEXT("DllGetClassObject..."));
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
	
    if (IsEqualCLSID(CLSID_UtilsDeskBand, rclsid))
    {
        hr = E_OUTOFMEMORY;

        CClassFactory *pClassFactory = new CClassFactory();
        if (pClassFactory)
        {
            hr = pClassFactory->QueryInterface(riid, ppv);
            pClassFactory->Release();
        }
    }
	
    return hr;
}

STDAPI DllCanUnloadNow()
{
    OutputDebugString(TEXT("DllCanUnloadNow..."));
	return g_DllRefCount > 0 ? S_FALSE : S_OK;
}

STDAPI DllRegisterServer()
{
    OutputDebugString(TEXT("DllRegisterServer..."));
    HRESULT hr = RegisterServer();
    if (SUCCEEDED(hr))
    {
        // Register the component category for the deskband object.
        hr = RegisterComCat();
    }

    return SUCCEEDED(hr) ? S_OK : SELFREG_E_CLASS;
}

STDAPI DllUnregisterServer()
{
    OutputDebugString(TEXT("DllUnregisterServer..."));
    WCHAR szCLSID[MAX_PATH];
    StringFromGUID2(CLSID_UtilsDeskBand, szCLSID, ARRAYSIZE(szCLSID));

    WCHAR szSubkey[MAX_PATH];
    HRESULT hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
    if (SUCCEEDED(hr))
    {
        if (ERROR_SUCCESS != RegDeleteTreeW(HKEY_CLASSES_ROOT, szSubkey))
        {
            hr = E_FAIL;
        }
    }

    return SUCCEEDED(hr) ? S_OK : SELFREG_E_CLASS;
}
