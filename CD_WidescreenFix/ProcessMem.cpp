#include <math.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include <tchar.h>

using namespace std;

class ProcessMem
{
public:
    string GetProcessPathByPID(DWORD processId) {
        // �������� ����� ��������
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if (hProcess == NULL) {
            return ""; // ���������� ������ ������ � ������ ������
        }

        // �������� ������ ��� �������� ����
        TCHAR path[MAX_PATH] = { 0 };
        DWORD pathLength = MAX_PATH;

        // �������� ���� � ������������ ����� ��������
        if (QueryFullProcessImageName(hProcess, 0, path, &pathLength)) {
            // ����������� ���� � ������
            string processPath = path;
            return processPath;
        }
        else {
            CloseHandle(hProcess);
            return ""; // ���������� ������ ������ � ������ ������
        }
    }

    DWORD GetProcId(const char* procName)
    {
        DWORD procId = 0;
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
                        procId = procEntry.th32ProcessID;
                        break;
                    }
                } while (Process32Next(hSnap, &procEntry));

            }
        }
        CloseHandle(hSnap);
        return procId;
    }

    uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
    {
        uintptr_t modBaseAddr = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
        if (hSnap != INVALID_HANDLE_VALUE)
        {
            MODULEENTRY32 modEntry;
            modEntry.dwSize = sizeof(modEntry);
            if (Module32First(hSnap, &modEntry))
            {
                do
                {
                    if (!_stricmp(modEntry.szModule, modName))
                    {
                        modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                        break;
                    }
                } while (Module32Next(hSnap, &modEntry));
            }
        }
        CloseHandle(hSnap);
        return modBaseAddr;
    }

    bool CompareFloat(float x, float y, float epsilon = 0.01f) {
        if (fabs(x - y) < epsilon)
            return true; //they are same
        return false; //they are not same
    }

    uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
    {
        uintptr_t addr = ptr;
        for (unsigned int i = 0; i < offsets.size(); ++i)
        {
            ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
            addr += offsets[i];
        }
        return addr;
    }

};