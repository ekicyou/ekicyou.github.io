// ----------------------------------------------------------------------------------------------
// Copyright (c) Mattias Högström.
// ----------------------------------------------------------------------------------------------
// This source code is subject to terms and conditions of the Microsoft Public License. A 
// copy of the license can be found in the License.html file at the root of this distribution. 
// If you cannot locate the Microsoft Public License, please send an email to 
// dlr@microsoft.com. By using this source code in any fashion, you are agreeing to be bound 
// by the terms of the Microsoft Public License.
// ----------------------------------------------------------------------------------------------
// You must not remove this notice, or any other, from this software.
// ----------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "MinimalHostControl.h"

void RunExecutable(TCHAR* fileName, LPCWSTR runtimeVersion)
{
   TCHAR totalPath[512];
   TCHAR* assemblyFileName = nullptr;

   // AppDomain.ExecuteAssembly needs an absolute path
   // Therefore we need to convert the relative ones
   DWORD len = GetFullPathName(fileName, sizeof(totalPath) / sizeof(TCHAR), totalPath, &assemblyFileName);

   ICLRMetaHost       *pMetaHost       = nullptr;
   ICLRMetaHostPolicy *pMetaHostPolicy = nullptr;
   ICLRRuntimeHost    *pRuntimeHost    = nullptr;
   ICLRRuntimeInfo    *pRuntimeInfo    = nullptr;
   HRESULT hr;
   hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost,
      (LPVOID*)&pMetaHost);
   hr = pMetaHost->GetRuntime(runtimeVersion, IID_PPV_ARGS(&pRuntimeInfo));
   if (FAILED(hr))
   {
      wprintf_s(L"Failed to start .Net runtime %s\n", runtimeVersion);
      goto cleanup;
   }

   hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost,IID_PPV_ARGS(&pRuntimeHost));         

   ICLRControl* pCLRControl = nullptr;
   hr = pRuntimeHost->GetCLRControl(&pCLRControl);

   MinimalHostControl* pMyHostControl = pMyHostControl = new MinimalHostControl();
   hr = pRuntimeHost->SetHostControl(pMyHostControl);

   LPCWSTR appDomainManagerTypename = L"SampleAppDomainManager.CustomAppDomainManager";
   LPCWSTR assemblyName = L"SampleAppDomainManager";
   hr = pCLRControl->SetAppDomainManagerType(assemblyName, appDomainManagerTypename);

   wprintf(L"Running runtime version: %s\n", runtimeVersion);
   wprintf_s(L"--- Start ---\n");
   hr = pRuntimeHost->Start();
   
   ICustomAppDomainManager* pAppDomainManager = pMyHostControl->GetDomainManagerForDefaultDomain();
   BSTR assemblyFilename = fileName;

   BSTR friendlyname = L"TestApp";
   hr = pAppDomainManager->Run(assemblyFilename, friendlyname);

   wprintf_s(L"--- End ---\n");
   hr = pRuntimeHost->Stop();

   cleanup:
   if (pRuntimeInfo != nullptr)
   {
      pRuntimeInfo->Release();
      pRuntimeInfo = nullptr;
   }
   if (pRuntimeHost != nullptr)
   {
      pRuntimeHost->Release();
      pRuntimeHost = nullptr;
   }
   if (pMetaHost != nullptr)
   {
      pMetaHost->Release();
      pMetaHost = nullptr;
   }
}

BOOL FileExists(LPCTSTR szPath) 
{ 
   DWORD dwAttrib = GetFileAttributes(szPath);
   return (dwAttrib != INVALID_FILE_ATTRIBUTES); 
} 

int _tmain(int argc, _TCHAR* argv[])
{
   if (argc != 2)
   {
      wprintf_s(L"Usage: SampleAppLauncher <path to exe>\n");
      return 0;
   }

   if (FileExists(argv[1]))
   {
      // Explicitly setting the runtime version
      // This is probably not future safe
      RunExecutable(argv[1], L"v4.0.30319");
      return 0;
   }
   else
   {
      wprintf_s(L"%s is not found\n", argv[1]);
      return 0;
   }
}

