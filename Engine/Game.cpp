/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	ball( Vec2( 300.0f, 300.0f ), Vec2( 100.0f, 100.0f ) ),
	walls( 0.0f, 0.0f, float( Graphics::ScreenWidth ), float( Graphics::ScreenHeight ) ),
	arial_16n( dwrite, L"Arial", 16.f ) 
{
}

void Game::Go()
{
	gfx.BeginFrame();

	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}

void Game::UpdateModel()
{
	dt = ft.Mark();
	ball.Update(dt);
	ball.DoWallCollision(walls);
}

void Game::ComposeFrame()
{
	const auto rect = RectF( 100.f, 100.f, 300.f, 136.f );
	
	gfx.FillRect( rect, Colors::White );
	ball.Draw( gfx );
	gfx.DrawString( std::to_wstring( dt ), rect, arial_16n, Colors::Red );
}
