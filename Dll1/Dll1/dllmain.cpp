#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

#include <d3d9.h>
#include <d3dx9.h>

#include <Dwmapi.h>

#include <windows.h>
#include <stdio.h>

LPDIRECT3DDEVICE9 d3ddev;
uintptr_t programm;
D3DRECT rec = { 10, 10, 120, 30 };
RECT fontRect = { 10, 15, 120, 120 };
D3DCOLOR bkgColor = D3DCOLOR_XRGB(164, 39, 227);
D3DCOLOR fontColor = D3DCOLOR_XRGB(255, 255, 255);
ID3DXFont* m_font = 0;

void GetDirectDevise() 
{
    programm = (uintptr_t)GetModuleHandle("Direct9.exe");
    d3ddev = *(LPDIRECT3DDEVICE9*)(programm + 0x3384);
}

#pragma pack(push, 1)
struct Data
{
public:
    Data(uintptr_t opcode) {
        opcode_func = opcode;
    }
private:
    byte call = 0xE8;
    uintptr_t opcode_func;
};
#pragma pack(pop)

void Hook() 
{
    d3ddev->Clear(1, &rec, D3DCLEAR_TARGET, bkgColor, 1.0f, 0);
    d3ddev->BeginScene();
    if (m_font == NULL) {
        D3DXCreateFont(d3ddev, 12, 0, FW_BOLD, 0, 0, 1, 0, 0, 0 | FF_DONTCARE, TEXT("Arial"), &m_font);
    }
    m_font->DrawText(0, "YES! IZI HOOK :-)", -1, &fontRect, 0, fontColor);
    d3ddev->EndScene();
    d3ddev->Present(0, 0, 0, 0);
} 

void Patch() {
    uintptr_t presentAdr = programm + 0x120B;
    uintptr_t hook_adr = (uintptr_t)Hook;
    uintptr_t hook_opcode = hook_adr - presentAdr - 5;
    Data data(hook_opcode);
    DWORD old_protect;
    VirtualProtect((void*)presentAdr, 19, PAGE_EXECUTE_READWRITE, &old_protect);
    *(Data*)(presentAdr) = data;
    memset((void*)(presentAdr + 5), 0x90, 9);
    VirtualProtect((void*)presentAdr, 19, old_protect, &old_protect);
}

DWORD WINAPI Main(HMODULE hModule) {

    GetDirectDevise();
    Patch();
    return 0; 
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

