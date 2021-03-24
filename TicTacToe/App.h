#pragma once
#include "Timer.h"
#include "Window.h"
#include "Rectangle.h"
#include "MainPipelineGraph.h"

class App
{
	static constexpr const char* WINDOW_TITLE = "Tic Tac Toe";

	bool run = true;
	WinAPI::Window window;
	GFX::Pipeline::MainPipelineGraph renderer;
	std::vector<std::shared_ptr<GFX::Shape::Rectangle>> fields;

	inline void ProcessInput();
	inline void ShowObjectWindow();
	inline void ShowOptionsWindow();
	inline void ChangeImageButton();
	inline void MakeFrame();

public:
	App(const std::string& commandLine = "");
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	~App() = default;

	size_t Run();
};