#include <GarrysMod/Lua/LuaBase.h>
#include <GarrysMod/Lua/Interface.h>
#include <MinHook.h>
#include <iostream>
#include <GarrysMod/Lua/Helpers.hpp>
typedef void (*ConMsg_t)(PRINTF_FORMAT_STRING const tchar* pMsg, ...);
ConMsg_t original_ConMsg = nullptr;
GarrysMod::Lua::ILuaBase* gLua = nullptr;

std::string GetFormatWithList(const char* fmt, va_list ap) {
    std::string message;
    va_list ap_copy;
    va_copy(ap_copy, ap);
    size_t len = vsnprintf(0, 0, fmt, ap_copy);
    message.resize(len + 1);  
    vsnprintf(&message[0], len + 1, fmt, ap);
    message.resize(len);  
    return message;
}

void hk_ConMsg(const char* format, ...) {

    va_list va;
	va_start(va, format);
    std::string message = GetFormatWithList(format, va);
    va_end(va);
    bool call = true;
    LuaHelpers::PushHookRun(gLua, "OnConMsgCalled");
    gLua->PushString(message.c_str());
    if (LuaHelpers::CallHookRun(gLua, 1, 1))
        call = !gLua->IsType(-1, GarrysMod::Lua::Type::BOOL) || gLua->GetBool(-1);
    gLua->Pop(gLua->Top());
    if (call)
        original_ConMsg(message.c_str());
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
