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
#include "../Include/Utility.hpp"

// Funkcja pomocnicza konwertuj¹ca string na UTF8
string Convert_to_UTF(string txt)
{
	vector<const char*> utf = {u8"\u00A9", u8"\u0105", u8"\u0107", u8"\u0119", u8"\u0142", u8"\u0144", u8"\u00F3", u8"\u015B", u8"\u017A", u8"\u017C"};
	vector<char> ansi = {'©', '¹', 'æ', 'ê', '³', 'ñ', 'ó', 'œ', 'Ÿ', '¿'};
	string temp="";

	for (int i=0; i<txt.size(); i++)
	{
		bool found=false;

		for (int j=0; j<ansi.size(); j++)
		{
			if (txt[i] == ansi[j])
			{
				temp += utf[j];
				found = true;
				break;
			}
		}

		if (found == false)
			temp += txt[i];
	}

	return temp;
}



// Funkcja pomocnicza rysuj¹ca wype³nione okrêgi
void Draw_filled_circle(int x, int y, int radius)
{
	for (int dy=1; dy<=radius; dy++)
	{
		int dx = floor(sqrt(2.0*radius*dy - dy*dy));
		SDL_RenderDrawLine(g_Renderer, x-dx, y+dy-radius, x+dx, y+dy-radius);
		SDL_RenderDrawLine(g_Renderer, x-dx, y-dy+radius, x+dx, y-dy+radius);
	}
}

// Funkcja pomocnicza rysuj¹ca puste prostok¹ty
void Draw_empty_rect(int x, int y, int width, int height, int thickness)
{
	for (int i=0; i<thickness; i++)
	{
		// Utwórz prostok¹t
		SDL_Rect rect;
		rect.x = x + i;
		rect.y = y + i;
		rect.w = width - 2*i;
		rect.h = height - 2*i;

		// Narysuj prostok¹t
		SDL_RenderDrawRect(g_Renderer, &rect);
	}
}

