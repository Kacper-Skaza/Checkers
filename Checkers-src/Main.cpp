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
#include "Include/Z_Includes.hpp"		// Biblioteki, w tym SDL2
#include "Include/Globals.hpp"			// Sta³e i zmienne globalne
#include "Include/InitClose.hpp"		// Funkcje inicjalizuj¹ce i zamykaj¹ce
#include "Include/Utility.hpp"			// Funkcje pomocnicze i Debug()

// Modu³y gry
#include "Include/Game/GameLogic.hpp"	// Logika warcabów
#include "Include/Game/FileSystem.hpp"	// Zarz¹dzanie zapisami i innymi plikami

// Modu³y scen
#include "Include/Scene/Game.hpp"
#include "Include/Scene/Menu.hpp"
#include "Include/Scene/Settings.hpp"
#include "Include/Scene/Credits.hpp"



// MAIN
int main()
{
	// Inicjalizacja SDL2 i innych
	if (Init_SDL() != 0 || Init_settings() != 0)
	{
		Close_SDL();
		return 1;
	}

	// Zmienne lokalne
	int l_LastRow=0, l_LastCol=0;

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
					Game_event(event, quit, error, l_LastRow, l_LastCol);
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

	// Zwolnij zasoby SDL2 i zapisz ustawienia
	Close_SDL();
	Save_settings();

	// SprawdŸ b³êdy
	if (error == true)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Incorrect scene !!!", "Incorrect scene !!!", NULL);
		return 2;
	}

	return 0;
}

