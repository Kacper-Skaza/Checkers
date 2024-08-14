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

#ifndef CHECKERS_GAMELOGIC_HPP
#define CHECKERS_GAMELOGIC_HPP
using namespace std;

// Funkcja dodaj¹ca tekst do g³ównego pola tekstowego
void MainField_append(string txt, SDL_Color color);

// Funkcja tworz¹ca now¹ grê
void Game_new();

// Funkcja ³aduj¹ca scenê gry
void Game_scene_load();

// Funkcja odpowiadaj¹ca za rozgrywkê z komputerem
void SI_turn(int random_num);

// Funkcja poruszaj¹ca pionkami
void Move_piece(int row, int col);

// Funkcja kalkuluj¹ca mo¿liwe ruchy
void Calculate_moves();

#endif
