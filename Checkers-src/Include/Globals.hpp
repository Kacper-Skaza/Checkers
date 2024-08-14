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

#ifndef CHECKERS_GLOBALS_HPP
#define CHECKERS_GLOBALS_HPP
using namespace std;

// SDL2 wskaŸniki
extern SDL_Window* g_Window;
extern SDL_Renderer* g_Renderer;
extern SDL_Cursor* g_DefaultCursor;
extern SDL_Cursor* g_HandCursor;
extern TTF_Font* g_Font_36;
extern TTF_Font* g_Font_72;
extern TTF_Font* g_Font_144;
extern TTF_Font* g_Font_Title;

// SDL2 kolory
extern const SDL_Color COLOR_WHITE;
extern const SDL_Color COLOR_GRAY;
extern const SDL_Color COLOR_SHADOW;
extern const SDL_Color COLOR_BLACK;
extern const SDL_Color COLOR_RED;
extern const SDL_Color COLOR_DARKRED;
extern const SDL_Color COLOR_ORANGE;
extern const SDL_Color COLOR_LIME;

// Sta³e globalne
extern const bool DEVELOPER_MODE;	// Tryb dewelopera - prze³¹cznik dla funkcji Debug() i innych podobnych, w³¹cza rzeczywiste FPS
extern const int CELL_NUM;			// Iloœæ pól
extern const int CELL_SIZE;			// Rozmiar pól
extern const int CELL_OFFSET;		// Przesuniêcie pól
extern const int THREADS_NUM;
extern const string GAME_NAME;
extern const string GAME_VERSION;
extern const string GAME_COPYRIGHT;

// Zmienne globalne
extern SDL_DisplayMode g_DisplayMode;
extern long long g_TargetFPS;
extern string g_Scene;
extern string g_Lang;
extern bool g_RenderFrame;

// Zmienne gry
extern pair<int, int> g_Score;
extern long long g_Turn;
extern bool g_PlayerSI;
extern bool g_CalculateSI;
extern int g_SelectedRow;
extern int g_SelectedCol;
extern bool g_CanSelect;
extern bool g_WhiteCanCapture;
extern bool g_RedCanCapture;
extern bool g_WhiteCanMove;
extern bool g_RedCanMove;

// Tablice reprezentuj¹ce plansze
// -2 -> damka (czerwona)
// -1 -> zwyk³y pion (czerwony)
// 0 -> puste pole
// 1 -> zwyk³y pion (bia³y)
// 2 -> damka (bia³a)
extern vector<vector<int> > g_Board;
extern const vector<vector<int> > g_BoardDefault;

// Tablica reprezentuj¹ca mo¿liwe ruchy wybranego piona
// 0 -> brak akcji
// 1-4 -> pion do zbicia
// 5-8 -> pole na które mo¿liwy jest ruch
extern vector<vector<int> > g_SelectedMoves;

// Tablice reprezentuj¹ce mo¿liwe zbicia
// 0 -> brak akcji
// 1 -> pion do zbicia
// 2 -> pion mog¹cy dokonaæ bicia
extern vector<vector<int> > g_WhiteCaptures;
extern vector<vector<int> > g_RedCaptures;

// Tablice przechowuj¹ce dane g³ównego pola tekstowego
extern vector<string> g_MainFieldText;
extern vector<SDL_Color> g_MainFieldColor;

// Struktura przycisku
struct Button
{
	SDL_Rect frame;	// Ramka przycisku
	SDL_Rect rect;	// Wype³nienie przycisku
	string text;	// Tekst przycisku
	bool hovered;	// Najechano myszk¹
	bool selected;	// Wybrano przycisk (ustawienia)
};

// Tablica przechowuj¹ca przyciski
extern vector<Button> g_ButtonArray;

#endif
