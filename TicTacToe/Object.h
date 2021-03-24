#pragma once
#include "GfxObject.h"
#include "BasicObject.h"

namespace GFX
{
	class Object : public GfxObject, public BasicObject
	{
	protected:
		DirectX::XMMATRIX CreateTransformMatrix() const noexcept;

	public:
		Object(const DirectX::XMFLOAT3& position) noexcept;
		Object(const std::string& name = "") noexcept;
		Object(const DirectX::XMFLOAT3& position, const std::string& name, float scale = 1.0f) noexcept;
		Object(const Object&) = default;
		Object(Object&&) = default;
		Object& operator=(const Object&) = default;
		Object& operator=(Object&&) = default;
		virtual ~Object() = default;

		virtual inline void UpdateTransformMatrix() noexcept { DirectX::XMStoreFloat4x4(transform.get(), CreateTransformMatrix()); }

		void SetAngle(const DirectX::XMFLOAT3& meshAngle) noexcept override;
		void SetScale(float newScale) noexcept override;
		void SetPos(const DirectX::XMFLOAT3& position) noexcept override;
		void Update(const DirectX::XMFLOAT3& delta, const DirectX::XMFLOAT3& deltaAngle) noexcept override;
		void UpdatePos(const DirectX::XMFLOAT3& delta) noexcept override;
		void UpdateAngle(const DirectX::XMFLOAT3& deltaAngle) noexcept override;
		bool Accept(Graphics& gfx, Probe::BaseProbe& probe) noexcept override;
	};
}