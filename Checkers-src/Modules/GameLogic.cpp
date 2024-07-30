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

// Funkcja tworz¹ca now¹ grê
void Game_new()
{
	// Wczytaj planszê
	g_Board.assign(g_BoardDefault.begin(), g_BoardDefault.end());

	// Zresetuj zmienne
	g_Turn = 0;
	g_SelectedRow = -1;
	g_SelectedCol = -1;
	g_CanSelect = true;

	// Oblicz ruchy
	Calculate_moves();
}

// Funkcja dodaj¹ca tekst do g³ównego pola tekstowego
void MainField_append(string txt, SDL_Color color)
{
	for (int i=0; i<g_MainFieldText.size()-1; i++)
		g_MainFieldText[i] = g_MainFieldText[i+1];

	for (int i=0; i<g_MainFieldColor.size()-1; i++)
		g_MainFieldColor[i] = g_MainFieldColor[i+1];

	g_MainFieldText[g_MainFieldText.size()-1] = txt;
	g_MainFieldColor[g_MainFieldColor.size()-1] = color;
}



// Funkcja odpowiadaj¹ca za rozgrywkê z komputerem
void AI_turn(int random_num)
{
	// Czas
	long long Timer_Start=0, Delay=0;
	Timer_Start = SDL_GetTicks();

	// Struktury oraz zmienne
	struct Move
	{
		int FromRow;
		int FromCol;
		int ToRow;
		int ToCol;
	};
	int SelectedMove=0;
	vector<Move> AvaibleMoves(0);

	// ZnajdŸ dostêpne ruchy
	for (int row=0; row<CELL_NUM; row++)
	{
		for (int col=0; col<CELL_NUM; col++)
		{
			if (g_Board[row][col] < 0)
			{
				Move_piece(false, row, col);

				if (g_SelectedRow == row && g_SelectedCol == col)
					for (int roww=0; roww<CELL_NUM; roww++)
						for (int coll=0; coll<CELL_NUM; coll++)
							if (g_SelectedMoves[roww][coll] >= 5)
								AvaibleMoves.push_back({row, col, roww, coll});
			}
		}
	}

	// Wybierz jeden z nich (losowo :P)
	SelectedMove = random_num % AvaibleMoves.size();

	// Kliknij wybrany pionek
	Move_piece(false, AvaibleMoves[SelectedMove].FromRow, AvaibleMoves[SelectedMove].FromCol);
	for (int row=0; row<CELL_NUM; row++)
		for (int col=0; col<CELL_NUM; col++)
			if (row != AvaibleMoves[SelectedMove].ToRow || col != AvaibleMoves[SelectedMove].ToCol)
				g_SelectedMoves[row][col] = 0;
	g_RenderFrame = true;

	// OpóŸnienie i flaga
	g_CanSelect = false;
	Delay = 800 - (SDL_GetTicks()-Timer_Start);
	if (Delay >= 0)
		SDL_Delay(Delay);
	g_CanSelect = true;

	// Wykonaj ruch
	Move_piece(true, AvaibleMoves[SelectedMove].ToRow, AvaibleMoves[SelectedMove].ToCol);
	g_RenderFrame = true;
}



