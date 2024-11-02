#define VERSION_10 0x00010000
#define VERSION_11 0x00010001
#define VERSION_12 0x00010002

using namespace std;

bool test = false;

class VerDef
{
public:
	int Init(HANDLE hProcess, uintptr_t moduleBase)
	{
		int gamever;
		int gamever_check;
		bool defined = false;

		int available_vers[] = { VERSION_10, VERSION_11, VERSION_12 };
		int available_offs[] = { 0x857E6, 0x85C71, 0x85C86 };

		for (int i = 0; i < size(available_vers); i++)
		{
			if (defined == false)
			{
				ReadProcessMemory(hProcess, (void*)(moduleBase + available_offs[i]), &gamever_check, 8, 0);
				if (test == true)
				{
					printf("%d / %d\n", i + 1, size(available_vers));
					printf("Offset - %#08x\n", available_offs[i]);
					printf("Game version - %#08x\n", gamever_check);
				}
				if (gamever_check == available_vers[i])
				{
					gamever = gamever_check;
					defined = true;
				}
			}
		}
		if (test == true)
		{
			printf("Current game version - %#08x\n", gamever);
		}
		return gamever;
	}

	void Test(HANDLE hProcess, uintptr_t moduleBase)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		test = true;
		Init(hProcess, moduleBase);
	}
};