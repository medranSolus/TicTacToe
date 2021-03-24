#pragma once
#include "WinApiException.h"
#include <DirectXTex.h>
#include <memory>
#include <cassert>

namespace GFX
{
	class Surface
	{
	public:
		class Pixel
		{
			// RGBA
			uint32_t dword = 255 << 24U;

		public:
			Pixel() = default;
			constexpr Pixel(const Pixel& p) noexcept : dword(p.dword) {}
			constexpr Pixel(uint32_t dw) noexcept : dword(dw) {}
			constexpr Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept : dword((a << 24U) | (b << 16U) | (g << 8U) | r) {}
			constexpr Pixel(Pixel col, uint8_t a) noexcept : Pixel((a << 24U) | col.dword) {}
			constexpr Pixel& operator=(const Pixel& color) noexcept { dword = color.dword; return *this; }
			~Pixel() = default;

			constexpr uint32_t GetValue() const noexcept { return dword; }
			constexpr uint32_t GetRGBA() const noexcept { return dword; }
			constexpr uint32_t GetBGRA() const noexcept { return (dword & 0xFF00FF00) | GetB() | (static_cast<uint32_t>(GetR()) << 16U); }
			constexpr uint8_t GetA() const noexcept { return dword >> 24U; }
			constexpr uint8_t GetB() const noexcept { return (dword >> 16U) & 0xFFU; }
			constexpr uint8_t GetG() const noexcept { return (dword >> 8U) & 0xFFU; }
			constexpr uint8_t GetR() const noexcept { return dword & 0xFFU; }

			constexpr void SetA(uint8_t x) noexcept { dword = (dword & 0x00FFFFFFU) | (x << 24U); }
			constexpr void SetR(uint8_t r) noexcept { dword = (dword & 0xFF00FFFFU) | (r << 16U); }
			constexpr void SetG(uint8_t g) noexcept { dword = (dword & 0xFFFF00FFU) | (g << 8U); }
			constexpr void SetB(uint8_t b) noexcept { dword = (dword & 0xFFFFFF00U) | b; }
		};

	private:
		static constexpr DXGI_FORMAT PIXEL_FORMAT = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

		DirectX::ScratchImage scratch;
		const DirectX::Image* image;

	public:
		Surface(const std::string& name);
		Surface(size_t width, size_t height, DXGI_FORMAT format = PIXEL_FORMAT);
		Surface(Surface&& surface) noexcept = default;
		Surface(const Surface&) = delete;
		Surface& operator=(Surface&& surface) noexcept = default;
		Surface& operator=(const Surface&) = delete;
		~Surface() = default;

		constexpr DXGI_FORMAT GetFormat() const noexcept { return image->format; }
		constexpr size_t GetWidth() const noexcept { return image->width; }
		constexpr size_t GetHeight() const noexcept { return image->height; }
		constexpr size_t GetRowByteSize() const noexcept { return image->rowPitch; }
		constexpr size_t GetSize() const noexcept { return GetWidth() * GetHeight(); }
		inline bool HasAlpha() const noexcept { return !scratch.IsAlphaAllOpaque(); }
		inline Pixel* GetBuffer() noexcept { return reinterpret_cast<Pixel*>(image->pixels); }
		inline const Pixel* GetBuffer() const noexcept { return reinterpret_cast<const Pixel*>(image->pixels); }
		inline void Clear(const Pixel& value) noexcept { memset(GetBuffer(), value.GetValue(), GetWidth() * GetHeight() * sizeof(Pixel)); }

		constexpr void PutPixel(size_t x, size_t y, Pixel c) noexcept(!IS_DEBUG);
		constexpr Pixel GetPixel(size_t x, size_t y) const noexcept(!IS_DEBUG);

		static bool IsImage(const std::string& ext) noexcept;

#pragma region Exception
		class ImageException : public virtual Exception::BasicException
		{
			std::string info;

		public:
			inline ImageException(unsigned int line, const char* file, std::string note) noexcept
				: BasicException(line, file), info(std::move(note)) {}
			virtual ~ImageException() = default;

			constexpr const std::string& GetImageInfo() const noexcept { return info; }
			inline const char* GetType() const noexcept override { return "Image Exception"; }

			const char* what() const noexcept override;
		};

		class DirectXTexException : public ImageException, public Exception::WinApiException
		{
		public:
			inline DirectXTexException(unsigned int line, const char* file, HRESULT hResult, std::string note) noexcept
				: BasicException(line, file), ImageException(line, file, note), WinApiException(line, file, hResult) {}
			virtual ~DirectXTexException() = default;

			inline const char* GetType() const noexcept override { return "DirectXTex Exception"; }

			const char* what() const noexcept override;
		};
#pragma endregion
	};

	constexpr void Surface::PutPixel(size_t x, size_t y, Pixel c) noexcept(!IS_DEBUG)
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(x < GetWidth());
		assert(y < GetHeight());
		GetBuffer()[y * GetWidth() + x] = c;
	}

	constexpr Surface::Pixel Surface::GetPixel(size_t x, size_t y) const noexcept(!IS_DEBUG)
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(x < GetWidth());
		assert(y < GetHeight());
		return GetBuffer()[y * GetWidth() + x];
	}
}