// Funkcja poruszaj¹ca pionkami
void Move_piece(bool calc_AI, int row, int col)
{
	if (g_Turn >= 0 && (row >= 0 && row < CELL_NUM) && (col >= 0 && col < CELL_NUM))
	{
		if (g_Turn%2 == 0) // Tura bia³ych
		{
			if (g_WhiteCanCapture == false)
			{
				if (g_Board[row][col] > 0) // Jeœli wybrano bia³y pion
				{
					if (g_CanSelect == true)
					{
						g_SelectedRow = row;
						g_SelectedCol = col;
					}
				}
				else if (g_SelectedRow != -1 && g_SelectedCol != -1) // Jeœli wybrano zwyk³y ruch
				{
					if (g_SelectedMoves[row][col] >= 5 && g_SelectedMoves[row][col] <= 8)
					{
						g_Board[row][col] = g_Board[g_SelectedRow][g_SelectedCol];
						g_Board[g_SelectedRow][g_SelectedCol] = 0;
						g_SelectedRow = -1;
						g_SelectedCol = -1;
						g_Turn++;
					}
				}
			}
			else
			{
				if (g_Board[row][col] > 0 && g_WhiteCaptures[row][col] == 2) // Jeœli wybrano bia³y pion który mo¿e dokonaæ bicia
				{
					if (g_CanSelect == true)
					{
						g_SelectedRow = row;
						g_SelectedCol = col;
					}
				}
				else if (g_SelectedRow != -1 && g_SelectedCol != -1) // Jeœli wybrano bicie
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

						if (g_WhiteCaptures[g_SelectedRow][g_SelectedCol] != 2) // Jeœli nie mo¿na dalej biæ tym samym pionem
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

						if (g_WhiteCaptures[g_SelectedRow][g_SelectedCol] != 2) // Jeœli nie mo¿na dalej biæ tym samym pionem
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
				if (g_Board[row][col] < 0) // Jeœli wybrano czerwony pion
				{
					if (g_CanSelect == true)
					{
						g_SelectedRow = row;
						g_SelectedCol = col;
					}
				}
				else if (g_SelectedRow != -1 && g_SelectedCol != -1) // Jeœli wybrano zwyk³y ruch
				{
					if (g_SelectedMoves[row][col] >= 5 && g_SelectedMoves[row][col] <= 8)
					{
						g_Board[row][col] = g_Board[g_SelectedRow][g_SelectedCol];
						g_Board[g_SelectedRow][g_SelectedCol] = 0;
						g_SelectedRow = -1;
						g_SelectedCol = -1;
						g_Turn++;
					}
				}
			}
			else
			{
				if (g_Board[row][col] < 0 && g_RedCaptures[row][col] == 2) // Jeœli wybrano czerwony pion który mo¿e dokonaæ bicia
				{
					if (g_CanSelect == true)
					{
						g_SelectedRow = row;
						g_SelectedCol = col;
					}
				}
				else if (g_SelectedRow != -1 && g_SelectedCol != -1) // Jeœli wybrano bicie
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

						if (g_RedCaptures[g_SelectedRow][g_SelectedCol] != 2) // Jeœli nie mo¿na dalej biæ tym samym pionem
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

						if (g_RedCaptures[g_SelectedRow][g_SelectedCol] != 2) // Jeœli nie mo¿na dalej biæ tym samym pionem
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

		// Kalkulacja ruchów na koñcu
		Calculate_moves();

		// Sprawdzenie czy to koniec gry
		if (g_Turn%2 == 0 && g_WhiteCanMove == false)
		{
			g_Turn = -1;
			g_Score.first++;

			if (g_Lang == "pol")
				MainField_append("CZERWONI WYGRALI !!!", COLOR_RED);
			else
				MainField_append("RED WON !!!", COLOR_RED);
		}
		else if (g_Turn%2 == 1 && g_RedCanMove == false)
		{
			g_Turn = -1;
			g_Score.second++;

			if (g_Lang == "pol")
				MainField_append("BIALI WYGRALI !!!", COLOR_RED);
			else
				MainField_append("WHITE WON !!!", COLOR_RED);
		}

		// Jeœli komputer ma zagraæ
		if (g_Turn%2 == 1 && g_RedCanMove == true && calc_AI == true && g_VersusAI == true)
		{
			thread DelayThread(AI_turn, rand());
			DelayThread.detach();
		}
	}
}



// Funkcja kalkuluj¹ca mo¿liwe ruchy
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

	// Kalkulacja biæ
	for (int row=0; row<CELL_NUM; row++)
	{
		for (int col=0; col<CELL_NUM; col++)
		{
			if (g_CanSelect == false && (g_SelectedRow != row || g_SelectedCol != col))
				continue; // Nie szukaj biæ dla innych pionków gdy jest wykonywany kolejny ruch z rzêdu

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

			/* Kolejnoœæ sprawdzania kierunków:
				1#2
				#X#
				3#4
			*/

			if (g_Board[row][col] > 0) // Jeœli wybrano bia³y pion
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
			else if (g_Board[row][col] < 0) // Jeœli wybrano czerwony pion
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

	// Kalkulacja zwyk³ych ruchów
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
				/* Kolejnoœæ sprawdzania kierunków:
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

	// Kalkulacja zwyk³ych ruchów dla wybranego pionka
	if (g_SelectedRow != -1 && g_SelectedCol != -1)
	{
		if (g_WhiteCanCapture == false && g_Board[g_SelectedRow][g_SelectedCol] == 2) // Bia³a damka
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
		else if (g_WhiteCanCapture == false && g_Board[g_SelectedRow][g_SelectedCol] == 1) // Bia³y zwyk³y pionek
		{
			if (g_SelectedRow > 0)
			{
				if (g_SelectedCol > 0 && g_Board[g_SelectedRow-1][g_SelectedCol-1] == 0)
					g_SelectedMoves[g_SelectedRow-1][g_SelectedCol-1] = 5;
				if (g_SelectedCol < 7 && g_Board[g_SelectedRow-1][g_SelectedCol+1] == 0)
					g_SelectedMoves[g_SelectedRow-1][g_SelectedCol+1] = 6;
			}
		}
		else if (g_RedCanCapture == false && g_Board[g_SelectedRow][g_SelectedCol] == -1) // Czerwony zwyk³y pionek
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

