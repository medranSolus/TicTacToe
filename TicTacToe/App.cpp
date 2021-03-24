#include "App.h"
#include "BaseProbe.h"
#include "DialogWindow.h"
#include "Math.h"

inline void App::ProcessInput()
{
	while (window.Keyboard().IsKeyReady())
	{
		if (auto opt = window.Keyboard().ReadKey())
		{
			if (opt.value().IsDown())
			{
				switch (opt.value().GetCode())
				{
				case VK_ESCAPE:
				{
					window.SwitchCursor();
					break;
				}
				case VK_F1:
				{
					window.Gfx().SwitchGUI();
					break;
				}
				}
			}
		}
	}
}

inline void App::ShowObjectWindow()
{
	static GFX::Probe::BaseProbe probe;
	if (ImGui::Begin("Objects", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		static std::vector<std::shared_ptr<GFX::Shape::Rectangle>>::iterator currentItem = fields.begin();
		if (ImGui::BeginCombo("Selected object", (*currentItem)->GetName().c_str()))
		{
			for (auto it = fields.begin(); it != fields.end(); ++it)
			{
				bool selected = (currentItem == it);
				if (ImGui::Selectable((*it)->GetName().c_str(), selected))
				{
					(*currentItem)->DisableOutline();
					currentItem = it;
					(*currentItem)->SetOutline();
				}
				if (selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ChangeImageButton();
		ImGui::NewLine();
		(*currentItem)->Accept(window.Gfx(), probe);
	}
	ImGui::End();
}

inline void App::ShowOptionsWindow()
{
	static GFX::Probe::BaseProbe probe;
	if (ImGui::Begin("Options", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::Button("Exit"))
			run = false;
		ImGui::SameLine();
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		renderer.ShowWindow(window.Gfx());
	}
	ImGui::End();
}

inline void App::ChangeImageButton()
{
	static std::optional<std::string> path = {};
	static std::optional<std::string> error = {};

	if (const auto file = GFX::GUI::DialogWindow::FileBrowserButton("Change image", ""))
		path = file;
	if (path)
	{
		try
		{
			//error = renderer.ChangeSkybox(window.Gfx(), path.value());
		}
		catch (const std::exception& e)
		{
			error = e.what();
		}
		path = {};
	}
	if (error)
	{
		if (GFX::GUI::DialogWindow::ShowInfo("Error",
			"Error occured during loading skybox.\n" + error.value()) == DialogResult::Accept)
			error = {};
	}
}

void App::MakeFrame()
{
	window.Gfx().BeginFrame();
	ProcessInput();
	ShowObjectWindow();
	ShowOptionsWindow();
	for (auto& rect : fields)
		rect->Submit();
	renderer.Execute(window.Gfx());
	renderer.Reset();
	window.Gfx().EndFrame();
}

App::App(const std::string& commandLine)
	: window(1600, 900, WINDOW_TITLE), renderer(window.Gfx(), 16.2f)
{
	window.Gfx().Gui().SetFont("Arial.ttf", 14.0f);
	window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveFovLH(1.047f, window.Gfx().GetRatio(), 0.01f, 500.0f));
	window.Gfx().SetView(DirectX::XMMatrixIdentity());

	constexpr float FIELD = 7.85f, POS = 3.0f, SIZE = 2.7f;
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(-POS, POS, FIELD), "0", "empty.png", SIZE, SIZE));
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(0.0f, POS, FIELD), "1", "p1.png", SIZE, SIZE));
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(POS, POS, FIELD), "2", "p2.png", SIZE, SIZE));
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(-POS, 0.0f, FIELD), "3", "p1.png", SIZE, SIZE));
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(0.0f, 0.0f, FIELD), "4", "p2.png", SIZE, SIZE));
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(POS, 0.0f, FIELD), "5", "empty.png", SIZE, SIZE));
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(-POS, -POS, FIELD), "6", "empty.png", SIZE, SIZE));
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(0.0f, -POS, FIELD), "7", "empty.png", SIZE, SIZE));
	fields.emplace_back(std::make_shared<GFX::Shape::Rectangle>(window.Gfx(), renderer, DirectX::XMFLOAT3(POS, -POS, FIELD), "8", "empty.png", SIZE, SIZE));
}

size_t App::Run()
{
	while (run)
	{
		if (const auto status = WinAPI::Window::ProcessMessage())
			return status.value();
		MakeFrame();
	}
	return 0U;
}