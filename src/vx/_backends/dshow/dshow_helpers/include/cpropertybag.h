// 
// Copyright (c) Microsoft Corporation.  All rights reserved. 
// 
// 
// Use of this source code is subject to the terms of the Microsoft end-user 
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT. 
// If you did not accept the terms of the EULA, you are not authorized to use 
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your 
// install media. 
// 

#ifndef _PROPERTY_BAG_
#define _PROPERTY_BAG_ 1

#include "ds_helper.h"

struct VAR_LIST 
{ 
	VARIANT var; 
	VAR_LIST *pNext; 
	BSTR pBSTRName; 
}; 

class CPropertyBag : public IPropertyBag 
{   
public: 
	CPropertyBag(); 
	~CPropertyBag(); 

	HRESULT STDMETHODCALLTYPE 
		Read( 
		LPCOLESTR pszPropName,  
		VARIANT *pVar,  
		IErrorLog *pErrorLog 
		); 


	HRESULT STDMETHODCALLTYPE 
		Write( 
		LPCOLESTR pszPropName,  
		VARIANT *pVar 
		); 

	ULONG STDMETHODCALLTYPE AddRef();         
	ULONG STDMETHODCALLTYPE Release();         
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);    

private: 
	ULONG _refCount; 
	VAR_LIST *pVar; 
}; 

#endif // _PROPERTY_BAG_