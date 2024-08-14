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
#include "../../Include/Scene/Game.hpp"

// Funkcja wyœwietlaj¹ca grê
void Game_render()
{
	// Przygotowanie
	SDL_SetRenderDrawColor(g_Renderer, COLOR_DARKRED.r, COLOR_DARKRED.g, COLOR_DARKRED.b, COLOR_DARKRED.a); // Ustaw kolor t³a
	SDL_RenderClear(g_Renderer); // Wyczyœæ ekran

	// Renderowanie
	Game_draw_board();		// Narysuj planszê
	Game_draw_pieces();		// Narysuj pionki
	Game_draw_markers();	// Narysuj znaczniki
	Game_draw_UI();			// Narysuj interfejs
}

// Funkcja rysuj¹ca planszê
void Game_draw_board()
{
	// Narysuj szary kwadrat (wraz z ramk¹)
	SDL_SetRenderDrawColor(g_Renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
	SDL_Rect frame = {CELL_OFFSET-10, CELL_OFFSET-10, CELL_SIZE*CELL_NUM+20, CELL_SIZE*CELL_NUM+20};
	SDL_RenderFillRect(g_Renderer, &frame);

	// Narysuj bia³e pola
	SDL_SetRenderDrawColor(g_Renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
	for (int row=0; row<CELL_NUM; row++)
	{
		for (int col=0; col<CELL_NUM; col++)
		{
			if ((row+col)%2 == 0) // Jeœli pole jest bia³e
			{
				SDL_Rect rect = {col*CELL_SIZE + CELL_OFFSET, row*CELL_SIZE + CELL_OFFSET, CELL_SIZE, CELL_SIZE};
				SDL_RenderFillRect(g_Renderer, &rect);
			}
		}
	}
}

// Funkcja rysuj¹ca pionki
void Game_draw_pieces()
{
	int radius = (CELL_SIZE*4) / 10;

	for (int row=0; row<CELL_NUM; row++)
	{
		for (int col=0; col<CELL_NUM; col++)
		{
			if (g_Board[row][col] != 0)
			{
				// Oblicz pozycjê
				int x = col*CELL_SIZE + CELL_SIZE/2 + CELL_OFFSET;
				int y = row*CELL_SIZE + CELL_SIZE/2 + CELL_OFFSET;

				// Narysuj pionka
				Draw_piece(g_Board[row][col], x, y, radius, true);
			}
		}
	}
}

// Funkcja rysuj¹ca znaczniki
void Game_draw_markers()
{
	// Rysowanie znacznika wybranego pola
	SDL_SetRenderDrawColor(g_Renderer, COLOR_LIME.r, COLOR_LIME.g, COLOR_LIME.b, COLOR_LIME.a);
	if (g_SelectedRow != -1 && g_SelectedCol != -1)
	{
		int x = g_SelectedCol*CELL_SIZE + CELL_OFFSET;
		int y = g_SelectedRow*CELL_SIZE + CELL_OFFSET;

		Draw_empty_rect(x-5, y-5, CELL_SIZE+10, CELL_SIZE+10, 12);
	}

	// Rysowanie znacznika bicia
	SDL_SetRenderDrawColor(g_Renderer, COLOR_ORANGE.r, COLOR_ORANGE.g, COLOR_ORANGE.b, COLOR_ORANGE.a);
	for (int row=0; row<CELL_NUM; row++)
	{
		for (int col=0; col<CELL_NUM; col++)
		{
			if (g_Turn%2 == 0) // Biali
			{
				if (g_WhiteCaptures[row][col] == 1)
				{
					int x = col*CELL_SIZE + CELL_OFFSET;
					int y = row*CELL_SIZE + CELL_OFFSET;

					Draw_empty_rect(x-5, y-5, CELL_SIZE+10, CELL_SIZE+10, 12);
				}
			}
			else // Czerwoni
			{
				if (g_RedCaptures[row][col] == 1)
				{
					int x = col*CELL_SIZE + CELL_OFFSET;
					int y = row*CELL_SIZE + CELL_OFFSET;

					Draw_empty_rect(x-5, y-5, CELL_SIZE+10, CELL_SIZE+10, 12);
				}
			}
		}
	}

	// Rysowanie znacznika mo¿liwego ruchu
	SDL_SetRenderDrawColor(g_Renderer, COLOR_LIME.r, COLOR_LIME.g, COLOR_LIME.b, COLOR_LIME.a);
	for (int row=0; row<CELL_NUM; row++)
	{
		for (int col=0; col<CELL_NUM; col++)
		{
			if (g_SelectedMoves[row][col] >= 5 && g_SelectedMoves[row][col] <= 8)
			{
				int x = col*CELL_SIZE + CELL_SIZE/2 + CELL_OFFSET;
				int y = row*CELL_SIZE + CELL_SIZE/2 + CELL_OFFSET;
				int radius = CELL_SIZE / 10;

				Draw_filled_circle(x, y, radius);
			}
		}
	}
}

// Funkcja rysuj¹ca interfejs
void Game_draw_UI()
{
	// Rysowanie nazw graczy i pola statystyk na prostok¹tach (przyciskach) :P
	vector<Button> UI_Rect(0);

	// Tekst pól
	vector<string> UI_Text(0);
	if (g_Lang == "pol")
	{
		if (g_PlayerSI == true)
			UI_Text = {"Gracz", "Komputer", " "};
		else
			UI_Text = {"Gracz #1", "Gracz #2", " "};
	}
	else
	{
		if (g_PlayerSI == true)
			UI_Text = {"Player", "Computer", " "};
		else
			UI_Text = {"Player #1", "Player #2", " "};
	}

	// Zmienne pól
	UI_Rect.resize(UI_Text.size());
	int buttonWidth = 500;
	int buttonHeight = 150;
	int buttonSpacing = 0;
	int startY = 100;
	int startX = 1900;

	// W³aœciwoœci pól
	UI_Rect[0].selected = false;
	UI_Rect[1].selected = false;
	UI_Rect[2].selected = true;
	if (g_Turn >= 0)
	{
		if (g_Turn%2 == 0)
			UI_Rect[0].selected = true;
		else
			UI_Rect[1].selected = true;
	}

	for (int i=0; i<UI_Rect.size(); i++)
	{
		// Zmiany pozycji
		if (i == 1)
		{
			startX = 2500;
		}
		else if (i == 2)
		{
			buttonWidth = 1100;
			buttonHeight = 855;
			startY = 350;
			startX = 1900;
		}

		// Nadaj w³aœciowoœci
		UI_Rect[i].frame = {startX, startY, buttonWidth, buttonHeight};
		UI_Rect[i].rect = {startX + 10, startY + 10, buttonWidth - 20, buttonHeight - 20};
		UI_Rect[i].text = UI_Text[i];
		string txt=UI_Text[i].c_str();

		// Narysuj pole
		if (UI_Rect[i].selected == true)
		{
			// Narysuj prostok¹ty
			SDL_SetRenderDrawColor(g_Renderer, COLOR_SHADOW.r, COLOR_SHADOW.g, COLOR_SHADOW.b, COLOR_SHADOW.a);
			SDL_RenderFillRect(g_Renderer, &UI_Rect[i].frame);
			SDL_SetRenderDrawColor(g_Renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
			SDL_RenderFillRect(g_Renderer, &UI_Rect[i].rect);

			// Narysuj tekst
			Draw_text(txt, UI_Rect[i].rect.x + UI_Rect[i].rect.w/2, UI_Rect[i].rect.y + UI_Rect[i].rect.h/2, "center-center", g_Font_72, COLOR_BLACK, false);
		}
		else
		{
			// Narysuj prostok¹ty
			SDL_SetRenderDrawColor(g_Renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
			SDL_RenderFillRect(g_Renderer, &UI_Rect[i].frame);
			SDL_SetRenderDrawColor(g_Renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
			SDL_RenderFillRect(g_Renderer, &UI_Rect[i].rect);

			// Narysuj tekst
			Draw_text(txt, UI_Rect[i].rect.x + UI_Rect[i].rect.w/2, UI_Rect[i].rect.y + UI_Rect[i].rect.h/2, "center-center", g_Font_72, COLOR_GRAY, false);
		}
	}

	// Rysowanie znaczników koloru graczy
	SDL_SetRenderDrawColor(g_Renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
	SDL_Rect temp_rect = {1910, 110, 50, 50};
	SDL_RenderFillRect(g_Renderer, &temp_rect);
	Draw_piece(1, temp_rect.x + temp_rect.w/2, temp_rect.y + temp_rect.h/2, 20, false);
	SDL_SetRenderDrawColor(g_Renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
	temp_rect.x = 2510;
	SDL_RenderFillRect(g_Renderer, &temp_rect);
	Draw_piece(-1, temp_rect.x + temp_rect.w/2, temp_rect.y + temp_rect.h/2, 20, false);

	// Rysowanie tekstu w polu g³ównym
	string Score = " " + to_string(g_Score.first) + " : " + to_string(g_Score.second) + " ";
	Draw_text(Score, 2450, 375, "top-center", g_Font_72, COLOR_WHITE, true);
	for (int i=0; i<g_MainFieldText.size(); i++)
		Draw_text(g_MainFieldText[i], 1925, 525+50*i, "bottom-left", g_Font_36, g_MainFieldColor[i], false);

	// Tekst przycisków
	vector<string> ButtonsText(0);
	if (g_Lang == "pol")
		ButtonsText = {"Nowa gra", "Zapisz grê", "Wczytaj grê", "Powrót do Menu"};
	else
		ButtonsText = {"New game", "Save game", "Load game", "Back to Menu"};

	// Zmienne przycisków
	g_ButtonArray.resize(ButtonsText.size());
	buttonWidth = 1000;
	buttonHeight = 115;
	buttonSpacing = 25;
	startY = 1305;
	startX = (3200-buttonWidth+1700) / 2;

	for (int i=0; i<g_ButtonArray.size(); i++)
	{
		// Zmiany pozycji
		if (i == 1)
		{
			buttonWidth -= 510;
		}
		else if (i == 2)
		{
			startY -= buttonHeight + buttonSpacing;
			startX += buttonWidth + 20;
		}
		else if (i == 3)
		{
			startX -= buttonWidth + 20;
			buttonWidth += 510;
		}

		// Nadaj w³aœciowoœci
		g_ButtonArray[i].frame = {startX, startY + i * (buttonHeight+buttonSpacing), buttonWidth, buttonHeight};
		g_ButtonArray[i].rect = {startX + 10, startY + i*(buttonHeight+buttonSpacing) + 10, buttonWidth - 10, buttonHeight - 10};
		g_ButtonArray[i].text = ButtonsText[i];
		g_ButtonArray[i].selected = false;

		// Narysuj przycisk
		Draw_button(g_ButtonArray[i]);
	}
}

// Funkcja obs³uguj¹ca eventy w grze
void Game_event(SDL_Event event, bool &quit, bool &error, int &l_LastRow, int &l_LastCol)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) // Klikanie
	{
		int x = event.button.x;
		int y = event.button.y;
		SDL_Point Point={x, y};

		if (event.button.button == SDL_BUTTON_LEFT && y-CELL_OFFSET >= 0 && x-CELL_OFFSET >= 0) // Lewy przycisk myszy
		{
			int row = (y-CELL_OFFSET) / CELL_SIZE;
			int col = (x-CELL_OFFSET) / CELL_SIZE;
			int l_LastTurn = g_Turn;
			l_LastRow = row;
			l_LastCol = col;

			// Wykonaj klikniêcie jeœli komputer nie wykonuje teraz ruchu
			if (g_CalculateSI == true)
				Move_piece(row, col);

			// Utwórz autosave jeœli coœ siê zmieni³o
			if (g_Turn != l_LastTurn && g_Turn > 2)
				Save_game("Autosave");
		}
		if (event.button.button == SDL_BUTTON_RIGHT && g_CalculateSI == true && g_CanSelect == true) // Prawy przycisk myszy
		{
			g_SelectedRow = -1;
			g_SelectedCol = -1;

			Calculate_moves();
		}

		for (int i=0; i<g_ButtonArray.size(); i++) // Przyciski w grze
		{
			if (SDL_PointInRect(&Point, &g_ButtonArray[i].frame) == true)
			{
				if (DEVELOPER_MODE == true)
					cout<<"Clicked on: '"<<g_ButtonArray[i].text<<"'"<<endl;

				// Wys³anie OnLoad Event jeœli konieczne
				SDL_Event CustomEvent;
				CustomEvent.type = SDL_MOUSEMOTION;
				CustomEvent.motion.timestamp = SDL_GetTicks();
				CustomEvent.motion.x = x;
				CustomEvent.motion.y = y;

				if (i == 0)			// "New game"
				{
					bool confirm=false;

					if (g_Turn <= 2)
						confirm = true;
					else if (g_Lang == "pol")
						confirm = Confirm_request("Czy na pewno chcesz rozpocz¹æ now¹ grê?", "(niezapisane postêpy zostan¹ utracone)");
					else
						confirm = Confirm_request("Are you sure you want to start a new game?", "(unsaved progress will be lost)");

					if (confirm == true)
					{
						Game_new();

						if (g_Lang == "pol")
							MainField_append("Utworzono now¹ grê!", COLOR_BLACK);
						else
							MainField_append("New game created!", COLOR_BLACK);
					}
				}
				else if (i == 1)	// "Save game"
				{
					bool answer=false;
					answer = Save_game("Save");

					if (answer == true)
					{
						if (g_Lang == "pol")
							MainField_append("Gra zosta³a zapisana!", COLOR_BLACK);
						else
							MainField_append("The game has been saved!", COLOR_BLACK);
					}
					else
					{
						if (g_Lang == "pol")
							MainField_append("B³¹d !!! Nie uda³o siê zapisaæ gry :(", COLOR_RED);
						else
							MainField_append("Error !!! Failed to save the game :(", COLOR_RED);
					}
				}
				else if (i == 2)	// "Load game"
				{
					bool confirm=false;

					if (g_Turn <= 2)
						confirm = true;
					else if (g_Lang == "pol")
						confirm = Confirm_request("Czy na pewno chcesz wczytaæ grê?", "(niezapisane postêpy zostan¹ utracone)");
					else
						confirm = Confirm_request("Are you sure you want to load the game?", "(unsaved progress will be lost)");

					if (confirm == true)
					{
						bool answer=false;
						answer = Load_game("Save");

						if (answer == true)
						{
							if (g_Lang == "pol")
								MainField_append("Gra zosta³a wczytana!", COLOR_BLACK);
							else
								MainField_append("The game has been loaded!", COLOR_BLACK);
						}
						else
						{
							if (g_Lang == "pol")
								MainField_append("B³¹d !!! Nie uda³o siê wczytaæ gry :(", COLOR_RED);
							else
								MainField_append("Error !!! Failed to load the game :(", COLOR_RED);
						}
					}
				}
				else if (i == 3)	// "Back to Menu"
				{
					bool confirm=false;

					if (g_Turn <= 2)
						confirm = true;
					else if (g_Lang == "pol")
						confirm = Confirm_request("Czy na pewno chcesz wyjœæ do menu?", "(niezapisane postêpy zostan¹ utracone)");
					else
						confirm = Confirm_request("Are you sure you want to exit to the menu?", "(unsaved progress will be lost)");

					if (confirm == true)
					{
						if (filesystem::exists("Game_data/Saves/Autosave.checkers"))
						{
							try
							{
								filesystem::remove("Game_data/Saves/Autosave.checkers");
							}
							catch (...)
							{
								SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "File system error!!!", "File system error!!!", NULL);
							}
						}

						g_Scene = "menu";
						SDL_PushEvent(&CustomEvent);
					}
				}

				break;
			}
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP) // Przeci¹ganie
	{
		int x = event.button.x;
		int y = event.button.y;

		if (event.button.button == SDL_BUTTON_LEFT && y-CELL_OFFSET >= 0 && x-CELL_OFFSET >= 0)
		{
			int row = (y-CELL_OFFSET) / CELL_SIZE;
			int col = (x-CELL_OFFSET) / CELL_SIZE;
			int l_LastTurn = g_Turn;

			// Wykonaj klikniêcie jeœli komputer nie wykonuje teraz ruchu
			if (g_CalculateSI == true && g_SelectedMoves[row][col] != 0) // Oraz wybrano pole na które mo¿na wykonaæ ruch
				if (l_LastRow != row || l_LastCol != col) // Ró¿ny rz¹d lub kolumna
					Move_piece(row, col);

			// Utwórz autosave jeœli coœ siê zmieni³o
			if (g_Turn != l_LastTurn && g_Turn > 2)
				Save_game("Autosave");
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

