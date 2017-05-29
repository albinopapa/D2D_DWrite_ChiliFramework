/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
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
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>
#include "Font.h"

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d2d1.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key )
{
	assert( key.hWnd != nullptr );

	InitDirect3D( key.hWnd );

	InitDirect2D();

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	/*pSysBuffer = reinterpret_cast<Color*>( 
		_aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight,16u ) );*/
}

void Graphics::DrawString( const std::wstring & Str, const RectF & Rect, const Font & F, Color C )
{	
	pBrush2d->SetColor( ConvertColor( C ) );
	pContext2d->DrawText( Str.c_str(), static_cast< UINT32 >( Str.length() ), F, Rect, pBrush2d.Get() );
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	/*if( pSysBuffer )
	{
		_aligned_free( pSysBuffer );
		pSysBuffer = nullptr;
	}*/
	// clear the state of the device context before destruction
	//if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::InitDirect3D( HWND WinHandle )
{
	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = WinHandle;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		nullptr
		/*&pImmediateContext*/ ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating device and swap chain" );
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		( LPVOID* )&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	/*if( FAILED( hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating render target view on backbuffer" );
	}*/


	// set backbuffer as the render target using created view
	//pImmediateContext->OMSetRenderTargets( 1, pRenderTargetView.GetAddressOf(), nullptr );


	// set viewport dimensions
	/*D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1, &vp );*/


	///////////////////////////////////////
	// create texture for cpu render target
	/*D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;*/
	// create the texture
	/*if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc, nullptr, &pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating sysbuffer texture" );
	}*/

	/*D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;*/
	// create the resource view on the texture
	/*if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
														&srvDesc, &pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating view on sysBuffer texture" );
	}*/


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	/*if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating pixel shader" );
	}*/


	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	/*if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating vertex shader" );
	}*/


	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	/*const FSQVertex vertices[] =
	{
		{ -1.0f, 1.0f, 0.5f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.5f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.5f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 0.5f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 0.5f, 1.0f, 1.0f },
		{ -1.0f, -1.0f, 0.5f, 0.0f, 1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd, &initData, &pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating vertex buffer" );
	}*/


	//////////////////////////////////////////
	// create input layout for fullscreen quad
	/*const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};*/

	// Ignore the intellisense error "namespace has no member"
	/*if( FAILED( hr = pDevice->CreateInputLayout( ied, 2,
												 FramebufferShaders::FramebufferVSBytecode,
												 sizeof( FramebufferShaders::FramebufferVSBytecode ),
												 &pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating input layout" );
	}*/


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	/*D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc, &pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating sampler state" );
	}*/
}

void Graphics::InitDirect2D()
{
	ComPtr<IDXGIDevice> pDxgiDevice;
	HRESULT hr = S_OK;

	if( FAILED( hr = pDevice.As( &pDxgiDevice ) ) )
	{
		throw CHILI_GFX_EXCEPTION( 
			hr, TEXT( "Failed to get DXGI device." ) );
	}

	if( FAILED( hr = D2D1CreateDevice( pDxgiDevice.Get(), nullptr, &pDevice2d) ) )
	{
		throw CHILI_GFX_EXCEPTION( 
			hr, TEXT( "Failed to create Direct2D device." ) );
	}

	if( FAILED( hr = pDevice2d->CreateDeviceContext( 
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pContext2d ) ) )
	{
		throw CHILI_GFX_EXCEPTION( 
			hr, TEXT( "Failed to create Direct2D device context." ) );
	}

	ComPtr<ID3D11Texture2D> pBackbuffer;
	if( FAILED( hr = pSwapChain->GetBuffer( 0, IID_PPV_ARGS( &pBackbuffer ) ) ) )
	{
		throw CHILI_GFX_EXCEPTION(
			hr, TEXT( "Failed to get back buffer from swapchain." ) );
	}

	ComPtr<IDXGISurface> pSurface;	
	if( FAILED( hr = pBackbuffer.As( &pSurface ) ) )
	{
		throw CHILI_GFX_EXCEPTION( 
			hr, TEXT( "Failed to get DXGI surface from backbuffer texture." ) );
	}

	if( FAILED( ( hr = pContext2d->CreateBitmapFromDxgiSurface( 
		pSurface.Get(), nullptr, &pRenderTarget2d ) ) ) )
	{
		throw CHILI_GFX_EXCEPTION( 
			hr, TEXT( "Failed to create Direct2D render target." ) );
	}

	pContext2d->SetTarget( pRenderTarget2d.Get() );

	if( FAILED( hr = pContext2d->CreateSolidColorBrush( 
		D2D1::ColorF( 1.f, 1.f, 1.f ), &pBrush2d ) ) )
	{
		throw CHILI_GFX_EXCEPTION(
			hr, TEXT( "Failed to create Direct2D solid color brush." ) );
	}

	
}

D2D1_COLOR_F Graphics::ConvertColor( Color C )
{
	return D2D1::ColorF( C.dword );
}

void Graphics::EndFrame2D()
{
	const auto hr = pContext2d->EndDraw();
	if( hr != S_OK )
	{
		int a = 0;
	}
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	/*if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}*/
	// setup parameters for copy operation
	/*Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData );
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	const size_t srcPitch = Graphics::ScreenWidth;
	const size_t rowBytes = srcPitch * sizeof( Color );*/
	// perform the copy line-by-line
	/*for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
	{
		memcpy( &pDst[ y * dstPitch ],&pSysBuffer[y * srcPitch],rowBytes );
	}*/
	// release the adapter memory
	//pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	/*pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );*/

	EndFrame2D();

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
		}
	}
}

