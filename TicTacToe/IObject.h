#pragma once
#include "IRenderable.h"

namespace GFX
{
	class IObject : public virtual Pipeline::IRenderable
	{
	public:
		virtual ~IObject() = default;

		virtual const DirectX::XMFLOAT3& GetAngle() const noexcept = 0;
		virtual void SetAngle(const DirectX::XMFLOAT3& meshAngle) noexcept = 0;

		virtual float GetScale() const noexcept = 0;
		virtual void SetScale(float newScale) noexcept = 0;

		virtual const DirectX::XMFLOAT3& GetPos() const noexcept = 0;
		virtual void SetPos(const DirectX::XMFLOAT3& position) noexcept = 0;

		virtual const std::string& GetName() const noexcept = 0;
		virtual void SetName(const std::string& newName) noexcept = 0;

		virtual inline void Update(const DirectX::XMFLOAT3& delta, const DirectX::XMFLOAT3& deltaAngle) noexcept { UpdatePos(delta); UpdateAngle(deltaAngle); }
		virtual void UpdatePos(const DirectX::XMFLOAT3& delta) noexcept = 0;
		virtual void UpdateAngle(const DirectX::XMFLOAT3& delta) noexcept = 0;
	};
}