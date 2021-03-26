#pragma once
#include "Rectangle.h"

class Field
{
public:
	enum class Possesion : uint8_t { None, P1, P2 };

private:
	static constexpr const char* EMPTY_TEXTURE = "empty.png";
	static constexpr float FRONT_Z = 7.85f;
	static constexpr float BACK_Z = FRONT_Z + 0.5f;
	static constexpr float OFFSET = 1.5f;
	static constexpr float GRID_ELEMENT_SIZE = 3.0f;
	static constexpr float ELEMENT_SIZE = GRID_ELEMENT_SIZE - 0.2f;
	static constexpr float POSITION_MOVE = GRID_ELEMENT_SIZE + (GRID_ELEMENT_SIZE - ELEMENT_SIZE) / 2.0f;

	static int gridElementSize;
	static std::string player1File;
	static std::string player2File;

	Possesion owner = Possesion::None;
	GFX::Shape::Rectangle field;
	int x;
	int y;

public:
	Field(GFX::Graphics& gfx, GFX::Pipeline::RenderGraph& graph, const std::string& tag, short posX, short posY);
	Field(Field&&) = default;
	Field& operator=(Field&&) = default;
	~Field() = default;

	static void SetPlayerImage(Possesion player, std::string&& file);

	constexpr Possesion GetOwner() const noexcept { return owner; }
	inline void ReloadImage(GFX::Graphics& gfx) noexcept { field.ChangeTexture(gfx, owner == Possesion::P1 ? player1File : player2File); }
	inline void Submit() noexcept { field.Submit(); }
	inline void SetWinner() noexcept { field.SetOutline(); }

	bool MouseHoover(int mouseX, int mouseY) noexcept;
	void SetBack() noexcept;
	void SetFront() noexcept;
	void SetOwner(GFX::Graphics& gfx, Possesion player);
	void ChangeImage() const noexcept;
	void Reset(GFX::Graphics& gf);
};