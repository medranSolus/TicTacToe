#include "Surface.h"
#include "Utils.h"
#include <filesystem>

#define IMG_EXCEPT(info) GFX::Surface::ImageException(__LINE__, __FILE__, info)
// Enables useage of IMG_ macros in current scope
#define DXT_ENABLE_EXCEPT() HRESULT __hResult
// Before using needs call to DXT_ENABLE_EXCEPT()
#define DXT_EXCEPT(info) GFX::Surface::DirectXTexException(__LINE__, __FILE__, __hResult, info)
// Before using needs call to DXT_ENABLE_EXCEPT()
#define	DXT_THROW_FAILED(call, info) if( FAILED(__hResult = (call))) throw DXT_EXCEPT(info)

namespace GFX
{
	Surface::Surface(const std::string& name)
	{
		DXT_ENABLE_EXCEPT();
		const std::filesystem::path path = name;
		std::string ext = path.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), [](char c) { return std::tolower(c); });

		if (ext == ".dds")
		{
			DXT_THROW_FAILED(DirectX::LoadFromDDSFile(Utils::ToUtf8(name).c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_FORCE_RGB, nullptr, scratch),
				"Loading image \"" + name + "\": failed.");
			image = scratch.GetImage(0, 0, 0);

			if (image->format != PIXEL_FORMAT)
			{
				DirectX::ScratchImage decompressed;
				DXT_THROW_FAILED(DirectX::Decompress(*image, PIXEL_FORMAT, decompressed),
					"Decompressing image \"" + name + "\": failed.");
				scratch = std::move(decompressed);
				image = scratch.GetImage(0, 0, 0);
			}
		}
		else if (ext == ".hdr")
		{
			DXT_THROW_FAILED(DirectX::LoadFromHDRFile(Utils::ToUtf8(name).c_str(), nullptr, scratch),
				"Loading image \"" + name + "\": failed.");

			throw IMG_EXCEPT("HDR textures not implemented!");
		}
		else if (ext == ".tga")
		{
			DXT_THROW_FAILED(DirectX::LoadFromTGAFile(Utils::ToUtf8(name).c_str(), nullptr, scratch),
				"Loading image \"" + name + "\": failed.");

			throw IMG_EXCEPT("TGA textures not implemented!");
		}
		else
		{
			DXT_THROW_FAILED(DirectX::LoadFromWICFile(Utils::ToUtf8(name).c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_IGNORE_SRGB, nullptr, scratch),
				"Loading image \"" + name + "\": failed.");
			image = scratch.GetImage(0, 0, 0);

			if (image->format != PIXEL_FORMAT)
			{
				DirectX::ScratchImage converted;
				DXT_THROW_FAILED(DirectX::Convert(*image, PIXEL_FORMAT, DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, converted),
					"Converting image \"" + name + "\": failed.");
				scratch = std::move(converted);
				image = scratch.GetImage(0, 0, 0);
			}
		}
	}

	Surface::Surface(size_t width, size_t height, DXGI_FORMAT format)
	{
		DXT_ENABLE_EXCEPT();
		DXT_THROW_FAILED(scratch.Initialize2D(format, width, height, 1U, 1U),
			"Creating image " + std::to_string(width) + "x" + std::to_string(height) + ": failed.");
		image = scratch.GetImage(0, 0, 0);
	}

	bool Surface::IsImage(const std::string& ext) noexcept
	{
		return ext == ".png" ||
			ext == ".jpg" ||
			ext == ".jpeg" ||
			ext == ".bmp" ||
			ext == ".tif" ||
			ext == ".tiff" ||
			ext == ".ico" ||
			ext == ".gif" ||
			ext == ".hdp" ||
			ext == ".jxr" ||
			ext == ".wdp" ||
			ext == ".dds" ||
			ext == ".hdr" ||
			ext == ".tga";
	}

#pragma region Exception
	const char* Surface::ImageException::what() const noexcept
	{
		std::ostringstream stream;
		stream << this->BasicException::what()
			<< "\n[Image Info] " << GetImageInfo();
		whatBuffer = stream.str();
		return whatBuffer.c_str();
	}

	const char* Surface::DirectXTexException::what() const noexcept
	{
		std::ostringstream stream;
		stream << this->WinApiException::what()
			<< "\n[Image Info] " << GetImageInfo();
		whatBuffer = stream.str();
		return whatBuffer.c_str();
	}
#pragma endregion
}