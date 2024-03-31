#include "StdAfx.h"
#include <TlHelp32.h>
#include <fstream>
#include "injector.h"
#define RPCLib "RPCDLL.dll"

using namespace std;


DWORD GetProcessID(const char* procName)
{
	DWORD procID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_stricmp(procEntry.szExeFile, procName))
				{
					procID = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procID;
}

bool IsCorrectTargetArchitecture(HANDLE hProc)
{
	BOOL bTarget = FALSE;
	if (!IsWow64Process(hProc, &bTarget))
	{
		printf("Can't confirm target process architecture: 0x%X\n", GetLastError());
		return false;
	}

	BOOL bHost = FALSE;
	IsWow64Process(GetCurrentProcess(), &bHost);

	return (bTarget == bHost);
}

int main()
{
	if (GetFileAttributes(RPCLib) == INVALID_FILE_ATTRIBUTES)
	{
		printf("DLL ERROR\n");
		Sleep(5000);
		return -1;
	}

	DWORD dwPID = 0;
	while (true)
	{
		dwPID = GetProcessID("lostsaga.exe");
		if (dwPID != 0)
		{
			break;
		}
	}

	//매핑인젝션 https://github.com/TheCruZ/Simple-Manual-Map-Injector 참고
	TOKEN_PRIVILEGES priv = { 0 };
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!hProc)
	{
		DWORD Err = GetLastError();
		printf("OpenProcess failed: 0x%X\n", Err);
		system("PAUSE");
		return -2;
	}

	if (!IsCorrectTargetArchitecture(hProc))
	{
		printf("Invalid Process Architecture.\n");
		CloseHandle(hProc);
		system("PAUSE");
		return -3;
	}

	std::ifstream File(RPCLib, std::ios::binary | std::ios::ate);

	if (File.fail())
	{
		printf("Opening the file failed: %X\n", (DWORD)File.rdstate());
		File.close();
		CloseHandle(hProc);
		system("PAUSE");
		return -5;
	}

	auto FileSize = File.tellg();
	if (FileSize < 0x1000)
	{
		printf("Filesize invalid.\n");
		File.close();
		CloseHandle(hProc);
		system("PAUSE");
		return -6;
	}

	BYTE* pSrcData = new BYTE[(UINT_PTR)FileSize];
	if (!pSrcData)
	{
		printf("Can't allocate dll file.\n");
		File.close();
		CloseHandle(hProc);
		system("PAUSE");
		return -7;
	}

	File.seekg(0, std::ios::beg);
	File.read((char*)(pSrcData), FileSize);
	File.close();

	printf("Mapping...\n");
	if (!ManualMapDll(hProc, pSrcData, FileSize))
	{
		delete[] pSrcData;
		CloseHandle(hProc);
		printf("Error while mapping.\n");
		system("PAUSE");
		return -8;
	}
	delete[] pSrcData;

	CloseHandle(hProc);
	printf("OK\n");
	return 0;

}