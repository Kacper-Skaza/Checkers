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

#ifndef CHECKERS_MENU_HPP
#define CHECKERS_MENU_HPP
using namespace std;

// Funkcja wyświetlająca menu
void Menu_render();

// Funkcja rysująca menu
void Menu_draw();

// Funkcja obsługująca eventy w menu
void Menu_event(SDL_Event event, bool &quit, bool &error);

#endif
