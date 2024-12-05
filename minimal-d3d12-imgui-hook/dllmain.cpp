#include "pch.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <filesystem>
#include <stacktrace>

import Globals;
import API;
import D3D12Hook;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
DWORD WINAPI AttachThread(LPVOID lParam);
LONG WINAPI ExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo);
void InitializeLogging();
DWORD WINAPI CreateConsole();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        char szPath[MAX_PATH];
        GetModuleFileNameA(hModule, szPath, MAX_PATH);
        DLL_PATH = std::filesystem::absolute(szPath).parent_path();
#ifndef NDEBUG
        CreateConsole();
#endif
        InitializeLogging();
        SetUnhandledExceptionFilter(ExceptionHandler);
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, &AttachThread, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        D3D12::RemoveHooks();
        break;
    default:
        break;
    }
    return TRUE;
}

DWORD WINAPI AttachThread(LPVOID lParam) {
    if (D3D12::Init() == D3D12::Status::Success) {
        D3D12::InstallHooks();
    }
    return 0;
}

LONG WINAPI ExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
    std::stringstream ss;
    ss << std::endl;
    auto trace = std::stacktrace::current();
    for (const auto& frame : trace) {
        ss << frame << std::endl;
    }
    const auto message = ss.str();
    LOG_ERROR("{}", message);
    //system("pause");
    exit(EXIT_FAILURE);
    return EXCEPTION_EXECUTE_HANDLER;
}

void InitializeLogging() {
#if SPDLOG_ACTIVE_LEVEL != SPDLOG_LEVEL_OFF
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>((DLL_PATH / "output.txt").string(), true);
#ifndef NDEBUG
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    spdlog::sinks_init_list sink_list = { file_sink, console_sink };
#else
    spdlog::sinks_init_list sink_list = { file_sink };
#endif
    spdlog::logger logger("log", sink_list.begin(), sink_list.end());
    spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));

    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_level(spdlog::level::trace);
    LOG_INFO("Logger initialized");
#endif
}

DWORD WINAPI CreateConsole() {
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
    return 0;
}
