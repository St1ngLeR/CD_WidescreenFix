#include <Windows.h>
#include <vector>
#include <algorithm>
#include <set>
#include <string>

#include "injector/injector.hpp"
#include "iniReader.h"

struct DisplayMode
{
    float width;
    float height;
    int32_t colorDepth;
    float refreshRate;

    bool operator<(const DisplayMode& other) const
	{
        if (width != other.width) return width < other.width;
        if (height != other.height) return height < other.height;
        if (colorDepth != other.colorDepth) return colorDepth < other.colorDepth;
        return refreshRate < other.refreshRate;
    }
};

void GetCurrentRefreshRateDisplayModes()
{
    DEVMODE currentMode;
    ZeroMemory(&currentMode, sizeof(currentMode));
    currentMode.dmSize = sizeof(currentMode);
    
    if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &currentMode))
	{
        injector::WriteMemory<int32_t>(0x7CD64C, 0);
        return;
    }
    
    int currentRefreshRate = currentMode.dmDisplayFrequency;
    std::set<DisplayMode> modesSet;
    
    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
    
    for (int i = 0; EnumDisplaySettings(NULL, i, &dm); i++)
	{
        if (dm.dmDisplayFrequency == currentRefreshRate)
		{
            DisplayMode mode;
            mode.width = static_cast<float>(dm.dmPelsWidth);
            mode.height = static_cast<float>(dm.dmPelsHeight);
            mode.colorDepth = dm.dmBitsPerPel;
            mode.refreshRate = static_cast<float>(dm.dmDisplayFrequency);
            modesSet.insert(mode);
        }
        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);
    }

    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
        L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e96e-e325-11ce-bfc1-08002be10318}\\0000\\Modes",
        0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{  
        DWORD index = 0;
        wchar_t valueName[256];
        DWORD valueNameSize = sizeof(valueName) / sizeof(wchar_t);
        BYTE data[256];
        DWORD dataSize = sizeof(data);
        
        while (RegEnumValueW(hKey, index, valueName, &valueNameSize, NULL, NULL, data, &dataSize) == ERROR_SUCCESS)
		{
            std::wstring modeStr(valueName);
            
            size_t firstX = modeStr.find(L'x');
            size_t secondX = modeStr.find(L'x', firstX + 1);
            size_t spacePos = modeStr.find(L' ');
            
            if (firstX != std::wstring::npos && secondX != std::wstring::npos && spacePos != std::wstring::npos)
			{
                int width = _wtoi(modeStr.substr(0, firstX).c_str());
                int height = _wtoi(modeStr.substr(firstX + 1, secondX - firstX - 1).c_str());
                int colorDepth = _wtoi(modeStr.substr(secondX + 1, spacePos - secondX - 1).c_str());
                int refreshRate = _wtoi(modeStr.substr(spacePos + 1).c_str());
                
                if (refreshRate == currentRefreshRate)
				{
                    DisplayMode mode;
                    mode.width = static_cast<float>(width);
                    mode.height = static_cast<float>(height);
                    mode.colorDepth = colorDepth;
                    mode.refreshRate = static_cast<float>(refreshRate);
                    modesSet.insert(mode);
                }
            }
            
            index++;
            valueNameSize = sizeof(valueName) / sizeof(wchar_t);
            dataSize = sizeof(data);
        }
        
        RegCloseKey(hKey);
    }

    for (int subKeyIndex = 0; subKeyIndex < 10; subKeyIndex++)
	{
        wchar_t subKeyPath[512];
        swprintf_s(subKeyPath, L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e96e-e325-11ce-bfc1-08002be10318}\\%04d\\Modes", subKeyIndex);
        
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, subKeyPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
            DWORD index = 0;
            wchar_t valueName[256];
            DWORD valueNameSize = sizeof(valueName) / sizeof(wchar_t);
            BYTE data[256];
            DWORD dataSize = sizeof(data);
            
            while (RegEnumValueW(hKey, index, valueName, &valueNameSize, NULL, NULL, data, &dataSize) == ERROR_SUCCESS)
			{
                std::wstring modeStr(valueName);
                
                size_t firstX = modeStr.find(L'x');
                size_t secondX = modeStr.find(L'x', firstX + 1);
                size_t spacePos = modeStr.find(L' ');
                
                if (firstX != std::wstring::npos && secondX != std::wstring::npos && spacePos != std::wstring::npos)
				{
                    int width = _wtoi(modeStr.substr(0, firstX).c_str());
                    int height = _wtoi(modeStr.substr(firstX + 1, secondX - firstX - 1).c_str());
                    int colorDepth = _wtoi(modeStr.substr(secondX + 1, spacePos - secondX - 1).c_str());
                    int refreshRate = _wtoi(modeStr.substr(spacePos + 1).c_str());
                    
                    if (refreshRate == currentRefreshRate)
					{
                        DisplayMode mode;
                        mode.width = static_cast<float>(width);
                        mode.height = static_cast<float>(height);
                        mode.colorDepth = colorDepth;
                        mode.refreshRate = static_cast<float>(refreshRate);
                        modesSet.insert(mode);
                    }
                }
                
                index++;
                valueNameSize = sizeof(valueName) / sizeof(wchar_t);
                dataSize = sizeof(data);
            }
            
            RegCloseKey(hKey);
        }
    }

    std::vector<DisplayMode> modes(modesSet.begin(), modesSet.end());

    uintptr_t address = 0x7CD64C;
	injector::WriteMemory<int32_t>(address, static_cast<int32_t>(modes.size()));
    address += sizeof(int32_t);

    for (const auto& mode : modes)
	{
		injector::WriteMemory<float>(address, mode.width);
        address += sizeof(float);
        
		injector::WriteMemory<float>(address, mode.height);
        address += sizeof(float);
        
		injector::WriteMemory<int32_t>(address, mode.colorDepth);
        address += sizeof(int32_t);
        
		injector::WriteMemory<int32_t>(address, 0);
        address += sizeof(int32_t);
        
		injector::WriteMemory<float>(address, 1.0f);
        address += sizeof(float);
        
		injector::WriteMemory<float>(address, 1.0f);
        address += sizeof(float);
        
		injector::WriteMemory<float>(address, mode.refreshRate);
        address += sizeof(float);
        
		injector::WriteMemory<int32_t>(address, 0);
        address += sizeof(int32_t);
    }
}

