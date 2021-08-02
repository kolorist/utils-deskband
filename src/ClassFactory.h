#pragma once

#include <unknwn.h> // for IClassFactory
#include <windows.h>

class CClassFactory : public IClassFactory
{
public:
    CClassFactory();

protected:
    virtual ~CClassFactory();

public:
    // IUnknown
    STDMETHODIMP                                QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG)                        AddRef();
    STDMETHODIMP_(ULONG)                        Release();

    // IClassFactory
    STDMETHODIMP                                CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv);
    STDMETHODIMP                                LockServer(BOOL fLock);


private:
    LONG                                        m_RefCount;
};

