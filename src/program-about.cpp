#include "program.h"
#include <imgui.h>

void Program::renderGuiAbout()
{
    if (ImGui::BeginPopupModal("GoldSrc Maps", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("A map viewer for Valve's GoldSrc engine.");
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
