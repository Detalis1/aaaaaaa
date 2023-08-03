#include <Windows.h>
#include <Psapi.h>
#include <string>

#define printf(a) MessageBoxA(0, a, "", 0)


void* GetEntryPoint(LPCSTR fileName)
{
  // LPCSTR fileName; //exe file to parse
    HANDLE hFile;
    HANDLE hFileMapping;
    LPVOID lpFileBase;
    PIMAGE_DOS_HEADER dosHeader;
    PIMAGE_NT_HEADERS peHeader;

    hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("\n CreateFile failed in read mode \n");
        return nullptr;
    }

    hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

    if (hFileMapping == 0)
    {
        printf("\n CreateFileMapping failed \n");
        CloseHandle(hFile);
        return nullptr;
    }

    lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);

    if (lpFileBase == 0)
    {
        printf("\n MapViewOfFile failed \n");
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return nullptr;
    }

    dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;  //pointer to dos headers

    if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE)
    {
        //if it is executable file print different fileds of structure
        //dosHeader->e_lfanew : RVA for PE Header
        printf("\n DOS Signature (MZ) Matched");

        //pointer to PE/NT header
        peHeader = (PIMAGE_NT_HEADERS)((u_char*)dosHeader + dosHeader->e_lfanew);

        if (peHeader->Signature == IMAGE_NT_SIGNATURE)
        {
            printf("\n PE Signature (PE) Matched \n");

            //address of entry point
            return (void*)peHeader->OptionalHeader.AddressOfEntryPoint;

        }
        UnmapViewOfFile(lpFileBase);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return 0;
    }
    else
    {
        printf("\n DOS Signature (MZ) Not Matched \n");
        UnmapViewOfFile(lpFileBase);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return nullptr;
    }
}





__int32 __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    DWORD oldProtect = NULL;
    STARTUPINFO si;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    void* entryPoint = GetEntryPoint("CMLauncher.exe");
    if(entryPoint == nullptr)
    {
        printf("Entry point nof finded!");
        return 0;
    }
    printf(std::to_string((DWORD)entryPoint).c_str());

    byte origBytes[2];
    printf("Go");
    if (CreateProcessW(L"CMLauncher.exe", nullptr, 0, 0, 0, CREATE_SUSPENDED, 0, 0, &si, &pi))
    {
        //loop ep
        //
     //   ReadProcessMemory(pi.hProcess, entryPoint, origBytes, 2, NULL);

        unsigned __int8 epbytes[2] = { 0xeb, 0xfe };
        VirtualProtectEx(pi.hProcess, entryPoint, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
        WriteProcessMemory(pi.hProcess, entryPoint, epbytes, sizeof(epbytes), nullptr);
        VirtualProtectEx(pi.hProcess, entryPoint, 2, oldProtect, &oldProtect);
        ResumeThread(pi.hThread);
        //

        //wait for ep
        //
        CONTEXT context;
        context.ContextFlags = CONTEXT_CONTROL;
        do
        {
            GetThreadContext(pi.hThread, &context);
        } while (context.Eip != (DWORD)entryPoint);
        //

        //inject dll
        //
      //  тут код его инжектора
      //

     //restore ep
            //
        SuspendThread(pi.hThread);

        VirtualProtectEx(pi.hProcess, entryPoint, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
        WriteProcessMemory(pi.hProcess, entryPoint, origBytes, 2, nullptr);
        VirtualProtectEx(pi.hProcess, entryPoint, 2, oldProtect, &oldProtect);

        ResumeThread(pi.hThread);
        //
    }
}
