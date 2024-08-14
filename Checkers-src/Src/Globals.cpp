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

#include "../Include/Z_Includes.hpp"
#include "../Include/Globals.hpp"

// SDL2 wskaŸniki
SDL_Window* g_Window = nullptr;
SDL_Renderer* g_Renderer = nullptr;
SDL_Cursor* g_DefaultCursor = nullptr;
SDL_Cursor* g_HandCursor = nullptr;
TTF_Font* g_Font_36 = nullptr;
TTF_Font* g_Font_72 = nullptr;
TTF_Font* g_Font_144 = nullptr;
TTF_Font* g_Font_Title = nullptr;

// SDL2 kolory
const SDL_Color COLOR_WHITE = {255, 255, 255, 255};
const SDL_Color COLOR_GRAY = {128, 128, 128, 255};
const SDL_Color COLOR_SHADOW = {48, 48, 48, 255};
const SDL_Color COLOR_BLACK = {0, 0, 0, 255};
const SDL_Color COLOR_RED = {255, 0, 0, 255};
const SDL_Color COLOR_DARKRED = {128, 0, 0, 255};
const SDL_Color COLOR_ORANGE = {255, 172, 28, 255};
const SDL_Color COLOR_LIME = {68, 221, 68, 255};

// Sta³e globalne
const bool DEVELOPER_MODE = false;	// Tryb dewelopera - prze³¹cznik dla funkcji Debug() i innych podobnych, w³¹cza rzeczywiste FPS
const int CELL_NUM = 8;				// Iloœæ pól
const int CELL_SIZE = 200;			// Rozmiar pól
const int CELL_OFFSET = 100;		// Przesuniêcie pól
const int THREADS_NUM = thread::hardware_concurrency();
const string GAME_NAME = ">> Checkers << made by Kacper Skaza";
const string GAME_VERSION = "v." + to_string(VER_MAJOR) + "." + to_string(VER_MINOR) + "." + to_string(VER_RELEASE);
const string GAME_COPYRIGHT = "© 2024 Kacper Skaza";

// Zmienne globalne
SDL_DisplayMode g_DisplayMode = {0, 0, 0, 0, nullptr};
long long g_TargetFPS = 60;
string g_Scene = "menu";
string g_Lang = "eng";
bool g_RenderFrame = true;

// Zmienne gry
pair<int, int> g_Score = {0, 0};
long long g_Turn = 0;
bool g_PlayerSI = true;
bool g_CalculateSI = true;
int g_SelectedRow = -1;
int g_SelectedCol = -1;
bool g_CanSelect = true;
bool g_WhiteCanCapture = false;
bool g_RedCanCapture = false;
bool g_WhiteCanMove = false;
bool g_RedCanMove = false;

// Tablice reprezentuj¹ce plansze
// -2 -> damka (czerwona)
// -1 -> zwyk³y pion (czerwony)
// 0 -> puste pole
// 1 -> zwyk³y pion (bia³y)
// 2 -> damka (bia³a)
vector<vector<int> > g_Board(CELL_NUM, vector<int>(CELL_NUM, 0));
const vector<vector<int> > g_BoardDefault =
{
	{0, -1, 0, -1, 0, -1, 0, -1},
	{-1, 0, -1, 0, -1, 0, -1, 0},
	{0, -1, 0, -1, 0, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 1, 0, 1, 0, 1, 0},
	{0, 1, 0, 1, 0, 1, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 0}
};

// Tablica reprezentuj¹ca mo¿liwe ruchy wybranego piona
// 0 -> brak akcji
// 1-4 -> pion do zbicia
// 5-8 -> pole na które mo¿liwy jest ruch
vector<vector<int> > g_SelectedMoves(CELL_NUM, vector<int>(CELL_NUM, 0));

// Tablice reprezentuj¹ce mo¿liwe zbicia
// 0 -> brak akcji
// 1 -> pion do zbicia
// 2 -> pion mog¹cy dokonaæ bicia
vector<vector<int> > g_WhiteCaptures(CELL_NUM, vector<int>(CELL_NUM, 0));
vector<vector<int> > g_RedCaptures(CELL_NUM, vector<int>(CELL_NUM, 0));

// Tablice przechowuj¹ce dane g³ównego pola tekstowego
vector<string> g_MainFieldText(14, " ");
vector<SDL_Color> g_MainFieldColor(14, COLOR_BLACK);

// Tablica przechowuj¹ca przyciski
vector<Button> g_ButtonArray(0);

