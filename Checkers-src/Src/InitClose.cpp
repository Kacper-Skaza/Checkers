/*
	Copyright 2024 Kacper Skaza

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "../Include/Z_Includes.hpp"
#include "../Include/Globals.hpp"
#include "../Include/InitClose.hpp"

// Funkcja inicjalizuj¹ca SDL2
int Init_SDL()
{
	// Ustawienia pocz¹tkowe
	SDL_SetHint(SDL_HINT_APP_NAME, GAME_NAME.c_str());
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "0");
	SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	// Podstawy
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		string txt = string("Failed to initialize SDL:\n") + SDL_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, txt.c_str(), txt.c_str(), NULL);
		return 11;
	}
	if (TTF_Init() != 0)
	{
		string txt = string("Failed to initialize TTF:\n") + TTF_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, txt.c_str(), txt.c_str(), NULL);
		return 12;
	}

	// Rozdzielczoœci wyœwietlacza
	if (SDL_GetDesktopDisplayMode(0, &g_DisplayMode) != 0)
	{
		string txt = string("Failed to retrieve display resolution:\n") + SDL_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, txt.c_str(), txt.c_str(), NULL);
		return 21;
	}
	if (DEVELOPER_MODE == true)
		cout<<"Desktop Display Mode: "<<g_DisplayMode.w<<"x"<<g_DisplayMode.h<<" @ "<<g_DisplayMode.refresh_rate<<"Hz"<<endl;
	// Okno gry
	g_Window = SDL_CreateWindow(GAME_NAME.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (g_DisplayMode.w*5) / 6, (g_DisplayMode.h*5) / 6,
								SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!g_Window)
	{
		string txt = string("Failed to create Window:\n") + SDL_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, txt.c_str(), txt.c_str(), NULL);
		return 22;
	}
	// Renderer
	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (!g_Renderer)
	{
		string txt = string("Failed to create Renderer:\n") + SDL_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, txt.c_str(), txt.c_str(), NULL);
		return 23;
	}
	SDL_SetRenderDrawBlendMode(g_Renderer, SDL_BLENDMODE_BLEND);	// Przezroczystoœæ
	SDL_RenderSetLogicalSize(g_Renderer, 3200, 1800);				// Rozmiar logiczny

	// Kursory
	g_DefaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	g_HandCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	if (!g_DefaultCursor || !g_HandCursor)
	{
		string txt = string("Failed to load cursor:\n") + SDL_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, txt.c_str(), txt.c_str(), NULL);
		return 31;
	}
	// Czcionki
	g_Font_36 = TTF_OpenFont("Game_data/Fonts/Roboto/Roboto-Medium.ttf", 36);
	g_Font_72 = TTF_OpenFont("Game_data/Fonts/Roboto/Roboto-Medium.ttf", 72);
	g_Font_144 = TTF_OpenFont("Game_data/Fonts/Roboto/Roboto-Medium.ttf", 144);
	g_Font_Title = TTF_OpenFont("Game_data/Fonts/Varsity-Regular/Varsity-Regular.ttf", 288);
	if (!g_Font_36 || !g_Font_72 || !g_Font_144 || !g_Font_Title)
	{
		string txt = string("Failed to load fonts:\n") + TTF_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, txt.c_str(), txt.c_str(), NULL);
		return 32;
	}

	return 0;
}

// Funkcja zwolniaj¹ca zasoby SDL2
void Close_SDL()
{
	TTF_CloseFont(g_Font_Title);
	TTF_CloseFont(g_Font_144);
	TTF_CloseFont(g_Font_72);
	TTF_CloseFont(g_Font_36);
	SDL_FreeCursor(g_HandCursor);
	SDL_FreeCursor(g_DefaultCursor);
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
	TTF_Quit();
	SDL_Quit();
}

