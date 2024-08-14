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

// Funkcja dodaj�ca tekst do g��wnego pola tekstowego
void MainField_append(string txt, SDL_Color color)
{
	for (int i=0; i<g_MainFieldText.size()-1; i++)
		g_MainFieldText[i] = g_MainFieldText[i+1];
	for (int i=0; i<g_MainFieldColor.size()-1; i++)
		g_MainFieldColor[i] = g_MainFieldColor[i+1];

	g_MainFieldText[g_MainFieldText.size()-1] = txt;
	g_MainFieldColor[g_MainFieldColor.size()-1] = color;
}

// Funkcja tworz�ca now� gr�
void Game_new()
{
	// Wczytaj plansz�
	g_Board.assign(g_BoardDefault.begin(), g_BoardDefault.end());

	// Zresetuj zmienne
	g_Turn = 0;
	g_CalculateSI = true;
	g_SelectedRow = -1;
	g_SelectedCol = -1;
	g_CanSelect = true;

	// Oblicz ruchy
	Move_piece(0, 0);
}

// Funkcja �aduj�ca scen� gry
void Game_scene_load()
{
	// Ustaw ziarno RNG
	srand(time( NULL ));

	// Zresetuj zmienne
	g_Score = {0, 0};
	g_MainFieldText.assign(14, " ");
	g_MainFieldColor.assign(14, COLOR_BLACK);

	// Sprawd� czy jest autosave
	try
	{
		if (filesystem::exists("Game_data/Saves/Autosave.checkers"))
		{
			bool confirm=false;

			if (g_Lang == "pol")
				confirm = Confirm_request("Czy chcesz wczyta� autozapis?", " ");
			else
				confirm = Confirm_request("Do you want to load autosave?", " ");

			if (confirm == true)
				Load_game("Autosave");
			else
				Game_new();

			filesystem::remove("Game_data/Saves/Autosave.checkers");
		}
		else
		{
			Game_new();
		}
	}
	catch (...)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "File system error!!!", "File system error!!!", NULL);
		Game_new();
	}
}



// Funkcja odpowiadaj�ca za rozgrywk� z komputerem
void SI_turn(int random_num)
{
	// Struktury oraz zmienne
	struct Move
	{
		int FromRow;
		int FromCol;
		int ToRow;
		int ToCol;
	};
	vector<Move> AvaibleMoves(0);
	Move SelectedMove;

	// Znajd� dost�pne ruchy
	for (int row=0; row<CELL_NUM; row++)
	{
		for (int col=0; col<CELL_NUM; col++)
		{
			if (g_Board[row][col] < 0)
			{
				Move_piece(row, col);

				if (g_SelectedRow == row && g_SelectedCol == col)
					for (int temp_row=0; temp_row<CELL_NUM; temp_row++)
						for (int temp_col=0; temp_col<CELL_NUM; temp_col++)
							if (g_SelectedMoves[temp_row][temp_col] >= 5)
								AvaibleMoves.push_back({row, col, temp_row, temp_col});
			}
		}
	}

	// Wybierz jeden z nich (losowo :P)
	SelectedMove = AvaibleMoves[random_num % AvaibleMoves.size()];

	// Kliknij wybrany pionek
	Move_piece(SelectedMove.FromRow, SelectedMove.FromCol);
	for (int row=0; row<CELL_NUM; row++)
		for (int col=0; col<CELL_NUM; col++)
			if (row != SelectedMove.ToRow || col != SelectedMove.ToCol)
				g_SelectedMoves[row][col] = 0;
	g_RenderFrame = true;

	// Op�nienie i flagi
	g_CanSelect = false;
	SDL_Delay(800);
	g_CanSelect = true;

	// Wykonaj ruch
	g_CalculateSI = true;
	Move_piece(SelectedMove.ToRow, SelectedMove.ToCol);
	g_RenderFrame = true;
}



