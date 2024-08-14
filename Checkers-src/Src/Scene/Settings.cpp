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
#include "../../Include/Scene/Settings.hpp"

// Funkcja wy�wietlaj�ca ustawienia
void Settings_render()
{
	// Przygotowanie
	SDL_SetRenderDrawColor(g_Renderer, COLOR_DARKRED.r, COLOR_DARKRED.g, COLOR_DARKRED.b, COLOR_DARKRED.a); // Ustaw kolor t�a
	SDL_RenderClear(g_Renderer); // Wyczy�� ekran

	// Renderowanie
	Settings_draw();
}

// Funkcja rysuj�ca ustawienia
void Settings_draw()
{
	// Narysuj tytu� oraz etykiety
	if (g_Lang == "pol")
	{
		Draw_text("USTAWIENIA", 1600, 250, "top-center", g_Font_144, COLOR_WHITE, false);
		Draw_text("Wybierz j�zyk:", 1100, 625, "top-center", g_Font_72, COLOR_WHITE, false);
		Draw_text("Wybierz docelowe FPS:", 2100, 625, "top-center", g_Font_72, COLOR_WHITE, false);
	}
	else
	{
		Draw_text("SETTINGS", 1600, 250, "top-center", g_Font_144, COLOR_WHITE, false);
		Draw_text("Select language:", 1100, 625, "top-center", g_Font_72, COLOR_WHITE, false);
		Draw_text("Select target FPS:", 2100, 625, "top-center", g_Font_72, COLOR_WHITE, false);
	}

	// Tekst przycisk�w
	vector<string> ButtonsText(0);
	if (g_Lang == "pol")
		ButtonsText = {"English", "Polski", "30", "60", "144", "Bez limitu", "Powr�t do Menu"};
	else
		ButtonsText = {"English", "Polski", "30", "60", "144", "Unlimited", "Back to Menu"};

	// Zmienne przycisk�w
	g_ButtonArray.resize(ButtonsText.size());
	int buttonWidth = 750;
	int buttonHeight = 115;
	int buttonSpacing = 0;
	int startY = 750;
	int startX = ((3200-buttonWidth) / 2) - 500;

	// Sprawdzanie aktualnych ustawie�
	for (int i=0; i<g_ButtonArray.size(); i++)
		g_ButtonArray[i].selected = false;

	// J�zyk
	if (g_Lang == "pol")
		g_ButtonArray[1].selected = true;
	else
		g_ButtonArray[0].selected = true;

	// FPS
	if (g_TargetFPS == 30)
		g_ButtonArray[2].selected = true;
	else if (g_TargetFPS == 60)
		g_ButtonArray[3].selected = true;
	else if (g_TargetFPS == 144)
		g_ButtonArray[4].selected = true;
	else if (g_TargetFPS == LLONG_MAX)
		g_ButtonArray[5].selected = true;

	for (int i=0; i<g_ButtonArray.size(); i++)
	{
		// Zmiany pozycji
		if (i == 2)
		{
			startY -= buttonHeight*2;
			startX += 1000;
		}
		else if (i == 6)
		{
			startY += 100;
			startX -= 500;
		}

		// Nadaj w�a�ciowo�ci
		g_ButtonArray[i].frame = {startX, startY + i * (buttonHeight+buttonSpacing), buttonWidth, buttonHeight};
		g_ButtonArray[i].rect = {startX + 10, startY + i*(buttonHeight+buttonSpacing) + 10, buttonWidth - 10, buttonHeight - 10};
		g_ButtonArray[i].text = ButtonsText[i];

		// Narysuj przycisk
		Draw_button(g_ButtonArray[i]);
	}
}

// Funkcja obs�uguj�ca eventy w ustawieniach
void Settings_event(SDL_Event event, bool &quit, bool &error)
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

				if (i == 0)			// "English"
					g_Lang = "eng";
				else if (i == 1)	// "Polski"
					g_Lang = "pol";
				else if (i == 2)	// "30"
					g_TargetFPS = 30;
				else if (i == 3)	// "60"
					g_TargetFPS = 60;
				else if (i == 4)	// "144"
					g_TargetFPS = 144;
				else if (i == 5)	// "Unlimited"
					g_TargetFPS = LLONG_MAX;
				else if (i == 6)	// "Back to Menu"
				{
					Save_settings();
					g_Scene = "menu";
					SDL_PushEvent(&CustomEvent);
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

