#include "ClassFactory.h"
#include "DeskBand.h"

extern long                                     g_DllRefCount;

CClassFactory::CClassFactory()
{
    m_RefCount = 1;
    InterlockedIncrement(&g_DllRefCount);
}

CClassFactory::~CClassFactory()
{
    InterlockedDecrement(&g_DllRefCount);
}

//
// IUnknown
//
STDMETHODIMP CClassFactory::QueryInterface(REFIID riid, void **ppv)
{
    HRESULT hr = S_OK;

    if (IsEqualIID(IID_IUnknown, riid) || IsEqualIID(IID_IClassFactory, riid))
    {
        *ppv = static_cast<IUnknown*>(this);
        AddRef();
    }
    else
    {
        hr = E_NOINTERFACE;
        *ppv = nullptr;
    }

    return hr;
}

STDMETHODIMP_(ULONG) CClassFactory::AddRef()
{
    return InterlockedIncrement(&m_RefCount);
}

STDMETHODIMP_(ULONG) CClassFactory::Release()
{
    ULONG cRef = InterlockedDecrement(&m_RefCount);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}

//
// IClassFactory
//
STDMETHODIMP CClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    HRESULT hr = CLASS_E_NOAGGREGATION;

    if (!pUnkOuter)
    {
        hr = E_OUTOFMEMORY;

        CDeskBand *pDeskBand = new CDeskBand();
        if (pDeskBand)
        {
            hr = pDeskBand->QueryInterface(riid, ppv);
            pDeskBand->Release();
        }
    }

    return hr;
}

STDMETHODIMP CClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
    {
        InterlockedIncrement(&g_DllRefCount);
    }
    else
    {
        InterlockedDecrement(&g_DllRefCount);
    }
    return S_OK;
}
