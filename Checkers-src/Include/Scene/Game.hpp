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

#ifndef CHECKERS_GAME_HPP
#define CHECKERS_GAME_HPP
using namespace std;

// Funkcja wyœwietlaj¹ca grê
void Game_render();

// Funkcja rysuj¹ca planszê
void Game_draw_board();

// Funkcja rysuj¹ca pionki
void Game_draw_pieces();

// Funkcja rysuj¹ca znaczniki
void Game_draw_markers();

// Funkcja rysuj¹ca interfejs
void Game_draw_UI();

// Funkcja obs³uguj¹ca eventy w grze
void Game_event(SDL_Event event, bool &quit, bool &error, int &l_LastRow, int &l_LastCol);

#endif
