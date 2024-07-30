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
#include "../Headers/Credits.hpp"

// Funkcja wyœwietlaj¹ca credits
void Credits_render()
{
	// Przygotowanie
	SDL_SetRenderDrawColor(g_Renderer, COLOR_DARKRED.r, COLOR_DARKRED.g, COLOR_DARKRED.b, COLOR_DARKRED.a); // Ustaw kolor t³a
	SDL_RenderClear(g_Renderer); // Wyczyœæ ekran

	// Renderowanie
	Credits_draw();
}

// Funkcja rysuj¹ca credits
void Credits_draw()
{
	// Narysuj tytu³ oraz etykiety
	if (g_Lang == "pol")
		Draw_text("AUTORZY", 1600, 250, "top-center", g_Font_144, COLOR_WHITE, false);
	else
		Draw_text("CREDITS", 1600, 250, "top-center", g_Font_144, COLOR_WHITE, false);

	// Credits
	vector<string> Credits_txt;
	if (g_Lang == "pol")
	{
		Credits_txt =
		{
			"Autor:",
			"Jestem studentem informatyki, pasjonatem gier komputerowych",
			"i programowania. Projekt warcabów zosta³ stworzony jako",
			"czêœæ mojego portfolio, celem demonstracji",
			"umiejêtnoœci programistycznych i pomys³owoœci.",
			" ",
			"Opis projektu:",
			"Gra w warcaby zosta³a zaprojektowana jako projekt do CV, maj¹cy",
			"na celu nie tylko zabawê, ale równie¿ pokazanie zdolnoœci",
			"w projektowaniu i implementacji oprogramowania. Projekt powsta³",
			"w ramach samodzielnych prac, rozwijaj¹cych umiejêtnoœci",
			"programowania gier oraz projektowania interfejsu u¿ytkownika.",
			" ",
			" ",
			GAME_COPYRIGHT,
			"Apache License 2.0"
		};
	}
	else
	{
		Credits_txt =
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
			"Apache License 2.0"
		};
	}
	for (int i=0; i<Credits_txt.size(); i++)
		Draw_text(Credits_txt[i], 1600, 475+50*i, "top-center", g_Font_36, COLOR_WHITE, false);

	// Tekst przycisków
	vector<string> Menu_options(0);
	if (g_Lang == "pol")
		Menu_options = {"Powrót do Menu"};
	else
		Menu_options = {"Back to Menu"};

	// Zmienne
	g_CreditsButtons.resize(Menu_options.size());
	int buttonWidth = 750;
	int buttonHeight = 115;
	int buttonSpacing = 25;
	int startY = 1310;
	int startX = (3200-buttonWidth) / 2;

	for (int i=0; i<g_CreditsButtons.size(); i++)
	{
		// Nadaj w³aœciowoœci
		g_CreditsButtons[i].frame = {startX, startY + i * (buttonHeight+buttonSpacing), buttonWidth, buttonHeight};
		g_CreditsButtons[i].rect = {startX + 10, startY + i*(buttonHeight+buttonSpacing) + 10, buttonWidth - 10, buttonHeight - 10};
		g_CreditsButtons[i].text = Menu_options[i];

		// Narysuj przycisk
		Draw_button(g_CreditsButtons[i]);
	}
}

// Funkcja obs³uguj¹ca eventy w credits
void Credits_event(SDL_Event event, bool &quit, bool &error)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) // Klikanie
	{
		int x = event.button.x;
		int y = event.button.y;
		SDL_Point Point={x, y};

		for (int i=0; i<g_CreditsButtons.size(); i++)
		{
			if (SDL_PointInRect(&Point, &g_CreditsButtons[i].frame) == true)
			{
				if (DEVELOPER_MODE == true)
					cout<<"Clicked on: '"<<g_CreditsButtons[i].text<<"'"<<endl;

				// Wys³anie OnLoad Event jeœli konieczne
				SDL_Event CustomEvent;
				CustomEvent.type = SDL_MOUSEMOTION;
				CustomEvent.motion.timestamp = SDL_GetTicks();
				CustomEvent.motion.x = x;
				CustomEvent.motion.y = y;

				if (i == 0)	// "Back to Menu"
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

		for (int i=0; i<g_CreditsButtons.size(); i++)
			g_CreditsButtons[i].hovered = false;

		for (int i=0; i<g_CreditsButtons.size(); i++)
		{
			if (SDL_PointInRect(&Point, &g_CreditsButtons[i].frame) == true)
			{
				g_CreditsButtons[i].hovered = true;
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

