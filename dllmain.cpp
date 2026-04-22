#include <Windows.h>
#include "injector/injector.hpp"
#include "iniReader.h"

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

DWORD WINAPI MainTHREAD(LPVOID)
{
	injector::MakeJMP(0x540D1C, a_ScreenFormatSwitch, true);
	injector::WriteMemory(0x540D0E, 5, true);

	injector::MakeJMP(0x54151D, a_ScreenFormatSet, true);

	while (true)
	{
		if (*(DWORD*)0x7CF6FC)
		{
			injector::MakeNOP(0x492134, 2, true);
			injector::MakeNOP(0x49213D, 2, true);
			injector::MakeNOP(0x492146, 2, true);
			injector::MakeNOP(0x49214F, 2, true);

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
				scale_2d_x = ((AspectWidth / AspectHeight / 1.333333333333333f / 1.1875f) * 0.00156250002328306f) / (AspectWidth / AspectHeight / 1.777777777777777f);
				scale_2d_y = (640.0f * 1.1875f / (AspectWidth / AspectHeight / 1.333333333333333f)) * (AspectWidth / AspectHeight / 1.777777777777777f);
				bgcredits = (0.5f / (AspectWidth / AspectHeight)) * 2.f;

				injector::WriteMemory(0x5FBF01, scale_general, true);
				injector::WriteMemory(0x6ED584, scale_2d_x, true);
				injector::WriteMemory(0x6ED58C, scale_2d_y, true);
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
