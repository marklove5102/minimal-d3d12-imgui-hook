// not a real pch because msvc won't allow use w/ modules. but common includes/defines are here.
#pragma once

// Windows + std
#ifndef NOMINMAX
#define NOMINMAX
#endif

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <iostream>

// ImGui
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// SPDLOG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>
// Makes it so if you replace spdlog you can replace the defines to new log system w/o having to touch other code.
#define LOG_TRACE SPDLOG_TRACE
#define LOG_DEBUG SPDLOG_DEBUG
#define LOG_WARN SPDLOG_WARN
#define LOG_INFO SPDLOG_INFO
#define LOG_CRITICAL SPDLOG_CRITICAL
#define LOG_ERROR SPDLOG_ERROR

#include "IDA.h"