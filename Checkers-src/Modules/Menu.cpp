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

#include "../Headers/A_Includes.hpp"
#include "../Headers/Globals.hpp"
#include "../Headers/Utility.hpp"
#include "../Headers/GameLogic.hpp"
#include "../Headers/FileSystem.hpp"
#include "../Headers/Menu.hpp"

// Funkcja wyœwietlaj¹ca menu
void Menu_render()
{
	// Przygotowanie
	SDL_SetRenderDrawColor(g_Renderer, COLOR_DARKRED.r, COLOR_DARKRED.g, COLOR_DARKRED.b, COLOR_DARKRED.a); // Ustaw kolor t³a
	SDL_RenderClear(g_Renderer); // Wyczyœæ ekran

	// Renderowanie
	Menu_draw();
}

// Funkcja rysuj¹ca menu
void Menu_draw()
{
	// Narysuj tytu³ oraz etykiety
	if (g_Lang == "pol")
		Draw_text("WARCABY", 1600, 210, "top-center", g_Font_Title, COLOR_WHITE, false);
	else
		Draw_text("CHECKERS", 1600, 210, "top-center", g_Font_Title, COLOR_WHITE, false);

	// Tekst przycisków
	vector<string> Menu_options(0);
	if (g_Lang == "pol")
		Menu_options = {"Gracz vs Gracz", "Gracz vs Komputer", "Ustawienia", "Autorzy", "Wyjœcie"};
	else
		Menu_options = {"Player vs Player", "Player vs Computer", "Settings", "Credits", "Exit"};

	// Zmienne
	g_MenuButtons.resize(Menu_options.size());
	int buttonWidth = 750;
	int buttonHeight = 115;
	int buttonSpacing = 25;
	int startY = 750;
	int startX = (3200-buttonWidth) / 2;

	for (int i=0; i<g_MenuButtons.size(); i++)
	{
		// Nadaj w³aœciowoœci
		g_MenuButtons[i].frame = {startX, startY + i * (buttonHeight+buttonSpacing), buttonWidth, buttonHeight};
		g_MenuButtons[i].rect = {startX + 10, startY + i*(buttonHeight+buttonSpacing) + 10, buttonWidth - 10, buttonHeight - 10};
		g_MenuButtons[i].text = Menu_options[i];

		// Narysuj przycisk
		Draw_button(g_MenuButtons[i]);
	}
}

// Funkcja obs³uguj¹ca eventy w menu
void Menu_event(SDL_Event event, bool &quit, bool &error)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) // Klikanie
	{
		int x = event.button.x;
		int y = event.button.y;
		SDL_Point Point={x, y};

		for (int i=0; i<g_MenuButtons.size(); i++)
		{
			if (SDL_PointInRect(&Point, &g_MenuButtons[i].frame) == true)
			{
				if (DEVELOPER_MODE == true)
					cout<<"Clicked on: '"<<g_MenuButtons[i].text<<"'"<<endl;

				// Wys³anie OnLoad Event jeœli konieczne
				SDL_Event CustomEvent;
				CustomEvent.type = SDL_MOUSEMOTION;
				CustomEvent.motion.timestamp = SDL_GetTicks();
				CustomEvent.motion.x = x;
				CustomEvent.motion.y = y;

				if (i == 0)			// "Player vs Player"
				{
					g_VersusAI = false;
					Load_game_scene();
					g_Scene = "game";
					SDL_PushEvent(&CustomEvent);
				}
				else if (i == 1)	// "Player vs Computer"
				{
					g_VersusAI = true;
					Load_game_scene();
					g_Scene = "game";
					SDL_PushEvent(&CustomEvent);
				}
				else if (i == 2)	// "Settings"
				{
					g_Scene = "settings";
					SDL_PushEvent(&CustomEvent);
				}
				else if (i == 3)	// "Credits"
				{
					g_Scene = "credits";
					SDL_PushEvent(&CustomEvent);
				}
				else if (i == 4)	// "Exit"
				{
					quit = true;
				}

				break;
			}
		}
	}
	else if (event.type == SDL_MOUSEMOTION) // Rodzaj kursora
	{
		int x = event.motion.x;
		int y = event.motion.y;
		SDL_Point Point={x, y};
		bool isMouseOverButton=false;

		for (int i=0; i<g_MenuButtons.size(); i++)
			g_MenuButtons[i].hovered = false;

		for (int i=0; i<g_MenuButtons.size(); i++)
		{
			if (SDL_PointInRect(&Point, &g_MenuButtons[i].frame) == true)
			{
				g_MenuButtons[i].hovered = true;
				isMouseOverButton = true;
				break;
			}
		}

		if (isMouseOverButton == false)
			SDL_SetCursor(g_DefaultCursor);
		else
			SDL_SetCursor(g_HandCursor);
	}
}

