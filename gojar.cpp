// gojar.cpp : Run a jar of the same name as the EXE from the same folder
// Tested on Win 7 x64
// Copyright (C)2011 Chris Poole - chris@hackernet.co.uk

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include "shlwapi.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	DWORD dwType;
	LONG lResult;

	// Retrieve the current jre version to get at the path
	TCHAR szCurrentVersion[32];
	DWORD dwLenCurrentVersion = sizeof(szCurrentVersion) / sizeof(TCHAR);

	lResult = RegGetValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\JavaSoft\\Java Runtime Environment"), _T("CurrentVersion"), RRF_RT_REG_SZ, &dwType, szCurrentVersion, &dwLenCurrentVersion);
	if (lResult != ERROR_SUCCESS)
	{
		MessageBox(NULL, _T("Unable to locate HKEY_LOCAL_MACHINE\\SOFTWARE\\JavaSoft\\Java Runtime Environment\\CurrentVersion"), _T("Error"), MB_OK | MB_ICONERROR);
		return 1;
	}

	TCHAR szJavaHomePath[MAX_PATH] = _T("SOFTWARE\\JavaSoft\\Java Runtime Environment\\");
	StrCat(szJavaHomePath, szCurrentVersion);

	// Build the location of javaw.exe
	TCHAR szJavaHome[MAX_PATH];
	DWORD dwLenJavaHome = sizeof(szJavaHome) / sizeof(TCHAR);

	lResult = RegGetValue(HKEY_LOCAL_MACHINE, szJavaHomePath, _T("JavaHome"), RRF_RT_REG_SZ, &dwType, szJavaHome, &dwLenJavaHome);
	if (lResult != ERROR_SUCCESS)
	{
		MessageBox(NULL, _T("Unable to locate JavaHome key"), _T("Error"), MB_OK | MB_ICONERROR);
		return 2;
	}

	StrCat(szJavaHome, _T("\\bin\\javaw.exe"));

	TCHAR szMyPath[MAX_PATH];

	GetModuleFileName(NULL, szMyPath, MAX_PATH);
	PathRemoveArgs(szMyPath);
	PathRemoveExtension(szMyPath);

	// Build the command line we will pass to javaw.exe
	TCHAR szExec[MAX_PATH] = _T("");
	StrCat(szExec, _T("javaw.exe -jar \""));
	StrCat(szExec, szMyPath);
	StrCat(szExec, _T(".jar\""));

	// Execute javaw with the command line
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	BOOL bSuccess = CreateProcess(szJavaHome, szExec, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (!bSuccess)
	{
		MessageBox(NULL, _T("Unable to execute javaw.exe"), _T("Error"), MB_OK | MB_ICONERROR);
		return 3;
	}

	return 0;
}

