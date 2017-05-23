/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "RectF.h"
#include <d2d1_1.h>


class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	/*struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};*/
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame2D();
	void EndFrame();
	
	void BeginFrame();
	void BeginFrame2D();

	// Don't use PutPixel if you don't have to.
	// Direct2D uses Direct3D on backend, and creating pixel sized quads is 
	// expensive
	void PutPixel( int X, int Y, int R, int G, int B );
	void PutPixel( const Vec2 &Point, Color C );

	void OutlineRect( float Left, float Top, float Right, float Bottom, Color C );
	void OutlineRect( const RectF &Rect, Color C );
	void FillRect( float x0, float y0, float x1, float y1, Color c );
	void FillRect( const RectF& rect, Color c );

	void OutlineCircle( float X, float Y, float Radius, Color C );
	void OutlineCircle( const Vec2& Center, float Radius, Color C );
	void FillCircle( float x, float y, float radius, Color c );
	void FillCircle( const Vec2& Center, float Radius, Color C );

	void DrawLine( float X0, float Y0, float X1, float Y1, Color C );
	void DrawLine( const Vec2 &P0, const Vec2 &P1, Color C );

	void DrawString( const std::wstring &Str, const RectF &Rect, const class Font &F, Color C );
	~Graphics();

private:
	void InitDirect3D( HWND WinHandle );
	void InitDirect2D();
	D2D1_COLOR_F ConvertColor( Color C );
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	/*Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;*/

	Microsoft::WRL::ComPtr<ID2D1Device>					pDevice2d;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext>			pContext2d;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>				pRenderTarget2d;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>		pBrush2d;

	//D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	//Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
};