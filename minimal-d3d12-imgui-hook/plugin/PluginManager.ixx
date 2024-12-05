module;

#include "../pch.h"
#include <vector>
#include "Plugin.h"

export module PluginManager;

import API;
import Sample;

export class PluginManager
{
private:
    std::vector<Plugin*> m_Plugins;

    //show settings by default
    bool m_ShowSettings = true;
    bool m_Initialized = false;

    
    static DWORD WINAPI DelayedStart(LPVOID lParam) {
        static_cast<PluginManager*>(lParam)->OnLoad();
        return 0;
    }
public:
    PluginManager() {
        CreateThread(nullptr, 0, &DelayedStart, static_cast<LPVOID>(this), 0, nullptr);
    }

    ~PluginManager() {
        for (auto& plugin : m_Plugins) {
            plugin->OnUnload();
        }
        RemovePatches();
    }

    void OnLoad()
    {
        InitializeAPI();
        ApplyPatches();
        m_Plugins.push_back(new Sample());
        for (auto& plugin : m_Plugins) {
            plugin->OnLoad();
        }
        m_Initialized = true;
    }

    void Present() {
        for (auto& plugin : m_Plugins) {
            plugin->OnDraw();
        }

        //simple menu
        if (m_ShowSettings) {
            ImGui::SetNextWindowPos({ 0.f, 0.f }, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({ 500.f, 300.f }, ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Settings", &m_ShowSettings)) {
                ImGui::Text("Hello World!");
            }
            ImGui::End();
        }
    }

    void WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_KEYUP:
            if (wParam == VK_DELETE) {
                m_ShowSettings = !m_ShowSettings;
            }
            break;
        default:
            break;
        }
    }

    bool IsInitialized() const {
        return m_Initialized;
    }

};