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

// SDL2 wska�niki
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

// Sta�e globalne
extern const bool DEVELOPER_MODE;	// Tryb dewelopera - prze��cznik dla funkcji Debug() i innych podobnych, w��cza rzeczywiste FPS
extern const int CELL_NUM;			// Ilo�� p�l
extern const int CELL_SIZE;			// Rozmiar p�l
extern const int CELL_OFFSET;		// Przesuni�cie p�l
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

// Tablice reprezentuj�ce plansze
// -2 -> damka (czerwona)
// -1 -> zwyk�y pion (czerwony)
// 0 -> puste pole
// 1 -> zwyk�y pion (bia�y)
// 2 -> damka (bia�a)
extern vector<vector<int> > g_Board;
extern const vector<vector<int> > g_BoardDefault;

// Tablica reprezentuj�ca mo�liwe ruchy wybranego piona
// 0 -> brak akcji
// 1-4 -> pion do zbicia
// 5-8 -> pole na kt�re mo�liwy jest ruch
extern vector<vector<int> > g_SelectedMoves;

// Tablice reprezentuj�ce mo�liwe zbicia
// 0 -> brak akcji
// 1 -> pion do zbicia
// 2 -> pion mog�cy dokona� bicia
extern vector<vector<int> > g_WhiteCaptures;
extern vector<vector<int> > g_RedCaptures;

// Tablice przechowuj�ce dane g��wnego pola tekstowego
extern vector<string> g_MainFieldText;
extern vector<SDL_Color> g_MainFieldColor;

// Struktura przycisku
struct Button
{
	SDL_Rect frame;	// Ramka przycisku
	SDL_Rect rect;	// Wype�nienie przycisku
	string text;	// Tekst przycisku
	bool hovered;	// Najechano myszk�
	bool selected;	// Wybrano przycisk (ustawienia)
};

// Tablica przechowuj�ca przyciski
extern vector<Button> g_ButtonArray;

#endif
