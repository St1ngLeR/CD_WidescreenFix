#include "pch.h"
#include "ProcessMem.cpp"
#include "VerDef.cpp"
#include <filesystem>

using namespace std;

ProcessMem PM;
VerDef VD;

int gcd(int a, int b)
{
    int result = min(a, b);
    while (result > 0) {
        if (a % result == 0 && b % result == 0) {
            break;
        }
        result--;
    }
    return result;
}

int offsets[8];

float AspectWidth;
float AspectHeight;

float scale_general;
float scale_2d_x;
float scale_2d_y;
float guibackground_scale_y;

float view_range = 10000000.0f;

string FullPath = PM.GetProcessPathByPID(GetCurrentProcessId());
filesystem::path p(FullPath);
string filename = p.filename().string();
uintptr_t moduleBase = PM.GetModuleBaseAddress(GetCurrentProcessId(), filename.c_str());
HANDLE hProcess = GetCurrentProcess();

DWORD WINAPI MainTHREAD(LPVOID)
{
    AspectWidth = GetSystemMetrics(SM_CXSCREEN) / gcd(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    AspectHeight = GetSystemMetrics(SM_CYSCREEN) / gcd(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

    scale_general = (AspectWidth / AspectHeight / 1.1875f);
    scale_2d_x = ((AspectWidth / AspectHeight / 1.333333333333333f / 1.1875f) * 0.00156250002328306f) / (AspectWidth / AspectHeight / 1.777777777777777f);
    scale_2d_y = (640.0f * 1.1875f / (AspectWidth / AspectHeight / 1.333333333333333f)) * (AspectWidth / AspectHeight / 1.777777777777777f);
    guibackground_scale_y = 0.5f / (AspectWidth / AspectHeight);

    int gamever = VD.Init(hProcess,moduleBase);
    if (gamever == VERSION_10)
    {
        offsets[0] = 0x1F6EF1;
        offsets[1] = 0x2E6899;
        offsets[2] = 0x2E68A1;
        offsets[3] = 0x2B44EE;
        offsets[4] = 0x13D075;
        offsets[5] = NULL;
        offsets[6] = 0x2C9981;
        offsets[7] = 0x22DC7D;
    }
    else if (gamever == VERSION_11)
    {
        offsets[0] = 0x1FBF11;
        offsets[1] = 0x2ED59C;
        offsets[2] = 0x2ED5A4;
        offsets[3] = 0x2BABAD;
        offsets[4] = 0x13F535;
        offsets[5] = 0x149E38;
        offsets[6] = 0x2D00C0;
        offsets[7] = 0x232DED;
    }
    else if (gamever == VERSION_12)
    {
        offsets[0] = 0x1FBF01;
        offsets[1] = 0x2ED584;
        offsets[2] = 0x2ED58C;
        offsets[3] = 0x2BABAD;
        offsets[4] = 0x13F4B5;
        offsets[5] = 0x149E08;
        offsets[6] = 0x2D00B8;
        offsets[7] = 0x232E1D;
    }

    WriteProcessMemory(hProcess, (void*)(moduleBase + offsets[0]), &scale_general, 4, 0);
    WriteProcessMemory(hProcess, (void*)(moduleBase + offsets[1]), &scale_2d_x, 4, 0);
    WriteProcessMemory(hProcess, (void*)(moduleBase + offsets[2]), &scale_2d_y, 4, 0);
    WriteProcessMemory(hProcess, (void*)(moduleBase + offsets[3]), &guibackground_scale_y, 4, 0);

    WriteProcessMemory(hProcess, (void*)(moduleBase + offsets[4]), &view_range, 4, 0);
    if (gamever != VERSION_10)
    {
        WriteProcessMemory(hProcess, (void*)(moduleBase + offsets[5]), &view_range, 4, 0);
    }
    WriteProcessMemory(hProcess, (void*)(moduleBase + offsets[6]), &view_range, 4, 0);
    WriteProcessMemory(hProcess, (void*)(moduleBase + offsets[7]), &view_range, 4, 0);
    while (true)
    {

    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainTHREAD, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

