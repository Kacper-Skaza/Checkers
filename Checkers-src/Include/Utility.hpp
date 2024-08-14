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

#ifndef CHECKERS_UTILITY_HPP
#define CHECKERS_UTILITY_HPP
using namespace std;

// Funkcja pomocnicza konwertuj¹ca string na UTF8
string Convert_to_UTF(string txt);

// Funkcja pomocnicza rysuj¹ca wype³nione okrêgi
void Draw_filled_circle(int x, int y, int radius);

// Funkcja pomocnicza rysuj¹ca puste prostok¹ty
void Draw_empty_rect(int x, int y, int width, int height, int thickness);

// Funkcja pomocnicza rysuj¹ca pionek
void Draw_piece(int val, int x, int y, int radius, bool DrawShadow);

// Funkcja pomocnicza rysuj¹ca przycisk
void Draw_button(Button &button);

// Funkcja pomocnicza rysuj¹ca tekst
void Draw_text(string txt, int x, int y, string pos, TTF_Font* TxtFont, SDL_Color TxtColor, bool DrawBg);

// Funkcja pomocnicza tworz¹ca okno potwierdzenia
bool Confirm_request(string line1, string line2);

// Funkcja debuguj¹ca
void Debug();

#endif
