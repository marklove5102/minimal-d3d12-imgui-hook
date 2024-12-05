module;

#include "pch.h"
#include <map>
#include <Psapi.h>
#include <sigmatch/sigmatch.hpp>

export module API;

import Pattern;


export struct TypeDetails
{
    std::string Return;
    std::string Name;
    std::string Arguments;
    uint64_t Offset;
};

using sigmatch_literals::operator ""_sig;
#define FUNC_PATTERN(name, signature) name = Find<decltype(##name##)>(BaseAddress, context, signature##_sig); LOG_DEBUG("Found " #name " at {:#08x}", reinterpret_cast<uint64_t>(name) - BaseAddress)
#define FUNC_PATTERN_RELATIVE(name, signature, opCodeByteOffset) name = FindRelative<decltype(##name##)>(BaseAddress, context, signature##_sig, opCodeByteOffset); LOG_DEBUG("Found " #name " at {:#08x}", reinterpret_cast<uint64_t>(name) - BaseAddress)
#define VAR_PATTERN(name, signature) name = Find<decltype(##name##)>(BaseAddress, context, signature##_sig)); LOG_DEBUG("Found " #name " at {:#08x}", reinterpret_cast<uint64_t>(name) - BaseAddress)
#define VAR_PATTERN_ABSOULUTE(name, signature, opCodeByteOffset) name = FindAbsolute<decltype(##name##)>(BaseAddress, context, signature##_sig, opCodeByteOffset); LOG_DEBUG("Found " #name " at {:#08x}", reinterpret_cast<uint64_t>(name) - BaseAddress)
#define VAR_PATTERN_RELATIVE(name, signature, opCodeByteOffset) name = FindRelative<decltype(##name##)>(BaseAddress, context, signature##_sig, opCodeByteOffset); LOG_DEBUG("Found " #name " at {:#08x}", reinterpret_cast<uint64_t>(name) - BaseAddress)
#define OFFSET(name, offset) name = reinterpret_cast<decltype(name)>(reinterpret_cast<uint64_t>(name) + offset); LOG_DEBUG("Offset " #name " at {:#08x}", reinterpret_cast<uint64_t>(name) - BaseAddress)

#define FUNC_DEF(ret, name, args) typedef ret __fastcall name##_t##args##; export name##_t* name## = nullptr;
#define VAR_DEF(type, name) export type* name## = nullptr;

// Definitions go here.
// Example usage.
VAR_DEF(void, gpServerPacketCallbackTable);


export void InitializeAPI() {
    sigmatch::this_process_target target;
    HMODULE hModule = GetModuleHandle(nullptr);
    MODULEINFO mInfo = {};
    GetModuleInformation(GetCurrentProcess(), hModule, &mInfo, sizeof(MODULEINFO));
    sigmatch::search_context context = target.in_range({ mInfo.lpBaseOfDll, mInfo.SizeOfImage });

    auto BaseAddress = reinterpret_cast<uint64_t>(mInfo.lpBaseOfDll);
    LOG_DEBUG("Base Address: {:#08x}, SizeOfImage: ", BaseAddress, mInfo.SizeOfImage);
    // Example usage.
    //VAR_PATTERN_RELATIVE(gpServerPacketCallbackTable, "48 8D 0D ?? ?? ?? ?? 48 83 3C C1 00 75 ?? 48 8D 44 24 ?? 48 8B F8 33 C0 B9 01 00 00 00 F3 AA 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 0F B6 C0 85 C0 74 ?? CC 33 C0 0F B6 44 24", 3);
}

export void ApplyPatches()
{
    sigmatch::this_process_target target;
    HMODULE hModule = GetModuleHandle(nullptr);
    MODULEINFO mInfo = {};
    GetModuleInformation(GetCurrentProcess(), hModule, &mInfo, sizeof(MODULEINFO));
    sigmatch::search_context context = target.in_range({ mInfo.lpBaseOfDll, mInfo.SizeOfImage });

    auto BaseAddress = reinterpret_cast<uint64_t>(mInfo.lpBaseOfDll);

}

export void RemovePatches()
{
    sigmatch::this_process_target target;
    HMODULE hModule = GetModuleHandle(nullptr);
    MODULEINFO mInfo = {};
    GetModuleInformation(GetCurrentProcess(), hModule, &mInfo, sizeof(MODULEINFO));
    sigmatch::search_context context = target.in_range({ mInfo.lpBaseOfDll, mInfo.SizeOfImage });

    auto BaseAddress = reinterpret_cast<uint64_t>(mInfo.lpBaseOfDll);

}

