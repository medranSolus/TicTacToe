#include "Field.h"

int Field::gridElementSize = 0;
std::string Field::player1File = "p1.png";
std::string Field::player2File = "p2.png";

Field::Field(GFX::Graphics& gfx, GFX::Pipeline::RenderGraph& graph, const std::string& tag, short posX, short posY)
	: field(gfx, graph, { posX * POSITION_MOVE - OFFSET, posY * POSITION_MOVE, FRONT_Z },
		tag, EMPTY_TEXTURE, ELEMENT_SIZE, ELEMENT_SIZE)
{
	float scale = gfx.GetHeight() / (3.0f * GRID_ELEMENT_SIZE);
	if (gridElementSize == 0)
		gridElementSize = static_cast<int>(ELEMENT_SIZE * scale);
	x = static_cast<int>((field.GetPos().x - ELEMENT_SIZE * 0.5f + GRID_ELEMENT_SIZE * 1.5f + OFFSET) * scale);
	y = static_cast<int>((GRID_ELEMENT_SIZE * 1.5f - ELEMENT_SIZE * 0.5f - field.GetPos().y) * scale);
}

void Field::SetPlayerImage(Possesion player, std::string&& file)
{
	switch (player)
	{
	case Field::Possesion::P1:
	{
		player1File = std::move(file);
		break;
	}
	case Field::Possesion::P2:
	{
		player2File = std::move(file);
		break;
	}
	}
}

bool Field::MouseHoover(int mouseX, int mouseY) noexcept
{
	if (owner == Possesion::None)
	{
		if (mouseX >= x && mouseX <= x + gridElementSize
			&& mouseY >= y && mouseY <= y + gridElementSize)
		{
			field.SetOutline();
			return true;
		}
		field.DisableOutline();
	}
	return false;
}

void Field::SetBack() noexcept
{
	auto& pos = field.GetPos();
	field.SetPos({ pos.x, pos.y, BACK_Z });
}

void Field::SetFront() noexcept
{
	auto& pos = field.GetPos();
	field.SetPos({ pos.x, pos.y, FRONT_Z });
}

void Field::SetOwner(GFX::Graphics& gfx, Possesion player)
{
	owner = player;
	field.DisableOutline();
	ReloadImage(gfx);
}

void Field::Reset(GFX::Graphics& gfx)
{
	owner = Possesion::None;
	field.DisableOutline();
	field.ChangeTexture(gfx, EMPTY_TEXTURE);
}