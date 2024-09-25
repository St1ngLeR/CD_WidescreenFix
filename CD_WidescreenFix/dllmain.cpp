#include "pch.h"
#include "ProcessMem.cpp"
#include <filesystem>

using namespace std;

ProcessMem PM;

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

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AspectWidth = GetSystemMetrics(SM_CXSCREEN) / gcd(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
        AspectHeight = GetSystemMetrics(SM_CYSCREEN) / gcd(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

        scale_general = (AspectWidth / AspectHeight / 1.1875f);
        scale_2d_x = ((AspectWidth / AspectHeight / 1.333333333333333f / 1.1875f) * 0.00156250002328306f) / (AspectWidth / AspectHeight / 1.777777777777777f);
        scale_2d_y = (640.0f * 1.1875f / (AspectWidth / AspectHeight / 1.333333333333333f)) * (AspectWidth / AspectHeight / 1.777777777777777f);
        guibackground_scale_y = 0.5f / (AspectWidth / AspectHeight);

        WriteProcessMemory(hProcess, (void*)(moduleBase + 0x1FBF01), &scale_general, 4, 0);
        WriteProcessMemory(hProcess, (void*)(moduleBase + 0x2ED584), &scale_2d_x, 4, 0);
        WriteProcessMemory(hProcess, (void*)(moduleBase + 0x2ED58C), &scale_2d_y, 4, 0);
        WriteProcessMemory(hProcess, (void*)(moduleBase + 0x2BABAD), &guibackground_scale_y, 4, 0);

        WriteProcessMemory(hProcess, (void*)(moduleBase + 0x13F4B5), &view_range, 4, 0);
        WriteProcessMemory(hProcess, (void*)(moduleBase + 0x149E08), &view_range, 4, 0);
        WriteProcessMemory(hProcess, (void*)(moduleBase + 0x2D00B8), &view_range, 4, 0);
        WriteProcessMemory(hProcess, (void*)(moduleBase + 0x232E1D), &view_range, 4, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