void Graphics::BeginFrame()
{
	BeginFrame2D();
	// clear the sysbuffer
	//memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::BeginFrame2D()
{
	pContext2d->BeginDraw();
	pContext2d->Clear();
}

void Graphics::PutPixel( int X, int Y, int R, int G, int B )
{
	const Vec2 point = {
		static_cast< float >( X ),
		static_cast< float >( Y )
	};
	const Color c = {
		static_cast< unsigned char >( R ),
		static_cast< unsigned char >( G ),
		static_cast< unsigned char >( B )
	};
	PutPixel( point, c );
}

void Graphics::PutPixel( const Vec2 & Point, Color C )
{
	assert( Point.x >= 0.f );
	assert( Point.x < ScreenWidth );
	assert( Point.y >= 0.f );
	assert( Point.y < ScreenHeight );

	DrawLine( Point, { Point.x + 1.f, Point.y }, C );
}

void Graphics::OutlineRect( float Left, float Top, float Right, float Bottom, Color C )
{
	OutlineRect( { Left, Top, Right, Bottom }, C );
}

void Graphics::OutlineRect( const RectF & Rect, Color C )
{
	pBrush2d->SetColor( D2D1::ColorF( C.dword ) );
	pContext2d->DrawRectangle( Rect, pBrush2d.Get() );
}

void Graphics::FillRect( float x0, float y0, float x1, float y1, Color c )
{
	if( x0 > x1 )std::swap( x0, x1 );
	if( y0 > y1 )std::swap( y0, y1 );

	FillRect( { x0, y0, x1, y1 }, c );
}

void Graphics::FillRect( const RectF & rect, Color c )
{
	pBrush2d->SetColor( ConvertColor( c.dword ) );
	pContext2d->FillRectangle( rect, pBrush2d.Get() );
}

void Graphics::OutlineCircle( float X, float Y, float Radius, Color C )
{
	OutlineCircle( { X, Y }, Radius, C );
}

void Graphics::OutlineCircle( const Vec2 & Center, float Radius, Color C )
{
	pBrush2d->SetColor( ConvertColor( C.dword ) );
	pContext2d->DrawEllipse( { Center, Radius, Radius }, pBrush2d.Get() );
}

void Graphics::FillCircle( float x, float y, float radius, Color c )
{
	FillCircle( { x, y }, radius, c );	
}

void Graphics::FillCircle( const Vec2 & Center, float Radius, Color C )
{
	pBrush2d->SetColor( ConvertColor( C.dword ) );
	pContext2d->FillEllipse( {Center, Radius, Radius }, pBrush2d.Get() );
}

void Graphics::DrawLine( float X0, float Y0, float X1, float Y1, Color C )
{
	if( X0 > X1 )std::swap( X0, X1 );
	if( Y0 > Y1 )std::swap( Y0, Y1 );

	DrawLine( { X0, Y0 }, { X1, Y1 }, C );
}

void Graphics::DrawLine( const Vec2 & P0, const Vec2 & P1, Color C )
{
	pBrush2d->SetColor( ConvertColor( C.dword ) );
	pContext2d->DrawLine( P0, P1, pBrush2d.Get() );
}


//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}