// Funkcja poruszaj�ca pionkami
void Move_piece(int row, int col)
{
	if (g_Turn >= 0 && (row >= 0 && row < CELL_NUM) && (col >= 0 && col < CELL_NUM))
	{
		if (g_Turn%2 == 0) // Tura bia�ych
		{
			if (g_WhiteCanCapture == false)
			{
				if (g_Board[row][col] > 0) // Je�li wybrano bia�y pion
				{
					if (g_CanSelect == true)
					{
						g_SelectedRow = row;
						g_SelectedCol = col;
					}
				}
				else if (g_SelectedRow != -1 && g_SelectedCol != -1) // Je�li wybrano zwyk�y ruch
				{
					if (g_SelectedMoves[row][col] >= 5 && g_SelectedMoves[row][col] <= 8)
					{
						g_Board[row][col] = g_Board[g_SelectedRow][g_SelectedCol];
						g_Board[g_SelectedRow][g_SelectedCol] = 0;
						g_SelectedRow = -1;
						g_SelectedCol = -1;
						g_CanSelect = true;
						g_Turn++;
					}
				}
			}
			else
			{
				if (g_Board[row][col] > 0 && g_WhiteCaptures[row][col] == 2) // Je�li wybrano bia�y pion kt�ry mo�e dokona� bicia
				{
					if (g_CanSelect == true)
					{
						g_SelectedRow = row;
						g_SelectedCol = col;
					}
				}
				else if (g_SelectedRow != -1 && g_SelectedCol != -1) // Je�li wybrano bicie
				{
					if (g_SelectedMoves[row][col] >= 5 && g_SelectedMoves[row][col] <= 8)
					{
						if (g_SelectedMoves[row][col]-4 == 1)
							g_Board[row+1][col+1] = 0;
						else if (g_SelectedMoves[row][col]-4 == 2)
							g_Board[row+1][col-1] = 0;
						else if (g_SelectedMoves[row][col]-4 == 3)
							g_Board[row-1][col+1] = 0;
						else if (g_SelectedMoves[row][col]-4 == 4)
							g_Board[row-1][col-1] = 0;

						g_Board[row][col] = g_Board[g_SelectedRow][g_SelectedCol];
						g_Board[g_SelectedRow][g_SelectedCol] = 0;
						g_SelectedRow = row;
						g_SelectedCol = col;
						g_CanSelect = false;
						Calculate_moves();

						if (g_WhiteCaptures[g_SelectedRow][g_SelectedCol] != 2) // Je�li nie mo�na dalej bi� tym samym pionem
						{
							g_CanSelect = true;
							g_SelectedRow = -1;
							g_SelectedCol = -1;
							g_Turn++;
						}
					}
					else if (g_SelectedMoves[row][col] >= 1 && g_SelectedMoves[row][col] <= 4)
					{
						if (g_SelectedMoves[row][col]+4 == 5)
						{
							g_Board[row-1][col-1] = g_Board[g_SelectedRow][g_SelectedCol];
							g_Board[g_SelectedRow][g_SelectedCol] = 0;
							g_SelectedRow = row-1;
							g_SelectedCol = col-1;
						}
						else if (g_SelectedMoves[row][col]+4 == 6)
						{
							g_Board[row-1][col+1] = g_Board[g_SelectedRow][g_SelectedCol];
							g_Board[g_SelectedRow][g_SelectedCol] = 0;
							g_SelectedRow = row-1;
							g_SelectedCol = col+1;
						}
						else if (g_SelectedMoves[row][col]+4 == 7)
						{
							g_Board[row+1][col-1] = g_Board[g_SelectedRow][g_SelectedCol];
							g_Board[g_SelectedRow][g_SelectedCol] = 0;
							g_SelectedRow = row+1;
							g_SelectedCol = col-1;
						}
						else if (g_SelectedMoves[row][col]+4 == 8)
						{
							g_Board[row+1][col+1] = g_Board[g_SelectedRow][g_SelectedCol];
							g_Board[g_SelectedRow][g_SelectedCol] = 0;
							g_SelectedRow = row+1;
							g_SelectedCol = col+1;
						}

						g_Board[row][col] = 0;
						g_CanSelect = false;
						Calculate_moves();

						if (g_WhiteCaptures[g_SelectedRow][g_SelectedCol] != 2) // Je�li nie mo�na dalej bi� tym samym pionem
						{
							g_CanSelect = true;
							g_SelectedRow = -1;
							g_SelectedCol = -1;
							g_Turn++;
						}
					}
				}
			}
		}
		else // Tura czerwonych
		{
			if (g_RedCanCapture == false)
			{
				if (g_Board[row][col] < 0) // Je�li wybrano czerwony pion
				{
					if (g_CanSelect == true)
					{
						g_SelectedRow = row;
						g_SelectedCol = col;
					}
				}
				else if (g_SelectedRow != -1 && g_SelectedCol != -1) // Je�li wybrano zwyk�y ruch
				{
					if (g_SelectedMoves[row][col] >= 5 && g_SelectedMoves[row][col] <= 8)
					{
						g_Board[row][col] = g_Board[g_SelectedRow][g_SelectedCol];
						g_Board[g_SelectedRow][g_SelectedCol] = 0;
						g_SelectedRow = -1;
						g_SelectedCol = -1;
						g_CanSelect = true;
						g_Turn++;
					}
				}
			}
			else
			{
				if (g_Board[row][col] < 0 && g_RedCaptures[row][col] == 2) // Je�li wybrano czerwony pion kt�ry mo�e dokona� bicia
				{
					if (g_CanSelect == true)
					{
						g_SelectedRow = row;
						g_SelectedCol = col;
					}
				}
				else if (g_SelectedRow != -1 && g_SelectedCol != -1) // Je�li wybrano bicie
				{
					if (g_SelectedMoves[row][col] >= 5 && g_SelectedMoves[row][col] <= 8)
					{
						if (g_SelectedMoves[row][col]-4 == 1)
							g_Board[row+1][col+1] = 0;
						else if (g_SelectedMoves[row][col]-4 == 2)
							g_Board[row+1][col-1] = 0;
						else if (g_SelectedMoves[row][col]-4 == 3)
							g_Board[row-1][col+1] = 0;
						else if (g_SelectedMoves[row][col]-4 == 4)
							g_Board[row-1][col-1] = 0;

						g_Board[row][col] = g_Board[g_SelectedRow][g_SelectedCol];
						g_Board[g_SelectedRow][g_SelectedCol] = 0;
						g_SelectedRow = row;
						g_SelectedCol = col;
						g_CanSelect = false;
						Calculate_moves();

						if (g_RedCaptures[g_SelectedRow][g_SelectedCol] != 2) // Je�li nie mo�na dalej bi� tym samym pionem
						{
							g_CanSelect = true;
							g_SelectedRow = -1;
							g_SelectedCol = -1;
							g_Turn++;
						}
					}
					else if (g_SelectedMoves[row][col] >= 1 && g_SelectedMoves[row][col] <= 4)
					{
						if (g_SelectedMoves[row][col]+4 == 5)
						{
							g_Board[row-1][col-1] = g_Board[g_SelectedRow][g_SelectedCol];
							g_Board[g_SelectedRow][g_SelectedCol] = 0;
							g_SelectedRow = row-1;
							g_SelectedCol = col-1;
						}
						else if (g_SelectedMoves[row][col]+4 == 6)
						{
							g_Board[row-1][col+1] = g_Board[g_SelectedRow][g_SelectedCol];
							g_Board[g_SelectedRow][g_SelectedCol] = 0;
							g_SelectedRow = row-1;
							g_SelectedCol = col+1;
						}
						else if (g_SelectedMoves[row][col]+4 == 7)
						{
							g_Board[row+1][col-1] = g_Board[g_SelectedRow][g_SelectedCol];
							g_Board[g_SelectedRow][g_SelectedCol] = 0;
							g_SelectedRow = row+1;
							g_SelectedCol = col-1;
						}
						else if (g_SelectedMoves[row][col]+4 == 8)
						{
							g_Board[row+1][col+1] = g_Board[g_SelectedRow][g_SelectedCol];
							g_Board[g_SelectedRow][g_SelectedCol] = 0;
							g_SelectedRow = row+1;
							g_SelectedCol = col+1;
						}

						g_Board[row][col] = 0;
						g_CanSelect = false;
						Calculate_moves();

						if (g_RedCaptures[g_SelectedRow][g_SelectedCol] != 2) // Je�li nie mo�na dalej bi� tym samym pionem
						{
							g_CanSelect = true;
							g_SelectedRow = -1;
							g_SelectedCol = -1;
							g_Turn++;
						}
					}
				}
			}
		}

		// Kalkulacja ruch�w na ko�cu
		Calculate_moves();

		// Sprawdzenie czy to koniec gry
		if (g_Turn%2 == 1 && g_RedCanMove == false && g_CalculateSI == true)
		{
			g_Turn = -1;
			g_Score.first++;

			if (g_Lang == "pol")
				MainField_append("BIALI WYGRALI !!!", COLOR_RED);
			else
				MainField_append("WHITE WON !!!", COLOR_RED);
		}
		else if (g_Turn%2 == 0 && g_WhiteCanMove == false && g_CalculateSI == true)
		{
			g_Turn = -1;
			g_Score.second++;

			if (g_Lang == "pol")
				MainField_append("CZERWONI WYGRALI !!!", COLOR_RED);
			else
				MainField_append("RED WON !!!", COLOR_RED);
		}

		// Je�li komputer ma zagra�
		if (g_Turn%2 == 1 && g_RedCanMove == true && g_PlayerSI == true && g_CalculateSI == true)
		{
			g_CalculateSI = false;

			if (THREADS_NUM >= 2)
			{
				thread ThreadSI(SI_turn, rand());
				ThreadSI.detach();
			}
			else
			{
				SI_turn(rand());
			}
		}
	}
}



