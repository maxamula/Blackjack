#include "options.h"

namespace blackjack
{
	Options::Options()
    {
        std::function<void(void)> overlay = [&]()
        {
            ImGui::SetNextWindowPos(ImVec2(490, 200));
            ImGui::SetNextWindowSize(ImVec2(300, 200));
            ImGui::Begin("overlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
            ImGui::Text("Player:");

            ImGui::Text("Skin sets:");
            ImGui::Columns(2, 0, false);
            if (ImGui::Button("Normal")) { PlayCard::LoadSkinSet("assets\\normal"); }
            ImGui::NextColumn();
            if (ImGui::Button("Serpia")) { PlayCard::LoadSkinSet("assets\\serpia"); }
            ImGui::Columns(1);

            char path[MAX_PATH];
            path[0] = '\0';
            ImGui::InputText("Custom: ", path, MAX_PATH);

            if (ImGui::Button("Set skin set"))
            {
                PlayCard::LoadSkinSet(path);
            }
            if (ImGui::Button("Return")) { GotoGame(); }
            ImGui::End();
        };

        m_overlay = this->CreateObject("Overlay");
        m_overlay.SetOverlay(overlay);
    }

}