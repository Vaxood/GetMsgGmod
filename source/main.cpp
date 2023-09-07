#include <GarrysMod/Lua/LuaBase.h>
#include <GarrysMod/Lua/Interface.h>
#include <MinHook.h>
#include <iostream>
#include <GarrysMod/Lua/Helpers.hpp>

typedef void (*ConMsg_t)(PRINTF_FORMAT_STRING const tchar* pMsg, ...);
ConMsg_t original_ConMsg = nullptr;
GarrysMod::Lua::ILuaBase* gLua = nullptr;
void hk_ConMsg(PRINTF_FORMAT_STRING const tchar* pMsg, ...)
{

    va_list args;
    va_start(args, pMsg);
    bool call = true;
   
    LuaHelpers::PushHookRun(gLua, "OnConMsgCalled");
    gLua->PushString(pMsg);
    if (LuaHelpers::CallHookRun(gLua, 1, 1))
        call = !gLua->IsType(-1, GarrysMod::Lua::Type::BOOL) || gLua->GetBool(-1);
     gLua->Pop(gLua->Top());
    if (call)
    original_ConMsg(pMsg, args);
    va_end(args);
}



void Init()
{
    MH_Initialize();
    void (*pConMsg)(const char* format, ...) = &ConMsg;
    MH_CreateHook(reinterpret_cast<void*>(pConMsg), reinterpret_cast<void*>(&hk_ConMsg), reinterpret_cast<void**>(&original_ConMsg));
    MH_EnableHook(MH_ALL_HOOKS);
}








GMOD_MODULE_OPEN()
{
    Init();
    gLua = LUA;


    return 0;
}

GMOD_MODULE_CLOSE()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	return 0;
}
