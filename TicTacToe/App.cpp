#include "App.h"
#include "BaseProbe.h"
#include "DialogWindow.h"
#include "Math.h"

inline bool App::IsSameOwner(uint8_t f1, uint8_t f2, uint8_t f3) noexcept
{
	const Field::Possesion ownerF2 = fields.at(f2).GetOwner();
	if (fields.at(f1).GetOwner() == ownerF2 && ownerF2 == fields.at(f3).GetOwner())
	{
		fields.at(f1).SetWinner();
		fields.at(f2).SetWinner();
		fields.at(f3).SetWinner();
		return true;
	}
	return false;
}

inline bool App::CheckVictory(uint8_t index) noexcept
{
	uint8_t checkAxis = 0;
	switch (index)
	{
	case 0: // Left Down
	{
		checkAxis = Axis::Left | Axis::Down | Axis::LeftDownToUpRight;
		break;
	}
	case 1: // Center Down
	{
		checkAxis = Axis::Horizontal | Axis::Down;
		break;
	}
	case 2: // Rright Down
	{
		checkAxis = Axis::Down | Axis::Right | Axis::LeftUpToDownRight;
		break;
	}
	case 3: // Left Middle
	{
		checkAxis = Axis::Left | Axis::Vertical;
		break;
	}
	case 4: // Center Middle
	{
		checkAxis = Axis::Horizontal | Axis::Vertical | Axis::LeftUpToDownRight | Axis::LeftDownToUpRight;
		break;
	}
	case 5: // Right Middle
	{
		checkAxis = Axis::Right | Axis::Vertical;
		break;
	}
	case 6: // Left Up
	{
		checkAxis = Axis::Left | Axis::Up | Axis::LeftUpToDownRight;
		break;
	}
	case 7: // Center Up
	{
		checkAxis = Axis::Horizontal | Axis::Up;
		break;
	}
	case 8: // Right Up
	{
		checkAxis = Axis::Right | Axis::Up | Axis::LeftDownToUpRight;
		break;
	}
	default:
		return false;
	}
	if (checkAxis & Axis::Left)
		if (IsSameOwner(0, 3, 6))
			return true;
	if (checkAxis & Axis::Horizontal)
		if (IsSameOwner(1, 4, 7))
			return true;
	if (checkAxis & Axis::Right)
		if (IsSameOwner(2, 5, 8))
			return true;
	if (checkAxis & Axis::Down)
		if (IsSameOwner(0, 1, 2))
			return true;
	if (checkAxis & Axis::Vertical)
		if (IsSameOwner(3, 4, 5))
			return true;
	if (checkAxis & Axis::Up)
		if (IsSameOwner(6, 7, 8))
			return true;
	if (checkAxis & Axis::LeftUpToDownRight)
		if (IsSameOwner(2, 4, 6))
			return true;
	if (checkAxis & Axis::LeftDownToUpRight)
		if (IsSameOwner(0, 4, 8))
			return true;
	return false;
}

inline void App::ProcessInput()
{
	if (play)
	{
		while (window.Mouse().IsInput())
		{
			if (auto e = window.Mouse().Read())
			{
				switch (e->GetType())
				{
				case WinAPI::Mouse::Event::Type::Move:
				case WinAPI::Mouse::Event::Type::LeftUp:
				case WinAPI::Mouse::Event::Type::LeftDown:
				{
					for (uint8_t i = 0; i < 9; ++i)
					{
						auto& field = fields.at(i);
						if (field.MouseHoover(e->GetX(), e->GetY()))
						{
							if (e->GetType() == WinAPI::Mouse::Event::Type::LeftUp)
							{
								field.SetFront();
								field.SetOwner(window.Gfx(), currentPlayer);
								--fieldsLeft;
								currentPlayer = currentPlayer == Field::Possesion::P1
									? Field::Possesion::P2 : Field::Possesion::P1;
								if (CheckVictory(i))
								{
									play = false;
									gameLog += "Player ";
									gameLog += currentPlayer == Field::Possesion::P1 ? "1" : "2";
									gameLog += " won! ";
									gameLog += std::to_string(static_cast<size_t>(time));
									gameLog += "s\n";
									break;
								}
								if (fieldsLeft == 0)
								{
									fieldsLeft = -1;
									play = false;
									gameLog += "Draw! ";
									gameLog += std::to_string(static_cast<size_t>(time));
									gameLog += "s\n";
									break;
								}
							}
							else if (e->IsLeftDown())
								field.SetBack();
						}
						else
							field.SetFront();
					}
					break;
				}
				}
			}
		}
	}
}

inline void App::ShowWindow()
{
	if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::NewLine();
		ImGui::Text("Game time: %.0fs", time);
		ImGui::NewLine();
		ImGui::NewLine();
		if (fieldsLeft != -1)
			ImGui::Text("Player %s %s", currentPlayer == Field::Possesion::P1 ? "1" : "2", play ? "turn" : "won!!!");
		else
			ImGui::Text("Draw! Game over!");
		ImGui::NewLine();
		if (ImGui::Button("Exit Game"))
			run = false;
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
			ResetGame();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Text("Change image:");
		ChangeImageButton(Field::Possesion::P1);
		ChangeImageButton(Field::Possesion::P2);
		ImGui::NewLine();
		ImGui::InputTextMultiline("##log", gameLog.data(), gameLog.size(), { -FLT_MIN, -FLT_MIN }, ImGuiInputTextFlags_ReadOnly);
	}
	ImGui::End();
}

inline void App::ChangeImageButton(Field::Possesion owner)
{
	static std::optional<std::string> path = {};
	static std::optional<std::string> error = {};
	static std::string title = "Player X";

	title.back() = owner == Field::Possesion::P1 ? '1' : '2';
	if (const auto file = GFX::GUI::DialogWindow::FileBrowserButton(title, ""))
		path = file;
	if (path)
	{
		try
		{
			Field::SetPlayerImage(owner, std::move(path.value()));
			for (auto& field : fields)
				if (field.GetOwner() == owner)
					field.ReloadImage(window.Gfx());
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
	ShowWindow();
	for (auto& field : fields)
		field.Submit();
	renderer.Execute(window.Gfx());
	renderer.Reset();
	window.Gfx().EndFrame();
}

inline void App::ResetGame()
{
	play = true;
	time = 0.0f;
	fieldsLeft = 9;
	for (auto& field : fields)
		field.Reset(window.Gfx());
	currentPlayer = currentPlayer == Field::Possesion::P1
		? Field::Possesion::P2 : Field::Possesion::P1;
	window.Mouse().Flush();
	timer.Mark();
}

App::App(const std::string& commandLine)
	: window(1200, 900, WINDOW_TITLE), renderer(window.Gfx(), 16.2f)
{
	window.Gfx().Gui().SetFont("Arial.ttf", 30.0f);
	window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveFovLH(1.047f, window.Gfx().GetRatio(), 0.01f, 500.0f));
	window.Gfx().SetView(DirectX::XMMatrixIdentity());

	uint8_t tag = 0;
	fields.reserve(9);
	for (short y = -1; y < 2; ++y)
		for (short x = -1; x < 2; ++x)
			fields.emplace_back(window.Gfx(), renderer, std::to_string(tag++), x, y);
}

size_t App::Run()
{
	timer.Mark();
	while (run)
	{
		if (const auto status = WinAPI::Window::ProcessMessage())
			return status.value();
		MakeFrame();
		if (play)
			time += timer.Mark();
	}
	return 0U;
}