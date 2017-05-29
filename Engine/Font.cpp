#include "Font.h"
#pragma comment(lib, "dwrite.lib")


DWrite::DWrite()
{
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_ISOLATED,
		__uuidof( IDWriteFactory ),
		reinterpret_cast< IUnknown** >( pFactory.GetAddressOf() )
	);
	if( FAILED( hr ) )
		throw std::runtime_error( "Failed to create DWrite factory." );
}

IDWriteFactory * DWrite::operator->()const
{
	return pFactory.Get();
}


Font::Font(
	DWrite &Dwrite,
	const std::wstring &FontName,
	float FontSize,
	DWRITE_FONT_WEIGHT Weight,
	DWRITE_FONT_STYLE Style,
	DWRITE_FONT_STRETCH Stretch,
	const std::wstring & Locale )
	:
	pFormat( [Dwrite](
		const std::wstring & FontName,
		DWRITE_FONT_WEIGHT Weight,
		DWRITE_FONT_STYLE Style,
		DWRITE_FONT_STRETCH Stretch,
		float FontSize,
		const std::wstring &Locale )
{
	Microsoft::WRL::ComPtr<IDWriteTextFormat> pFormat;
	HRESULT hr;
	if( FAILED( hr = Dwrite->CreateTextFormat(
		FontName.c_str(), nullptr, Weight, Style, Stretch, FontSize, Locale.c_str(), &pFormat ) ) )
	{
		throw std::runtime_error( "Failed to create text format." );
	}

	return pFormat;
}( FontName, Weight, Style, Stretch, FontSize, Locale ) )
{
}

Font::Font( Font &&Fnt )
	:
	pFormat( std::move( Fnt.pFormat ) )
{
}

Font & Font::operator=( Font &&Src )
{
	pFormat = std::move( Src.pFormat );
	return *this;
}

Font::operator IDWriteTextFormat*( ) const
{
	return pFormat.Get();
}

IDWriteTextFormat * Font::operator->()
{
	return pFormat.Get();
}