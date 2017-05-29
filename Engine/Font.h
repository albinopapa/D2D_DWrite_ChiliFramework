#pragma once

#include <string>
#include <dwrite.h>
#include <wrl/client.h>

// Loads a font using the DWrite class
// Can be changed by move assignment only, 
// which destroys the original and replaces with new instance
class Font
{
public:
	Font(
		class DWrite &Dwrite,
		const std::wstring &FontName,
		float FontSize = 16.f,
		DWRITE_FONT_WEIGHT Weight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE Style = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH Stretch = DWRITE_FONT_STRETCH_NORMAL,
		const std::wstring &Locale = L"en-us" );
	Font( Font && );
	Font &operator=( Font && );

	Font( const Font & ) = delete;
	Font &operator=( const Font & ) = delete;

	operator IDWriteTextFormat*( ) const;
	IDWriteTextFormat* operator->();
private:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> pFormat;
};


// Must stay alive as long as all fonts are alive.
class DWrite
{
public:
	DWrite();

	IDWriteFactory *operator->()const;
private:
	Microsoft::WRL::ComPtr<IDWriteFactory> pFactory;
};

