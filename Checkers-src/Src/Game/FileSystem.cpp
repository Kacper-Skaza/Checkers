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

// Funkcja inicjalizuj¹ca ustawienia
int Init_settings()
{
	try
	{
		filesystem::create_directories("Game_data");
		filesystem::create_directories("Game_data/Fonts");
		filesystem::create_directories("Game_data/Saves");

		if (filesystem::exists("Game_data/Saves/Settings.checkers"))
			Load_settings();
		else
			Save_settings();
	}
	catch (...)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "File system error!!!", "File system error!!!", NULL);
		return 11;
	}

	return 0;
}

// Funkcja zapisuj¹ca ustawienia
void Save_settings()
{
	ofstream out("Game_data/Saves/Settings.checkers");

	if (out.good() == true)
	{
		out<<"GAME_VERSION = {"<<GAME_VERSION<<"}"<<endl;

		out<<"g_TargetFPS = {"<<g_TargetFPS<<"}"<<endl;
		out<<"g_Lang = {"<<g_Lang<<"}";

		out.flush();
	}

	out.close();
}

// Funkcja ³aduj¹ca ustawienia
void Load_settings()
{
	ifstream inp("Game_data/Saves/Settings.checkers");

	if (inp.good() == true)
	{
		while (!inp.eof())
		{
			vector<string> T = {"", "", ""};
			string txt="";
			int j=0;

			getline(inp, txt);

			for (int i=0; i<txt.size(); i++)
			{
				if (txt[i] != ' ' && txt[i] != '{' && txt[i] != '}')
					T[j] += txt[i];
				else if (txt[i] == ' ' && j<2)
					j++;
			}

			if (T[1] == "=")
			{
				if (T[0] == "GAME_VERSION") // GAME_VERSION
				{
					if (T[2] != GAME_VERSION)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania GAME_VERSION: "<<T[2]<<endl;

						Save_settings();
						break;
					}
				}
				else if (T[0] == "g_TargetFPS") // g_TargetFPS
				{
					try
					{
						g_TargetFPS = stoll(T[2]);
					}
					catch (...)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania g_TargetFPS: "<<T[2]<<endl;

						g_TargetFPS = 60;
					}
				}
				else if (T[0] == "g_Lang") // g_Lang
				{
					g_Lang = T[2];
				}
			}
		}
	}

	inp.close();
}



// Funkcja zapisuj¹ca grê
bool Save_game(string FileName)
{
	ofstream out("Game_data/Saves/" + FileName + ".checkers");

	if (out.good() == true)
	{
		out<<"GAME_VERSION = {"<<GAME_VERSION<<"}"<<endl;

		out<<"g_Score.first = {"<<g_Score.first<<"}"<<endl;
		out<<"g_Score.second = {"<<g_Score.second<<"}"<<endl;
		out<<"g_Turn = {"<<g_Turn<<"}"<<endl;
		out<<"g_SelectedRow = {"<<g_SelectedRow<<"}"<<endl;
		out<<"g_SelectedCol = {"<<g_SelectedCol<<"}"<<endl;
		out<<"g_CanSelect = {"<<g_CanSelect<<"}"<<endl;

		out<<"g_Board = {";
		for (int row=0; row<CELL_NUM; row++)
		{
			for (int col=0; col<CELL_NUM; col++)
			{
				out<<g_Board[row][col];
				if (col != CELL_NUM-1)
					out<<",";
			}

			out<<";";
		}
		out<<"}";

		out.flush();
		out.close();
		return true;
	}

	out.close();
	return false;
}

// Funkcja ³aduj¹ca grê
bool Load_game(string FileName)
{
	ifstream inp("Game_data/Saves/" + FileName + ".checkers");

	if (inp.good() == true)
	{
		while (!inp.eof())
		{
			vector<string> T = {"", "", ""};
			string txt="";
			int j=0;

			getline(inp, txt);

			for (int i=0; i<txt.size(); i++)
			{
				if (txt[i] != ' ' && txt[i] != '{' && txt[i] != '}')
					T[j] += txt[i];
				else if (txt[i] == ' ' && j<2)
					j++;
			}

			if (T[1] == "=")
			{
				if (T[0] == "GAME_VERSION") // GAME_VERSION
				{
					if (T[2] != GAME_VERSION)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania GAME_VERSION: "<<T[2]<<endl;

						inp.close();
						return false;
					}
				}
				else if (T[0] == "g_Score.first") // g_Score.first
				{
					try
					{
						g_Score.first = stoi(T[2]);
					}
					catch (...)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania g_Score.first: "<<T[2]<<endl;

						inp.close();
						Game_new();
						return false;
					}
				}
				else if (T[0] == "g_Score.second") // g_Score.second
				{
					try
					{
						g_Score.second = stoi(T[2]);
					}
					catch (...)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania g_Score.second: "<<T[2]<<endl;

						inp.close();
						Game_new();
						return false;
					}
				}
				else if (T[0] == "g_Turn") // g_Turn
				{
					try
					{
						g_Turn = stoll(T[2]);
					}
					catch (...)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania g_Turn: "<<T[2]<<endl;

						inp.close();
						Game_new();
						return false;
					}
				}
				else if (T[0] == "g_SelectedRow") // g_SelectedRow
				{
					try
					{
						g_SelectedRow = stoi(T[2]);
					}
					catch (...)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania g_SelectedRow: "<<T[2]<<endl;

						inp.close();
						Game_new();
						return false;
					}
				}
				else if (T[0] == "g_SelectedCol") // g_SelectedCol
				{
					try
					{
						g_SelectedCol = stoi(T[2]);
					}
					catch (...)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania g_SelectedCol: "<<T[2]<<endl;

						inp.close();
						Game_new();
						return false;
					}
				}
				else if (T[0] == "g_CanSelect") // g_CanSelect
				{
					try
					{
						g_CanSelect = stoi(T[2]);
					}
					catch (...)
					{
						if (DEVELOPER_MODE == true)
							cout<<"B³¹d ³adowania g_CanSelect: "<<T[2]<<endl;

						inp.close();
						Game_new();
						return false;
					}
				}
				else if (T[0] == "g_Board") // g_Board
				{
					g_Board.assign(CELL_NUM, vector<int>(CELL_NUM, 0));
					string temp="";
					int pos=0;

					for (int row=0; row<CELL_NUM; row++)
					{
						for (int col=0; col<CELL_NUM; col++)
						{
							temp = "";

							while (T[2][pos] != ',' && T[2][pos] != ';' && pos < T[2].size())
							{
								temp += T[2][pos];
								pos++;
							}
							pos++;

							try
							{
								g_Board[row][col] = stoi(temp);
							}
							catch (...)
							{
								if (DEVELOPER_MODE == true)
									cout<<"B³¹d ³adowania g_Board: "<<temp<<endl;

								inp.close();
								Game_new();
								return false;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		inp.close();
		return false;
	}

	// Zakoñcz
	inp.close();
	g_CalculateSI = true;
	Move_piece(0, 0);
	return true;
}

