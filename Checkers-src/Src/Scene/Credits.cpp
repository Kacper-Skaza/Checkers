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
#include "../../Include/Scene/Credits.hpp"

// Funkcja wy�wietlaj�ca credits
void Credits_render()
{
	// Przygotowanie
	SDL_SetRenderDrawColor(g_Renderer, COLOR_DARKRED.r, COLOR_DARKRED.g, COLOR_DARKRED.b, COLOR_DARKRED.a); // Ustaw kolor t�a
	SDL_RenderClear(g_Renderer); // Wyczy�� ekran

	// Renderowanie
	Credits_draw();
}

// Funkcja rysuj�ca credits
void Credits_draw()
{
	// Narysuj tytu� oraz etykiety
	if (g_Lang == "pol")
		Draw_text("AUTORZY", 1600, 250, "top-center", g_Font_144, COLOR_WHITE, false);
	else
		Draw_text("CREDITS", 1600, 250, "top-center", g_Font_144, COLOR_WHITE, false);

	// Credits
	vector<string> CreditsText;
	if (g_Lang == "pol")
	{
		CreditsText =
		{
			"Autor:",
			"Jestem studentem informatyki, pasjonatem gier komputerowych",
			"i programowania. Projekt warcab�w zosta� stworzony jako",
			"cz�� mojego portfolio, celem demonstracji",
			"umiej�tno�ci programistycznych i pomys�owo�ci.",
			" ",
			"Opis projektu:",
			"Gra w warcaby zosta�a zaprojektowana jako projekt do CV, maj�cy",
			"na celu nie tylko zabaw�, ale r�wnie� pokazanie zdolno�ci",
			"w projektowaniu i implementacji oprogramowania. Projekt powsta�",
			"w ramach samodzielnych prac, rozwijaj�cych umiej�tno�ci",
			"programowania gier oraz projektowania interfejsu u�ytkownika.",
			" ",
			" ",
			GAME_COPYRIGHT,
			"Apache License 2.0",
			"Build " + GAME_VERSION + "." + to_string(VER_BUILD)
		};
	}
	else
	{
		CreditsText =
		{
			"Author:",
			"I am a computer science student, passionate about computer games",
			"and programming. The checkers project was created as",
			"part of my portfolio, aiming to demonstrate",
			"programming skills and creativity.",
			" ",
			"Project description:",
			"Checkers game was designed as a CV project, aiming",
			"not only for entertainment, but also to showcase abilities",
			"in software design and implementation. The project was",
			"developed independently, enhancing skills",
			"in game programming and user interface design.",
			" ",
			" ",
			GAME_COPYRIGHT,
			"Apache License 2.0",
			"Build " + GAME_VERSION + "." + to_string(VER_BUILD)
		};
	}
	for (int i=0; i<CreditsText.size(); i++)
		Draw_text(CreditsText[i], 1600, 450+50*i, "top-center", g_Font_36, COLOR_WHITE, false);

	// Tekst przycisk�w
	vector<string> ButtonsText(0);
	if (g_Lang == "pol")
		ButtonsText = {"Powr�t do Menu"};
	else
		ButtonsText = {"Back to Menu"};

	// Zmienne przycisk�w
	g_ButtonArray.resize(ButtonsText.size());
	int buttonWidth = 750;
	int buttonHeight = 115;
	int buttonSpacing = 25;
	int startY = 1310;
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

// Funkcja obs�uguj�ca eventy w credits
void Credits_event(SDL_Event event, bool &quit, bool &error)
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

				if (i == 0)		// "Back to Menu"
				{
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