std::string confpath = std::filesystem::current_path().string() + "\\CDWidescreenFix.ini";
CIniReader patchconf(confpath);

float AspectWidth;
float AspectHeight;

float scale_general;
float scale_2d_x;
float scale_2d_y;
float guibackground_scale_y;
float bgcredits;

int window_x;
int window_y;

float msg_x;
float msg_y;

double trkpic_x;

float light1;
double light2;

int gcd(int a, int b)
{
	int result = min(a, b);
	while (result > 0)
	{
		if (a % result == 0 && b % result == 0)
		{
			break;
		}
		result--;
	}
	return result;
}

bool ParseAspectRatio(const std::string& ratio, int& width, int& height)
{
	std::istringstream iss(ratio);
	char colon;

	if (iss >> width >> colon >> height && colon == ':')
	{
		return true;
	}
	return false;
}

std::string ForceAspectRatio = patchconf.ReadString("General", "ForceAspectRatio", "auto");

bool FixHUDMessagesScale = patchconf.ReadBoolean("General", "FixHUDMessagesScale", true);
bool FixLightFlaresScale = patchconf.ReadBoolean("General", "FixLightFlaresScale", true);

float HUDMessagesScale = patchconf.ReadFloat("General", "HUDMessagesScale", 1.1f);
float LightFlaresScale = patchconf.ReadFloat("General", "LightFlaresScale", 1.1f);

const char* WSF_Section_Main = "WidescreenFix.Main";
const char* ScreenFormatAuto_Key = "SCREENFORMATAUTO";

const char* RaceFormat_Auto_ID = "auto";
const char* RaceFormat_16_9_ID = "16:9";
const char* RaceFormat_16_10_ID = "16:10";
const char* RaceFormat_4_3_ID = "4:3";
const char* RaceFormat_5_4_ID = "5:4";

