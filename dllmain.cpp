#include <Windows.h>
#include "injector/injector.hpp"
#include "ini.h"

ini::File patchconf = ini::open(std::filesystem::current_path().string() + "\\CDWidescreenFix.ini");

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

bool ParseAspectRatio(const std::string& ratio, int& width, int& height) {
	std::istringstream iss(ratio);
	char colon;

	if (iss >> width >> colon >> height && colon == ':') {
		return true;
	}
	return false;
}

DWORD WINAPI MainTHREAD(LPVOID)
{
	while (true)
	{
		std::string ForceAspectRatio = patchconf["General"].get<std::string>("ForceAspectRatio");
		bool FixHUDMessagesScale = patchconf["General"].get<int>("FixHUDMessagesScale");
		bool FixLightFlaresScale = patchconf["General"].get<int>("FixLightFlaresScale");

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

					injector::WriteMemory(0x6E3D4A, msg_x, true);
					injector::WriteMemory(0x6E3D4E, msg_y, true);
				}

				if (FixLightFlaresScale)
				{
					light1 = 266.66666666f * ((float)window_y / (float)window_x);
					light2 = 0.88f * ((float)window_y / (float)window_x);

					injector::WriteMemory(0x6EFF0E, light1, true);
					injector::WriteMemory(0x6EFF5A, light2, true);
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
