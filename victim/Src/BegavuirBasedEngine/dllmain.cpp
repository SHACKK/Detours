#include "pch.h"

static LONG dwSlept = 0;

static VOID(WINAPI* True_Sleep)(DWORD dwMilliseconds) = Sleep;

VOID WINAPI Hooked_Sleep(DWORD dwMilliseconds)
{
    printf("Let's say slept %us(ms).\n", dwMilliseconds);
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)True_Sleep, Hooked_Sleep);
        DetourTransactionCommit();
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)True_Sleep, Hooked_Sleep);
        DetourTransactionCommit();
    }
    return TRUE;
}