void __declspec(naked) a_ScreenFormatSwitch()
{
	__asm
	{
		call sub_4B2790

		mov ebx, -1
		mov edx, 0x6D00D6
		lea eax, [esp + 0x6F0]
		call sub_69586C
		mov ebx, dword ptr [ScreenFormatAuto_Key]
		mov edx, dword ptr [WSF_Section_Main]
		mov ecx, eax
		mov eax, ds: [0x7E3110]
		call sub_66F410
		mov ebx, -1
		mov edx, eax
		lea eax, [esp + 0x6B0]
		call sub_69586C
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_Auto_ID]
		mov esi, eax
		lea eax, [esp + 0x690]
		call sub_69586C
		mov	edi, ds: [0x7962E8]
		mov ebx, esi
		mov edx, eax
		mov eax, edi
		call sub_4B3460
		lea eax, [esp + 0x690]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6B0]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6F0]
		xor edx, edx
		call sub_6959C9
			
		mov ebx, -1
		mov edx, 0x6D00D6
		lea eax, [esp + 0x6F0]
		call sub_69586C
		mov ecx, eax
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_16_9_ID]
		lea eax, [esp + 0x6B0]
		call sub_69586C
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_16_9_ID]
		mov esi, eax
		lea eax, [esp + 0x690]
		call sub_69586C
		mov	edi, ds: [0x7962E8]
		mov ebx, esi
		mov edx, eax
		mov eax, edi
		call sub_4B3460
		lea eax, [esp + 0x690]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6B0]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6F0]
		xor edx, edx
		call sub_6959C9

		mov ebx, -1
		mov edx, 0x6D00D6
		lea eax, [esp + 0x6F0]
		call sub_69586C
		mov ecx, eax
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_16_10_ID]
		lea eax, [esp + 0x6B0]
		call sub_69586C
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_16_10_ID]
		mov esi, eax
		lea eax, [esp + 0x690]
		call sub_69586C
		mov	edi, ds: [0x7962E8]
		mov ebx, esi
		mov edx, eax
		mov eax, edi
		call sub_4B3460
		lea eax, [esp + 0x690]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6B0]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6F0]
		xor edx, edx
		call sub_6959C9

		mov ebx, -1
		mov edx, 0x6D00D6
		lea eax, [esp + 0x6F0]
		call sub_69586C
		mov ecx, eax
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_4_3_ID]
		lea eax, [esp + 0x6B0]
		call sub_69586C
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_4_3_ID]
		mov esi, eax
		lea eax, [esp + 0x690]
		call sub_69586C
		mov	edi, ds: [0x7962E8]
		mov ebx, esi
		mov edx, eax
		mov eax, edi
		call sub_4B3460
		lea eax, [esp + 0x690]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6B0]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6F0]
		xor edx, edx
		call sub_6959C9

		mov ebx, -1
		mov edx, 0x6D00D6
		lea eax, [esp + 0x6F0]
		call sub_69586C
		mov ecx, eax
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_5_4_ID]
		lea eax, [esp + 0x6B0]
		call sub_69586C
		mov ebx, -1
		mov edx, dword ptr [RaceFormat_5_4_ID]
		mov esi, eax
		lea eax, [esp + 0x690]
		call sub_69586C
		mov	edi, ds: [0x7962E8]
		mov ebx, esi
		mov edx, eax
		mov eax, edi
		call sub_4B3460
		lea eax, [esp + 0x690]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6B0]
		xor edx, edx
		call sub_6959C9
		lea eax, [esp + 0x6F0]
		xor edx, edx
		call sub_6959C9

		jmp ScreenFormatDetect

	ScreenFormatDetect:
		mov ebx, -1
		lea edx, ForceAspectRatio
		lea eax, [esp+0x530]
		call sub_69586C
		mov ecx, ds: [0x7962E8]
		mov edx,eax
		mov eax,ecx
		call sub_4B37B0
		lea eax, [esp+0x530]
		jmp loc_540E83

	loc_540E83:
		push 0x540E83
		retn

	sub_4B37B0:
		push 0x4B37B0
		retn

	loc_540E45:
		push 0x540E45
		retn

	sub_66F410:
		push 0x66F410
		retn

	sub_6959C9:
		push 0x6959C9
		retn

	sub_4B3460:
		push 0x4B3460
		retn

	sub_69586C:
		push 0x69586C
		retn

	sub_4B2790:
		push 0x4B2790
		retn
	}
}

void SetAspectRatio(const char* value)
{
	patchconf.WriteString("General", "ForceAspectRatio", value);
	ForceAspectRatio = value;
}

void __declspec(naked) a_ScreenFormatSet()
{
	__asm
	{
		mov edx, esp
		mov eax, ds: [0x7962E8]
		call sub_4B4050
		call sub_695B53
		push eax
		call SetAspectRatio
		add esp, 4
		jmp loc_541590

	sub_6959C9:
		push 0x6959C9
		retn

	sub_695B53:
		push 0x695B53
		retn

	sub_63CB30:
		push 0x63CB30
		retn

	sub_69586C:
		push 0x69586C
		retn 

	loc_541590:
		push 0x541590
		retn

	sub_696082:
		push 0x696082
		retn

	sub_4B4050:
		push 0x4B4050
		retn
	}
}

