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

#include "../../Include/Z_Includes.hpp"
#include "../../Include/Globals.hpp"
#include "../../Include/Utility.hpp"
#include "../../Include/Game/GameLogic.hpp"
#include "../../Include/Game/FileSystem.hpp"
#include "../../Include/Scene/Menu.hpp"

// Funkcja wy�wietlaj�ca menu
void Menu_render()
{
	// Przygotowanie
	SDL_SetRenderDrawColor(g_Renderer, COLOR_DARKRED.r, COLOR_DARKRED.g, COLOR_DARKRED.b, COLOR_DARKRED.a); // Ustaw kolor t�a
	SDL_RenderClear(g_Renderer); // Wyczy�� ekran

	// Renderowanie
	Menu_draw();
}

// Funkcja rysuj�ca menu
void Menu_draw()
{
	// Narysuj tytu� oraz etykiety
	if (g_Lang == "pol")
		Draw_text("WARCABY", 1600, 210, "top-center", g_Font_Title, COLOR_WHITE, false);
	else
		Draw_text("CHECKERS", 1600, 210, "top-center", g_Font_Title, COLOR_WHITE, false);

	// Tekst przycisk�w
	vector<string> ButtonsText(0);
	if (g_Lang == "pol")
		ButtonsText = {"Gracz vs Gracz", "Gracz vs Komputer", "Ustawienia", "Autorzy", "Wyj�cie"};
	else
		ButtonsText = {"Player vs Player", "Player vs Computer", "Settings", "Credits", "Exit"};

	// Zmienne przycisk�w
	g_ButtonArray.resize(ButtonsText.size());
	int buttonWidth = 750;
	int buttonHeight = 115;
	int buttonSpacing = 25;
	int startY = 750;
	int startX = (3200-buttonWidth) / 2;

	for (int i=0; i<g_ButtonArray.size(); i++)
	{
		// Nadaj w�a�ciowo�ci
		g_ButtonArray[i].frame = {startX, startY + i * (buttonHeight+buttonSpacing), buttonWidth, buttonHeight};
		g_ButtonArray[i].rect = {startX + 10, startY + i*(buttonHeight+buttonSpacing) + 10, buttonWidth - 10, buttonHeight - 10};
		g_ButtonArray[i].text = ButtonsText[i];
		g_ButtonArray[i].selected = false;

		// Narysuj przycisk
		Draw_button(g_ButtonArray[i]);
	}
}

// Funkcja obs�uguj�ca eventy w menu
void Menu_event(SDL_Event event, bool &quit, bool &error)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) // Klikanie
	{
		int x = event.button.x;
		int y = event.button.y;
		SDL_Point Point={x, y};

		for (int i=0; i<g_ButtonArray.size(); i++)
		{
			if (SDL_PointInRect(&Point, &g_ButtonArray[i].frame) == true)
			{
				if (DEVELOPER_MODE == true)
					cout<<"Clicked on: '"<<g_ButtonArray[i].text<<"'"<<endl;

				// Wys�anie OnLoad Event je�li konieczne
				SDL_Event CustomEvent;
				CustomEvent.type = SDL_MOUSEMOTION;
				CustomEvent.motion.timestamp = SDL_GetTicks();
				CustomEvent.motion.x = x;
				CustomEvent.motion.y = y;

				if (i == 0)			// "Player vs Player"
				{
					g_PlayerSI = false;
					Game_scene_load();
					g_Scene = "game";
					SDL_PushEvent(&CustomEvent);
				}
				else if (i == 1)	// "Player vs Computer"
				{
					g_PlayerSI = true;
					Game_scene_load();
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

		for (int i=0; i<g_ButtonArray.size(); i++)
		{
			if (SDL_PointInRect(&Point, &g_ButtonArray[i].frame) == true)
			{
				g_ButtonArray[i].hovered = true;
				isMouseOverButton = true;
			}
			else
			{
				g_ButtonArray[i].hovered = false;
			}
		}

		if (isMouseOverButton == false)
			SDL_SetCursor(g_DefaultCursor);
		else
			SDL_SetCursor(g_HandCursor);
	}
}

