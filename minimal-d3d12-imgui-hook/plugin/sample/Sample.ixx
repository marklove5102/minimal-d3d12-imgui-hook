module;

#include "../../pch.h"

#include "../Plugin.h"

export module Sample;

import API;
import Globals;

export class Sample : public Plugin {
private:
public:
    void OnLoad() override {
        
    }

    void OnUnload() override {
	
    }

    void OnDraw() override {
        auto drawList = ImGui::GetBackgroundDrawList();

        //simple shape
        drawList->AddCircleFilled({ 100.f, 100.f }, 30.f, IM_COL32(255, 0, 0, 128));
    }
};

