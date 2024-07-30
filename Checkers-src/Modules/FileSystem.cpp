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

// Funkcja inicjalizuj¹ca system plików
int Init_settings()
{
	try
	{
		filesystem::create_directories("Saves");

		if (filesystem::exists("Saves/Settings.checkers"))
			Load_settings();
		else
			Save_settings();
	}
	catch (...)
	{
		cout<<"B³¹d systemu plików !!!";
		return 11;
	}

	return 0;
}

// Funkcja zapisuj¹ca ustawienia
void Save_settings()
{
	ofstream out("Saves/Settings.checkers");
	out<<"GAME_VERSION = {"<<GAME_VERSION<<"}"<<endl;

	out<<"g_TargetFPS = {"<<g_TargetFPS<<"}"<<endl;
	out<<"g_Lang = {"<<g_Lang<<"}";

	out.flush();
	out.close();
}

// Funkcja ³aduj¹ca ustawienia
void Load_settings()
{
	ifstream inp("Saves/Settings.checkers");

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



// Funkcja ³aduj¹ca scenê gry
void Load_game_scene()
{
	// Ustaw ziarno RNG
	srand(time( NULL ));

	// Zresetuj zmienne
	g_Score = {0, 0};
	g_MainFieldText.assign(14, " ");
	g_MainFieldColor.assign(14, COLOR_BLACK);

	// SprawdŸ czy jest autosave
	try
	{
		filesystem::path AutosavePath = "Saves/Autosave.checkers";

		if (filesystem::exists(AutosavePath))
		{
			bool confirm=false;

			if (g_Lang == "pol")
				confirm = Confirm_request("Czy chcesz wczytaæ autozapis?", " ");
			else
				confirm = Confirm_request("Do you want to load autosave?", " ");

			if (confirm == true)
				Load_game("Autosave");
			else
				Game_new();

			filesystem::remove(AutosavePath);
		}
		else
		{
			Game_new();
		}
	}
	catch (...)
	{
		cout<<"B³¹d systemu plików !!!";
		Game_new();
	}
}

// Funkcja zapisuj¹ca grê
bool Save_game(string FileName)
{
	ofstream out("Saves/" + FileName + ".checkers");
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

// Funkcja ³aduj¹ca grê
bool Load_game(string FileName)
{
	ifstream inp("Saves/" + FileName + ".checkers");

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
	Calculate_moves();

	// Jeœli komputer ma zagraæ
	if (g_Turn%2 == 1 && g_VersusAI == true)
	{
		thread DelayThread(AI_turn, rand());
		DelayThread.detach();
	}

	return true;
}

