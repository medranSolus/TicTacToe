#include "DialogWindow.h"
#include "Math.h"
#include "WinApi.h"
#include "Utils.h"
#include "Surface.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_stdlib.h"
#include <algorithm>

namespace GFX::GUI
{
	std::vector<std::string> DialogWindow::drives;

	void DialogWindow::SetupDrives() noexcept
	{
		DWORD driveMask = GetLogicalDrives();
		std::error_code error;
		std::string drive = "A:";
		for (; drive.front() <= 'Z'; ++drive.front())
		{
			if ((driveMask & 1) && std::filesystem::exists(drive + "\\", error))
				drives.emplace_back(drive);
			driveMask >>= 1;
		}
	}

	std::vector<std::filesystem::directory_entry> DialogWindow::GetDirContent(const std::filesystem::directory_entry& entry) noexcept
	{
		std::vector<std::filesystem::directory_entry> dirContent;
		for (const auto& entry : std::filesystem::directory_iterator(entry, std::filesystem::directory_options::skip_permission_denied))
			if (Surface::IsImage(entry.path().extension().string()))
				dirContent.emplace_back(entry);
		std::sort(dirContent.begin(), dirContent.end(), [](const std::filesystem::directory_entry& e1, const std::filesystem::directory_entry& e2)
			{
				// Ascending
				if (e1.is_directory() && !e2.is_directory())
					return true;
				else if (!e1.is_directory() && e2.is_directory())
					return false;
				return e1.path().filename() < e2.path().filename();
			});
		return std::move(dirContent);
	}

	std::optional<std::string> DialogWindow::FileBrowserButton(const std::string& title, const std::string& startDir) noexcept
	{
		static std::filesystem::directory_entry currentDir;
		static size_t selected;
		if (ImGui::Button(title.c_str()))
		{
			currentDir = std::move(std::filesystem::directory_entry(std::filesystem::current_path().string() + "\\" + startDir));
			selected = -1;
			if (drives.size() == 0)
				SetupDrives();
			ImGui::OpenPopup(title.c_str());
		}

		// Always center this window when appearing
		ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f },
			ImGuiCond_Appearing, { 0.5f, 0.5f });
		std::string selectFile = "";
		if (ImGui::BeginPopupModal(title.c_str(), nullptr/*, ImGuiWindowFlags_NoMove*/))
		{
			if (!currentDir.exists())
				currentDir = std::move(std::filesystem::directory_entry(std::filesystem::current_path()));
			std::vector<std::filesystem::directory_entry> dirContent = std::move(GetDirContent(currentDir));

			if (ImGui::Button(" ^ "))
			{
				currentDir = std::move(std::filesystem::directory_entry(currentDir.path().parent_path()));
				selected = -1;
			}
			ImGui::SameLine();
			std::string driveLetter = currentDir.path().root_name().string();
			ImGui::SetNextItemWidth(40.0f);
			if (ImGui::BeginCombo("##dialog_drive", currentDir.path().root_name().string().c_str()))
			{
				for (const auto& letter : drives)
				{
					bool selected = (driveLetter == letter);
					if (ImGui::Selectable(letter.c_str(), selected))
						currentDir = std::move(std::filesystem::directory_entry(letter + "\\"));
					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();

			ImGui::BeginChild("##dialog_path", { -1.0f, 34.0f }, false, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::Dummy({ 0.0f, 0.0f });
			auto path = currentDir.path().wstring();
			path.erase(path.begin(), path.begin() + 3);
			ImGui::Text(Utils::ToAscii(path).c_str());
			ImGui::EndChild();

			ImGui::BeginChild("##dialog_content", { -1.0f, -35.0f }, true);
			for (size_t i = 0, size = dirContent.size(); i < size; ++i)
			{
				if (ImGui::Selectable(Utils::ToAscii(dirContent.at(i).path().filename().wstring()).c_str(), selected == i, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (!dirContent.at(i).is_directory())
						selected = i;
					if (ImGui::IsMouseDoubleClicked(0))
					{
						if (dirContent.at(i).is_directory())
						{
							currentDir = std::move(dirContent.at(i));
							selected = -1;
						}
					}
				}
			}
			ImGui::EndChild();

			ImGui::BeginChild("##dialog_selected", { -107.0f, 30.0f }, true);
			if (selected != -1)
				ImGui::Text(Utils::ToAscii(dirContent.at(selected).path().filename().wstring()).c_str());
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::Button("Select", { 0.0f, 30.0f }) && selected != -1)
			{
				selectFile = Utils::ToAscii(dirContent.at(selected).path().wstring());
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", { 0.0f, 30.0f }))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (selectFile.size())
			return selectFile;
		return {};
	}

	DialogWindow::Result DialogWindow::ShowInfo(const std::string& title, const std::string& text) noexcept
	{
		Result result = Result::None;
		if (!ImGui::IsPopupOpen(title.c_str()))
			ImGui::OpenPopup(title.c_str());
		ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f },
			ImGuiCond_Appearing, { 0.5f, 0.5f });
		if (ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::PushTextWrapPos(250.0f);
			ImGui::TextWrapped(text.c_str());
			ImGui::PopTextWrapPos();
			if (ImGui::Button("OK", { -1.0f, 0.0f }))
			{
				result = Result::Accept;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		return result;
	}
}