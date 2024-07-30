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

/*
	Flagi konsolidatora wymagane do kompilacji:
		-lSDL2 -lSDL2_ttf
*/

// Modu³y ogólne
#include "Headers/A_Includes.hpp"	// Biblioteki, w tym SDL2
#include "Headers/Globals.hpp"		// Sta³e i zmienne globalne
#include "Headers/Utility.hpp"		// Funkcje pomocnicze i Debug()
#include "Headers/GameLogic.hpp"	// Logika warcabów
#include "Headers/FileSystem.hpp"	// Zarz¹dzanie zapisami i innymi plikami

// Modu³y renderuj¹ce
#include "Headers/Game.hpp"
#include "Headers/Menu.hpp"
#include "Headers/Settings.hpp"
#include "Headers/Credits.hpp"



// Funkcja inicjalizuj¹ca SDL2
int Init_SDL()
{
	// Podstawy
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout<<"Nie uda³o siê zainicjowaæ SDL: "<<SDL_GetError()<<endl;
		return 11;
	}
	if (TTF_Init() != 0)
	{
		cout<<"Nie uda³o siê zainicjowaæ TTF: "<<TTF_GetError()<<endl;
		return 12;
	}

	// Okno gry
	g_Window = SDL_CreateWindow(GAME_NAME.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!g_Window)
	{
		cout<<"Nie uda³o siê utworzyæ okna: "<<SDL_GetError()<<endl;
		return 21;
	}
	// Renderer
	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (!g_Renderer)
	{
		cout<<"Nie uda³o siê utworzyæ renderera: "<<SDL_GetError()<<endl;
		return 22;
	}
	SDL_SetRenderDrawBlendMode(g_Renderer, SDL_BLENDMODE_BLEND);	// Przezroczystoœæ
	SDL_RenderSetLogicalSize(g_Renderer, 3200, 1800);				// Rozmiar logiczny

	// Kursory
	g_DefaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	g_HandCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	if (!g_DefaultCursor || !g_HandCursor)
	{
		cout<<"Nie uda³o siê za³adowaæ kursora: "<<SDL_GetError()<<endl;
		return 31;
	}
	// Czcionki
	g_Font_36 = TTF_OpenFont("Fonts/Roboto/Roboto-Medium.ttf", 36);
	g_Font_72 = TTF_OpenFont("Fonts/Roboto/Roboto-Medium.ttf", 72);
	g_Font_144 = TTF_OpenFont("Fonts/Roboto/Roboto-Medium.ttf", 144);
	g_Font_Title = TTF_OpenFont("Fonts/Varsity-Regular/Varsity-Regular.ttf", 288);
	if (!g_Font_36 || !g_Font_72 || !g_Font_144 || !g_Font_Title)
	{
		cout<<"Nie uda³o siê za³adowaæ czcionki: "<<TTF_GetError()<<endl;
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



// MAIN
int main()
{
	// Ustawienia pocz¹tkowe SDL2
	SDL_SetHint(SDL_HINT_APP_NAME, GAME_NAME.c_str());
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "0");
	SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	// Inicjalizacja SDL2 i systemu plików
	if (Init_SDL() != 0 || Init_settings() != 0)
	{
		Close_SDL();
		return 1;
	}

	// Zmienne lokalne
	int Last_Row=0, Last_Col=0;

	// Obs³uga FPS
	long long Frame_Start=0, Frame_Delay=0;
	long long Timer_Start=0, Fps_Counter=0;
	string Fps_Display="0 FPS";

	// Obs³uga pêtli g³ównej
	bool quit=false, error=false;
	SDL_Event event;

	// Pêtla g³ówna
	while (!quit && !error)
	{
		// FPS
		Frame_Start = SDL_GetTicks();
		Fps_Counter++;

		// Event
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else
			{
				if (g_Scene == "game")
					Game_event(event, quit, error, Last_Row, Last_Col);
				else if (g_Scene == "menu")
					Menu_event(event, quit, error);
				else if (g_Scene == "settings")
					Settings_event(event, quit, error);
				else if (g_Scene == "credits")
					Credits_event(event, quit, error);
				else
					error = true;
			}

			g_RenderFrame = true;
		}

		// Oblicz FPS
		if (Timer_Start < SDL_GetTicks()-1000 || (Fps_Counter >= g_TargetFPS && DEVELOPER_MODE == false))
		{
			Timer_Start = SDL_GetTicks();
			Fps_Display = to_string(Fps_Counter) + " FPS";
			Fps_Counter = 0;
			g_RenderFrame = true;
		}

		// Funkcja wyœwietlaj¹ca
		if (g_RenderFrame == true || g_TargetFPS == LLONG_MAX)
		{
			if (g_Scene == "game")
				Game_render();
			else if (g_Scene == "menu")
				Menu_render();
			else if (g_Scene == "settings")
				Settings_render();
			else if (g_Scene == "credits")
				Credits_render();
			else
				error = true;

			// Koñczenie
			Draw_text(Fps_Display, 3190, 10, "top-right", g_Font_36, COLOR_LIME, true);				// Narysuj FPS
			Draw_text(GAME_VERSION, 3190, 1755, "bottom-right", g_Font_36, COLOR_WHITE, false);		// Narysuj wersjê gry
			Draw_text(GAME_COPYRIGHT, 3190, 1795, "bottom-right", g_Font_36, COLOR_WHITE, false);	// Narysuj informacjê o copyright

			// Wyœwietl na ekranie i zresetuj zmienn¹
			SDL_RenderPresent(g_Renderer);
			g_RenderFrame = false;
		}

		// FPS
		Frame_Delay = floor(1000.0/g_TargetFPS) - (SDL_GetTicks()-Frame_Start);

		// OpóŸnienie
		if (Frame_Delay > 2)
			SDL_Delay(Frame_Delay-1);
		else if (Frame_Delay > 0)
			SDL_Delay(Frame_Delay);
		else
			SDL_Delay(0);
	}

	// Zwolnij zasoby SDL2
	Close_SDL();

	// SprawdŸ b³êdy
	if (error == true)
	{
		cout<<"B³êdna scena !!!"<<endl;
		return 2;
	}
	return 0;
}