// Funkcja kalkuluj�ca mo�liwe ruchy
void Calculate_moves()
{
	// Resetowanie zmiennych
	g_SelectedMoves.assign(CELL_NUM, vector<int>(CELL_NUM, 0));
	g_WhiteCaptures.assign(CELL_NUM, vector<int>(CELL_NUM, 0));
	g_RedCaptures.assign(CELL_NUM, vector<int>(CELL_NUM, 0));
	g_WhiteCanCapture = false;
	g_WhiteCanMove = false;
	g_RedCanCapture = false;
	g_RedCanMove = false;

	// Kalkulacja bi�
	for (int row=0; row<CELL_NUM; row++)
	{
		for (int col=0; col<CELL_NUM; col++)
		{
			if (g_CanSelect == false && (g_SelectedRow != row || g_SelectedCol != col))
				continue; // Nie szukaj bi� dla innych pionk�w gdy jest wykonywany kolejny ruch z rz�du

			// Kalkulacja zamiany w damki
			if (row == 0 && g_Board[row][col] == 1)
			{
				g_Board[0][col] = 2;
				continue;
			}
			if (row == 7 && g_Board[row][col] == -1)
			{
				g_Board[7][col] = -2;
				continue;
			}

			/* Kolejno�� sprawdzania kierunk�w:
				1#2
				#X#
				3#4
			*/

			if (g_Board[row][col] > 0) // Je�li wybrano bia�y pion
			{
				int temp_row=0, temp_col=0;

				temp_row = row;
				temp_col = col;
				while (temp_row >= 2 && temp_col >= 2)
				{
					if (g_Board[temp_row-1][temp_col-1] < 0 && g_Board[temp_row-2][temp_col-2] == 0)
					{
						g_WhiteCaptures[temp_row-1][temp_col-1] = 1;
						g_WhiteCaptures[row][col] = 2;
						g_WhiteCanCapture = true;
						g_WhiteCanMove = true;

						if (g_SelectedRow == row && g_SelectedCol == col)
						{
							g_SelectedMoves[temp_row-1][temp_col-1] = 1;
							g_SelectedMoves[temp_row-2][temp_col-2] = 5;
						}
					}
					if (abs(g_Board[row][col]) == 1 || g_Board[temp_row-1][temp_col-1] != 0)
						break;

					temp_row--;
					temp_col--;
				}

				temp_row = row;
				temp_col = col;
				while (temp_row >= 2 && temp_col <= 5)
				{
					if (g_Board[temp_row-1][temp_col+1] < 0 && g_Board[temp_row-2][temp_col+2] == 0)
					{
						g_WhiteCaptures[temp_row-1][temp_col+1] = 1;
						g_WhiteCaptures[row][col] = 2;
						g_WhiteCanCapture = true;
						g_WhiteCanMove = true;

						if (g_SelectedRow == row && g_SelectedCol == col)
						{
							g_SelectedMoves[temp_row-1][temp_col+1] = 2;
							g_SelectedMoves[temp_row-2][temp_col+2] = 6;
						}
					}
					if (abs(g_Board[row][col]) == 1 || g_Board[temp_row-1][temp_col+1] != 0)
						break;

					temp_row--;
					temp_col++;
				}

				temp_row = row;
				temp_col = col;
				while (temp_row <= 5 && temp_col >= 2)
				{
					if (g_Board[temp_row+1][temp_col-1] < 0 && g_Board[temp_row+2][temp_col-2] == 0)
					{
						g_WhiteCaptures[temp_row+1][temp_col-1] = 1;
						g_WhiteCaptures[row][col] = 2;
						g_WhiteCanCapture = true;
						g_WhiteCanMove = true;

						if (g_SelectedRow == row && g_SelectedCol == col)
						{
							g_SelectedMoves[temp_row+1][temp_col-1] = 3;
							g_SelectedMoves[temp_row+2][temp_col-2] = 7;
						}
					}
					if (abs(g_Board[row][col]) == 1 || g_Board[temp_row+1][temp_col-1] != 0)
						break;

					temp_row++;
					temp_col--;
				}

				temp_row = row;
				temp_col = col;
				while (temp_row <= 5 && temp_col <= 5)
				{
					if (g_Board[temp_row+1][temp_col+1] < 0 && g_Board[temp_row+2][temp_col+2] == 0)
					{
						g_WhiteCaptures[temp_row+1][temp_col+1] = 1;
						g_WhiteCaptures[row][col] = 2;
						g_WhiteCanCapture = true;
						g_WhiteCanMove = true;

						if (g_SelectedRow == row && g_SelectedCol == col)
						{
							g_SelectedMoves[temp_row+1][temp_col+1] = 4;
							g_SelectedMoves[temp_row+2][temp_col+2] = 8;
						}
					}
					if (abs(g_Board[row][col]) == 1 || g_Board[temp_row+1][temp_col+1] != 0)
						break;

					temp_row++;
					temp_col++;
				}
			}
			else if (g_Board[row][col] < 0) // Je�li wybrano czerwony pion
			{
				int temp_row=0, temp_col=0;

				temp_row = row;
				temp_col = col;
				while (temp_row >= 2 && temp_col >= 2)
				{
					if (g_Board[temp_row-1][temp_col-1] > 0 && g_Board[temp_row-2][temp_col-2] == 0)
					{
						g_RedCaptures[temp_row-1][temp_col-1] = 1;
						g_RedCaptures[row][col] = 2;
						g_RedCanCapture = true;
						g_RedCanMove = true;

						if (g_SelectedRow == row && g_SelectedCol == col)
						{
							g_SelectedMoves[temp_row-1][temp_col-1] = 1;
							g_SelectedMoves[temp_row-2][temp_col-2] = 5;
						}
					}
					if (abs(g_Board[row][col]) == 1 || g_Board[temp_row-1][temp_col-1] != 0)
						break;

					temp_row--;
					temp_col--;
				}

				temp_row = row;
				temp_col = col;
				while (temp_row >= 2 && temp_col <= 5)
				{
					if (g_Board[temp_row-1][temp_col+1] > 0 && g_Board[temp_row-2][temp_col+2] == 0)
					{
						g_RedCaptures[temp_row-1][temp_col+1] = 1;
						g_RedCaptures[row][col] = 2;
						g_RedCanCapture = true;
						g_RedCanMove = true;

						if (g_SelectedRow == row && g_SelectedCol == col)
						{
							g_SelectedMoves[temp_row-1][temp_col+1] = 2;
							g_SelectedMoves[temp_row-2][temp_col+2] = 6;
						}
					}
					if (abs(g_Board[row][col]) == 1 || g_Board[temp_row-1][temp_col+1] != 0)
						break;

					temp_row--;
					temp_col++;
				}

				temp_row = row;
				temp_col = col;
				while (temp_row <= 5 && temp_col >= 2)
				{
					if (g_Board[temp_row+1][temp_col-1] > 0 && g_Board[temp_row+2][temp_col-2] == 0)
					{
						g_RedCaptures[temp_row+1][temp_col-1] = 1;
						g_RedCaptures[row][col] = 2;
						g_RedCanCapture = true;
						g_RedCanMove = true;

						if (g_SelectedRow == row && g_SelectedCol == col)
						{
							g_SelectedMoves[temp_row+1][temp_col-1] = 3;
							g_SelectedMoves[temp_row+2][temp_col-2] = 7;
						}
					}
					if (abs(g_Board[row][col]) == 1 || g_Board[temp_row+1][temp_col-1] != 0)
						break;

					temp_row++;
					temp_col--;
				}

				temp_row = row;
				temp_col = col;
				while (temp_row <= 5 && temp_col <= 5)
				{
					if (g_Board[temp_row+1][temp_col+1] > 0 && g_Board[temp_row+2][temp_col+2] == 0)
					{
						g_RedCaptures[temp_row+1][temp_col+1] = 1;
						g_RedCaptures[row][col] = 2;
						g_RedCanCapture = true;
						g_RedCanMove = true;

						if (g_SelectedRow == row && g_SelectedCol == col)
						{
							g_SelectedMoves[temp_row+1][temp_col+1] = 4;
							g_SelectedMoves[temp_row+2][temp_col+2] = 8;
						}
					}
					if (abs(g_Board[row][col]) == 1 || g_Board[temp_row+1][temp_col+1] != 0)
						break;

					temp_row++;
					temp_col++;
				}
			}
		}
	}

	// Kalkulacja zwyk�ych ruch�w
	if (g_CanSelect == false)
	{
		g_WhiteCanMove = true;
		g_RedCanMove = true;
	}
	else
	{
		for (int row=0; row<CELL_NUM; row++)
		{
			for (int col=0; col<CELL_NUM; col++)
			{
				/* Kolejno�� sprawdzania kierunk�w:
					1#2
					#X#
					3#4
				*/

				if (g_WhiteCanMove == false && g_Board[row][col] > 0)
				{
					if (row > 0 && col > 0)
					{
						if (g_Board[row-1][col-1] == 0)
							g_WhiteCanMove = true;
					}
					if (row > 0 && col < 7)
					{
						if (g_Board[row-1][col+1] == 0)
							g_WhiteCanMove = true;
					}
					if (row < 7 && col > 0 && g_Board[row][col] == 2)
					{
						if (g_Board[row+1][col-1] == 0)
							g_WhiteCanMove = true;
					}
					if (row < 7 && col < 7 && g_Board[row][col] == 2)
					{
						if (g_Board[row+1][col+1] == 0)
							g_WhiteCanMove = true;
					}
				}
				else if (g_RedCanMove == false && g_Board[row][col] < 0)
				{
					if (row > 0 && col > 0 && g_Board[row][col] == -2)
					{
						if (g_Board[row-1][col-1] == 0)
							g_RedCanMove = true;
					}
					if (row > 0 && col < 7 && g_Board[row][col] == -2)
					{
						if (g_Board[row-1][col+1] == 0)
							g_RedCanMove = true;
					}
					if (row < 7 && col > 0)
					{
						if (g_Board[row+1][col-1] == 0)
							g_RedCanMove = true;
					}
					if (row < 7 && col < 7)
					{
						if (g_Board[row+1][col+1] == 0)
							g_RedCanMove = true;
					}
				}

				if (g_WhiteCanMove == true && g_RedCanMove == true)
					break;
			}

			if (g_WhiteCanMove == true && g_RedCanMove == true)
				break;
		}
	}

	// Kalkulacja zwyk�ych ruch�w dla wybranego pionka
	if (g_SelectedRow != -1 && g_SelectedCol != -1)
	{
		if (g_WhiteCanCapture == false && g_Board[g_SelectedRow][g_SelectedCol] == 2) // Bia�a damka
		{
			int temp_row=0, temp_col=0;

			temp_row = g_SelectedRow;
			temp_col = g_SelectedCol;
			while (temp_row > 0 && temp_col > 0 && g_Board[temp_row-1][temp_col-1] == 0)
			{
				g_SelectedMoves[temp_row-1][temp_col-1] = 5;
				temp_row--;
				temp_col--;
			}

			temp_row = g_SelectedRow;
			temp_col = g_SelectedCol;
			while (temp_row > 0 && temp_col < 7 && g_Board[temp_row-1][temp_col+1] == 0)
			{
				g_SelectedMoves[temp_row-1][temp_col+1] = 6;
				temp_row--;
				temp_col++;
			}

			temp_row = g_SelectedRow;
			temp_col = g_SelectedCol;
			while (temp_row < 7 && temp_col > 0 && g_Board[temp_row+1][temp_col-1] == 0)
			{
				g_SelectedMoves[temp_row+1][temp_col-1] = 7;
				temp_row++;
				temp_col--;
			}

			temp_row = g_SelectedRow;
			temp_col = g_SelectedCol;
			while (temp_row < 7 && temp_col < 7 && g_Board[temp_row+1][temp_col+1] == 0)
			{
				g_SelectedMoves[temp_row+1][temp_col+1] = 8;
				temp_row++;
				temp_col++;
			}
		}
		else if (g_WhiteCanCapture == false && g_Board[g_SelectedRow][g_SelectedCol] == 1) // Bia�y zwyk�y pionek
		{
			if (g_SelectedRow > 0)
			{
				if (g_SelectedCol > 0 && g_Board[g_SelectedRow-1][g_SelectedCol-1] == 0)
					g_SelectedMoves[g_SelectedRow-1][g_SelectedCol-1] = 5;
				if (g_SelectedCol < 7 && g_Board[g_SelectedRow-1][g_SelectedCol+1] == 0)
					g_SelectedMoves[g_SelectedRow-1][g_SelectedCol+1] = 6;
			}
		}
		else if (g_RedCanCapture == false && g_Board[g_SelectedRow][g_SelectedCol] == -1) // Czerwony zwyk�y pionek
		{
			if (g_SelectedRow < 7)
			{
				if (g_SelectedCol > 0 && g_Board[g_SelectedRow+1][g_SelectedCol-1] == 0)
					g_SelectedMoves[g_SelectedRow+1][g_SelectedCol-1] = 7;
				if (g_SelectedCol < 7 && g_Board[g_SelectedRow+1][g_SelectedCol+1] == 0)
					g_SelectedMoves[g_SelectedRow+1][g_SelectedCol+1] = 8;
			}
		}
		else if (g_RedCanCapture == false && g_Board[g_SelectedRow][g_SelectedCol] == -2) // Czerwona damka
		{
			int temp_row=0, temp_col=0;

			temp_row = g_SelectedRow;
			temp_col = g_SelectedCol;
			while (temp_row > 0 && temp_col > 0 && g_Board[temp_row-1][temp_col-1] == 0)
			{
				g_SelectedMoves[temp_row-1][temp_col-1] = 5;
				temp_row--;
				temp_col--;
			}

			temp_row = g_SelectedRow;
			temp_col = g_SelectedCol;
			while (temp_row > 0 && temp_col < 7 && g_Board[temp_row-1][temp_col+1] == 0)
			{
				g_SelectedMoves[temp_row-1][temp_col+1] = 6;
				temp_row--;
				temp_col++;
			}

			temp_row = g_SelectedRow;
			temp_col = g_SelectedCol;
			while (temp_row < 7 && temp_col > 0 && g_Board[temp_row+1][temp_col-1] == 0)
			{
				g_SelectedMoves[temp_row+1][temp_col-1] = 7;
				temp_row++;
				temp_col--;
			}

			temp_row = g_SelectedRow;
			temp_col = g_SelectedCol;
			while (temp_row < 7 && temp_col < 7 && g_Board[temp_row+1][temp_col+1] == 0)
			{
				g_SelectedMoves[temp_row+1][temp_col+1] = 8;
				temp_row++;
				temp_col++;
			}
		}
	}

	// Debug
	if (DEVELOPER_MODE == true)
	{
		cout<<"Func: Calculate_moves()";
		Debug();
	}
}

