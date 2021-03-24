#include "BaseProbe.h"
#include "BaseShape.h"
#include "Math.h"

#define Tag(label) MakeTag(label).c_str()

namespace GFX::Probe
{
	inline std::string BaseProbe::MakeTag(const std::string& label) const noexcept
	{
		return label + "##" + (technique == nullptr ? "None" : technique->GetName());
	}

	void BaseProbe::SetTechnique(Pipeline::Technique* currentTechnique) noexcept
	{
		technique = currentTechnique;
		auto& style = ImGui::GetStyle();
		auto colorMark = style.Colors[ImGuiCol_CheckMark];
		auto colorText = style.Colors[ImGuiCol_Text];
		style.Colors[ImGuiCol_CheckMark] = style.Colors[ImGuiCol_Text] = { 0.4f, 1.0f, 0.6f, 1.0f };
		ImGui::Checkbox(Tag("-- " + technique->GetName() + " --"), &technique->Active());
		style.Colors[ImGuiCol_CheckMark] = colorMark;
		style.Colors[ImGuiCol_Text] = colorText;
	}

	bool BaseProbe::Visit(Data::CBuffer::DynamicCBuffer& buffer) const noexcept
	{
		bool dirty = false;
		if (auto scale = buffer["scale"]; scale.Exists())
		{
			dirty |= ImGui::InputFloat(Tag("Scale"), &scale, 0.01f, 0.0f, "%.3f");
			if (static_cast<float>(scale) < 0.001f)
				scale = 0.001f;
		}
		if (auto position = buffer["position"]; position.Exists())
		{
			ImGui::Columns(2, "##mesh_options", false);
			DirectX::XMFLOAT3& pos = static_cast<DirectX::XMFLOAT3&>(position);
			ImGui::Text("Position");
			ImGui::SetNextItemWidth(-15.0f);
			dirty |= ImGui::InputFloat(Tag("X##position"), &pos.x, 0.1f, 0.0f, "%.2f");
			ImGui::SetNextItemWidth(-15.0f);
			dirty |= ImGui::InputFloat(Tag("Y##position"), &pos.y, 0.1f, 0.0f, "%.2f");
			ImGui::SetNextItemWidth(-15.0f);
			dirty |= ImGui::InputFloat(Tag("Z##position"), &pos.z, 0.1f, 0.0f, "%.2f");
			ImGui::NextColumn();
		}
		if (auto angle = buffer["angle"]; angle.Exists())
		{
			DirectX::XMFLOAT3& rotation = static_cast<DirectX::XMFLOAT3&>(angle);
			ImGui::Text("Rotation");
			ImGui::SetNextItemWidth(-15.0f);
			dirty |= ImGui::SliderAngle(Tag("X##rotation"), &rotation.x, 0.0f, 360.0f, "%.2f");
			ImGui::SetNextItemWidth(-15.0f);
			dirty |= ImGui::SliderAngle(Tag("Y##rotation"), &rotation.y, 0.0f, 360.0f, "%.2f");
			ImGui::SetNextItemWidth(-15.0f);
			dirty |= ImGui::SliderAngle(Tag("Z##rotation"), &rotation.z, 0.0f, 360.0f, "%.2f");
			ImGui::Columns(1);
		}
		return dirty;
	}
}