float text_scale_x;

void __declspec(naked) a_TextFix()
{
	__asm
	{
		mov eax, [eax + 0x84]
		fmul dword ptr [text_scale_x]
		
		jmp loc_5F2261

	loc_5F2261:
		push 0x5F2261
		retn
	}
}

void __declspec(naked) a_EnumScreenResolutionFix()
{
	__asm
	{
		call GetCurrentRefreshRateDisplayModes
		jmp loc_5EA1F6

	loc_5EA1F6:
		push 0x5EA1F6
		retn
	}
}

DWORD WINAPI MainTHREAD(LPVOID)
{
	injector::MakeJMP(0x540D1C, a_ScreenFormatSwitch, true);
	injector::WriteMemory(0x540D0E, 5, true);

	injector::MakeJMP(0x54151D, a_ScreenFormatSet, true);
	//injector::MakeJMP(0x5F2258, a_TextFix, true);
	injector::MakeJMP(0x5EA13D, a_EnumScreenResolutionFix, true);

	injector::WriteMemory<int>(0x5B5FAC, 0x9ED905D9, true);
	injector::WriteMemory<short>(0x5B5FB0, 0x006D, true);
	injector::MakeNOP(0x5B5FB2, 3, true);

	while (true)
	{
		if (*(DWORD*)0x7CF6FC)
		{
			injector::MakeNOP(0x492134, 2, true);
			injector::MakeNOP(0x49213D, 2, true);
			injector::MakeNOP(0x492146, 2, true);
			injector::MakeNOP(0x49214F, 2, true);

			if (*(DWORD*)0x7DC3F4 == 0)
			{
				if (ForceAspectRatio == "auto")
				{
					window_x = injector::ReadMemory<int>(injector::ReadMemory<DWORD>(0x7CF6FC) + 0x250, true);
					window_y = injector::ReadMemory<int>(injector::ReadMemory<DWORD>(0x7CF6FC) + 0x254, true);
				}
				else
				{
					ParseAspectRatio(ForceAspectRatio, window_x, window_y);
				}

				if ((window_x != 0) && (window_y != 0))
				{
					AspectWidth = window_x / gcd(window_x, window_y);
					AspectHeight = window_y / gcd(window_x, window_y);

					scale_general = (AspectWidth / AspectHeight / 1.1875f);
					scale_2d_x = ((AspectWidth / AspectHeight / 1.333333333333333f / 1.1875f) / 480.f / 1.333333333333333f) / (AspectWidth / AspectHeight / 1.777777777777777f);
					//scale_2d_y = (640.0f * 1.1875f / (AspectWidth / AspectHeight / 1.333333333333333f)) * (AspectWidth / AspectHeight / 1.777777777777777f);
					bgcredits = (0.5f / (AspectWidth / AspectHeight)) * 2.f;
					text_scale_x = ((float)window_y / 480.f);

					injector::WriteMemory(0x5FBF01, scale_general, true);
					injector::WriteMemory(0x6ED584, scale_2d_x, true);
					injector::WriteMemory(0x6ED58C, 570.f, true);
					injector::WriteMemory(0x6BABAD, guibackground_scale_y, true);
					injector::WriteMemory(0x7001D9, bgcredits, true);

					trkpic_x = 110.f * ((float)window_y / (float)window_x);
					injector::WriteMemory(0x6C485C, trkpic_x, true);

					if (FixHUDMessagesScale)
					{
						msg_x = 0.190476f * ((float)window_y / (float)window_x);
						msg_y = 0.0416666666666667f * ((float)window_y / (float)window_x);

						injector::WriteMemory(0x6E3D4A, msg_x * HUDMessagesScale, true);
						injector::WriteMemory(0x6E3D4E, msg_y * HUDMessagesScale, true);
					}

					if (FixLightFlaresScale)
					{
						light1 = 266.66666666f * ((float)window_y / (float)window_x);
						light2 = 0.88f * ((float)window_y / (float)window_x);

						injector::WriteMemory(0x6EFF0E, light1 * LightFlaresScale, true);
						injector::WriteMemory(0x6EFF5A, light2 * LightFlaresScale, true);
					}
				}
			}
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (*(DWORD*)0x4000A8 == 0x29BB90)	// Checking the entry point of 1.2 executable
		{
			CreateThread(0, 0, MainTHREAD, 0, 0, 0);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}
