#pragma once
#include "Timer.h"
#include "Window.h"
#include "Field.h"
#include "MainPipelineGraph.h"

class App
{
	static constexpr const char* WINDOW_TITLE = "Tic Tac Toe";

	enum Axis : uint8_t
	{
		Left = 1, Horizontal = 2, Right = 4,
		Down = 8, Vertical = 16, Up = 32,
		LeftUpToDownRight = 64, LeftDownToUpRight = 128
	};

	bool run = true;
	bool play = true;
	float time = 0.0f;
	int8_t fieldsLeft = 9;
	Timer timer;
	WinAPI::Window window;
	GFX::Pipeline::MainPipelineGraph renderer;
	Field::Possesion currentPlayer = Field::Possesion::P1;
	std::vector<Field> fields;
	std::string gameLog = "";

	inline bool IsSameOwner(uint8_t f1, uint8_t f2, uint8_t f3) noexcept;
	inline bool CheckVictory(uint8_t index) noexcept;
	inline void ProcessInput();
	inline void ShowWindow();
	inline void ChangeImageButton(Field::Possesion owner);
	inline void MakeFrame();
	inline void ResetGame();

public:
	App(const std::string& commandLine = "");
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	~App() = default;

	size_t Run();
};