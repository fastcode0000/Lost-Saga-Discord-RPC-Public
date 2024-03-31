#include "StdAfx.h"
#include "Application.h"
#include <DiscordRPC.h>
//https://github.com/fastcode0000/DiscordRPC_Static_Lib 참고
#include "MemoryManager.hpp"

using namespace std;

std::string multibyte_to_utf8(const std::string& str)
{
    int nLen = str.size();
    wchar_t warr[256];
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, warr, 256);
    char carr[256];
    memset(carr, '\0', sizeof(carr));
    WideCharToMultiByte(CP_UTF8, 0, warr, -1, carr, 256, NULL, NULL);
    return carr;
}

void OnThread()
{
    bool bResult = g_App.MemoryInit();
    if (!bResult)
    {
        //cout << "Memory Init ERROR" << endl;
        //예외 필요
        return;
    }

    //cout << "Memory Init OK" << endl;

    g_Discord.SetData("1064183848096780439", "", "Lost Saga In Korea", "mode_anything", "image", "normal");
    g_Discord.Initialize();

    //cout << "Discord Init OK" << endl;

    while (true)
    {
        if (g_App.IsModeChanged())
        {
            char szMode[MAX_PATH], szImage[MAX_PATH];
            g_App.GetModeAndImageName(szMode, szImage);


            char szName[MAX_PATH] = "";
            g_App.GetNickName(szName);
            std::string ok = multibyte_to_utf8(szName);

            char szBuf[MAX_PATH];
            sprintf(szBuf, "%s - %s", szMode, ok.c_str());

            g_Discord.SetData("1064183848096780439", "", szBuf, szImage, "image", "normal");
            g_Discord.Update();
        }

        Sleep(2000);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        /*if (AllocConsole())
        {
            freopen("CONIN$", "rb", stdin);
            freopen("CONOUT$", "wb", stdout);
            freopen("CONOUT$", "wb", stderr);
            ios::sync_with_stdio();
        }*/
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OnThread, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}