// Funkcja pomocnicza rysuj¹ca pionek
void Draw_piece(int val, int x, int y, int radius, bool DrawShadow)
{
	// Narysuj cieñ
	if (DrawShadow == true)
	{
		SDL_SetRenderDrawColor(g_Renderer, COLOR_SHADOW.r, COLOR_SHADOW.g, COLOR_SHADOW.b, COLOR_SHADOW.a);
		Draw_filled_circle(x-12, y+12, radius);
	}

	// Ko³o #1
	if (val > 0)
		SDL_SetRenderDrawColor(g_Renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
	else
		SDL_SetRenderDrawColor(g_Renderer, COLOR_RED.r, COLOR_RED.g, COLOR_RED.b, COLOR_RED.a);
	Draw_filled_circle(x, y, radius);

	if (abs(val) == 1) // Zwyk³y pion (dla damki nic wiêcej nie jest rysowane)
	{
		// Ko³o #2
		int temp_radius = radius * 0.60;
		SDL_SetRenderDrawColor(g_Renderer, COLOR_SHADOW.r, COLOR_SHADOW.g, COLOR_SHADOW.b, COLOR_SHADOW.a);
		Draw_filled_circle(x, y, temp_radius);

		// Ko³o #3
		temp_radius = radius * 0.54;
		if (val > 0)
			SDL_SetRenderDrawColor(g_Renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
		else
			SDL_SetRenderDrawColor(g_Renderer, COLOR_RED.r, COLOR_RED.g, COLOR_RED.b, COLOR_RED.a);
		Draw_filled_circle(x, y, temp_radius);
	}
}

// Funkcja pomocnicza rysuj¹ca przycisk
void Draw_button(Button &button)
{
	// Zmienne
	SDL_Color TxtColor;
	string txt="";

	// Wybierz kolor tekstu
	if (button.selected == true)
		TxtColor = COLOR_LIME;
	else if (button.hovered == true)
		TxtColor = COLOR_ORANGE;
	else
		TxtColor = COLOR_WHITE;

	// SprawdŸ czy przycisk jest wybrany (ustawienia)
	if (button.selected == true)
		txt = (">> " + button.text + " <<").c_str();
	else
		txt = button.text.c_str();

	// Narysuj prostok¹ty
	SDL_SetRenderDrawColor(g_Renderer, COLOR_SHADOW.r, COLOR_SHADOW.g, COLOR_SHADOW.b, COLOR_SHADOW.a);
	SDL_RenderFillRect(g_Renderer, &button.frame);
	SDL_SetRenderDrawColor(g_Renderer, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b, COLOR_GRAY.a);
	SDL_RenderFillRect(g_Renderer, &button.rect);

	// Narysuj tekst
	Draw_text(txt, button.rect.x + button.rect.w/2, button.rect.y + button.rect.h/2, "center-center", g_Font_72, TxtColor, false);
}

// Funkcja pomocnicza rysuj¹ca tekst
void Draw_text(string txt, int x, int y, string pos, TTF_Font* TxtFont, SDL_Color TxtColor, bool DrawBg)
{
	// Warunek pocz¹tkowy
	if (txt == "" || txt == " ")
		return;

	// Obs³uga znaków specjalnych
	txt = Convert_to_UTF(txt);

	// Utwórz powierzchniê z tekstem
	SDL_Surface* textSurface;
	if (DrawBg == true)
		textSurface = TTF_RenderUTF8_Shaded(TxtFont, txt.c_str(), TxtColor, COLOR_BLACK);
	else
		textSurface = TTF_RenderUTF8_Blended(TxtFont, txt.c_str(), TxtColor);

	// Utwórz teksturê z powierzchni
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_Renderer, textSurface);

	// Pobierz wymiary tekstury
	int textWidth = textSurface->w;
	int textHeight = textSurface->h;

	// Ustaw pozycjê tekstu
	SDL_Rect dstRect;
	if (pos == "center" || pos == "center-center")
		dstRect = {x - textWidth/2, y - textHeight/2, textWidth, textHeight};
	else if (pos == "top-left")
		dstRect = {x, y, textWidth, textHeight};
	else if (pos == "top-center")
		dstRect = {x - textWidth/2, y, textWidth, textHeight};
	else if (pos == "top-right")
		dstRect = {x - textWidth, y, textWidth, textHeight};
	else if (pos == "bottom-left")
		dstRect = {x, y - textHeight, textWidth, textHeight};
	else if (pos == "bottom-center")
		dstRect = {x - textWidth/2, y - textHeight, textWidth, textHeight};
	else if (pos == "bottom-right")
		dstRect = {x - textWidth, y - textHeight, textWidth, textHeight};
	else
	{
		if (DEVELOPER_MODE == true)
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Wrong text position !!!", "Wrong text position !!!", NULL);

		// Zwolnij zasoby
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		return;
	}

	// Renderuj teksturê
	SDL_RenderCopy(g_Renderer, textTexture, nullptr, &dstRect);

	// Zwolnij zasoby
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}



// Funkcja pomocnicza tworz¹ca okno potwierdzenia
bool Confirm_request(string line1, string line2)
{
	// Obs³uga znaków specjalnych
	line1 = Convert_to_UTF(line1);
	line2 = Convert_to_UTF(line2);

	// Zmienne
	int Window_Width=0, Window_Height=0, Renderer_Width=0, Renderer_Height=0;

	// Pobierz rozmiary g³ównego okna oraz renderera
	SDL_GetWindowSize(g_Window, &Window_Width, &Window_Height);
	SDL_RenderGetLogicalSize(g_Renderer, &Renderer_Width, &Renderer_Height);

	// Przyciemnij ekran g³ównego okna
	SDL_RenderSetLogicalSize(g_Renderer, Window_Width, Window_Height);
	SDL_SetRenderDrawColor(g_Renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, 128);
	SDL_Rect TempRect = {0, 0, Window_Width, Window_Height};
	SDL_RenderFillRect(g_Renderer, &TempRect);
	SDL_RenderPresent(g_Renderer);
	SDL_RenderSetLogicalSize(g_Renderer, Renderer_Width, Renderer_Height);

	// Utwórz okno potwierdzenia
	SDL_Window* Temp_Window = SDL_CreateWindow(line1.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (g_DisplayMode.w*5) / 12, (g_DisplayMode.h*5) / 12,
												SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	SDL_Renderer* Temp_Renderer = g_Renderer;
	g_Renderer = SDL_CreateRenderer(Temp_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_RenderSetLogicalSize(g_Renderer, 1600, 900);

	// Tekst przycisków
	vector<string> ButtonsText(0);
	if (g_Lang == "pol")
		ButtonsText = {"Tak", "Nie"};
	else
		ButtonsText = {"Yes", "No"};

	// Zmienne przycisków
	g_ButtonArray.resize(ButtonsText.size());
	int buttonWidth = 500;
	int buttonHeight = 115;
	int buttonSpacing = 20;
	int startY = 600;
	int startX = (1600-buttonSpacing)/2 - buttonWidth;

	// Obs³uga pêtli g³ównej
	bool quit=false, answer=false;
	SDL_Event event;

	// Pêtla g³ówna
	while (!quit)
	{
		// Event
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEBUTTONDOWN) // Klikanie
			{
				int x = event.button.x;
				int y = event.button.y;
				SDL_Point Point={x, y};

				for (int i=0; i<g_ButtonArray.size(); i++)
				{
					if (SDL_PointInRect(&Point, &g_ButtonArray[i].frame) == true)
					{
						if (DEVELOPER_MODE == true)
							cout<<"Clicked on: '"<<g_ButtonArray[i].text<<"'"<<endl;

						if (i == 0)			// "Yes"
						{
							answer = true;
							quit = true;
						}
						else if (i == 1)	// "No"
						{
							answer = false;
							quit = true;
						}

						break;
					}
				}
			}
			else if (event.type == SDL_MOUSEMOTION) // Rodzaj kursora
			{
				int x = event.motion.x;
				int y = event.motion.y;
				SDL_Point Point={x, y};
				bool isMouseOverButton=false;

				for (int i=0; i<g_ButtonArray.size(); i++)
				{
					if (SDL_PointInRect(&Point, &g_ButtonArray[i].frame) == true)
					{
						g_ButtonArray[i].hovered = true;
						isMouseOverButton = true;
					}
					else
					{
						g_ButtonArray[i].hovered = false;
					}
				}

				if (isMouseOverButton == false)
					SDL_SetCursor(g_DefaultCursor);
				else
					SDL_SetCursor(g_HandCursor);
			}
		}

		// Przygotowanie
		SDL_SetRenderDrawColor(g_Renderer, COLOR_DARKRED.r, COLOR_DARKRED.g, COLOR_DARKRED.b, COLOR_DARKRED.a); // Ustaw kolor t³a
		SDL_RenderClear(g_Renderer); // Wyczyœæ ekran

		// Narysuj zapytanie
		Draw_text(line1, 800, 200, "top-center", g_Font_72, COLOR_WHITE, false);
		Draw_text(line2, 800, 300, "top-center", g_Font_72, COLOR_WHITE, false);

		for (int i=0; i<g_ButtonArray.size(); i++)
		{
			// Nadaj w³aœciowoœci
			g_ButtonArray[i].frame = {startX + i*(buttonWidth+buttonSpacing), startY, buttonWidth, buttonHeight};
			g_ButtonArray[i].rect = {startX + i*(buttonWidth+buttonSpacing) + 10, startY + 10, buttonWidth - 10, buttonHeight - 10};
			g_ButtonArray[i].text = ButtonsText[i];
			g_ButtonArray[i].selected = false;

			// Narysuj przycisk
			Draw_button(g_ButtonArray[i]);
		}

		// Wyœwietl na ekranie
		SDL_RenderPresent(g_Renderer);

		// OpóŸnienie oraz wyœwietlanie okna "na górze"
		for (int i=0; i<10; i++)
		{
			SDL_RaiseWindow(Temp_Window);
			SDL_Delay(1);
		}
	}

	// Zwolnij zasoby okna potwierdzenia
	SDL_DestroyRenderer(g_Renderer);
	g_Renderer = Temp_Renderer;
	SDL_DestroyWindow(Temp_Window);
	SDL_RaiseWindow(g_Window);

	return answer;
}



// Funkcja debuguj¹ca
void Debug()
{
	if (DEVELOPER_MODE == false)
		return;

	cout<<endl<<endl<<"===== START REPORT ====="<<endl;

	cout<<endl<<"g_Board = "<<endl;
	for (int row=0; row<CELL_NUM; row++)
	{
		cout<<"{";

		for (int col=0; col<CELL_NUM; col++)
		{
			cout<<g_Board[row][col];
			if (col != CELL_NUM-1)
				cout<<", ";
		}

		cout<<"},"<<endl;
	}

	cout<<endl<<"g_SelectedMoves = "<<endl;
	for (int row=0; row<CELL_NUM; row++)
	{
		cout<<"{";

		for (int col=0; col<CELL_NUM; col++)
		{
			cout<<g_SelectedMoves[row][col];
			if (col != CELL_NUM-1)
				cout<<", ";
		}

		cout<<"},"<<endl;
	}

	cout<<endl<<"g_WhiteCaptures = "<<endl;
	for (int row=0; row<CELL_NUM; row++)
	{
		cout<<"{";

		for (int col=0; col<CELL_NUM; col++)
		{
			cout<<g_WhiteCaptures[row][col];
			if (col != CELL_NUM-1)
				cout<<", ";
		}

		cout<<"},"<<endl;
	}

	cout<<endl<<"g_RedCaptures = "<<endl;
	for (int row=0; row<CELL_NUM; row++)
	{
		cout<<"{";

		for (int col=0; col<CELL_NUM; col++)
		{
			cout<<g_RedCaptures[row][col];
			if (col != CELL_NUM-1)
				cout<<", ";
		}

		cout<<"},"<<endl;
	}

	cout<<endl<<">> Zmienne globalne <<"<<endl;
	cout<<"g_TargetFPS = "<<g_TargetFPS<<endl;
	cout<<"g_Scene = "<<g_Scene<<endl;
	cout<<"g_Lang = "<<g_Lang<<endl;
	cout<<"g_RenderFrame = "<<g_RenderFrame<<endl;

	cout<<endl<<">> Zmienne gry <<"<<endl;
	cout<<"g_Score = {"<<g_Score.first<<", "<<g_Score.second<<"}"<<endl;
	cout<<"g_Turn = "<<g_Turn<<endl;
	cout<<"g_PlayerSI = "<<g_PlayerSI<<endl;
	cout<<"g_CalculateSI = "<<g_CalculateSI<<endl;
	cout<<"g_SelectedRow = "<<g_SelectedRow<<endl;
	cout<<"g_SelectedCol = "<<g_SelectedCol<<endl;
	cout<<"g_CanSelect = "<<g_CanSelect<<endl;
	cout<<"g_WhiteCanCapture = "<<g_WhiteCanCapture<<endl;
	cout<<"g_RedCanCapture = "<<g_RedCanCapture<<endl;
	cout<<"g_WhiteCanMove = "<<g_WhiteCanMove<<endl;
	cout<<"g_RedCanMove = "<<g_RedCanMove<<endl;

	cout<<endl<<"===== END REPORT ====="<<endl<<endl;
}

