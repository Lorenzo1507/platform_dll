// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"

/*We need to go to project settings, go to advanced and then set in the c++/cli properties
common language runtime support (/clr)*/

/*It is also necessary to add in the C/C++ command line also in the project settings
/ZctwoPhase- */
/*
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
*/
