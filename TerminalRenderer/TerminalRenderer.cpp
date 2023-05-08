#include "TerminalRenderer.h"

#include <iostream>
using namespace std;

#include <Windows.h>

#define FOREGROUND_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED


ScreenContent::ScreenContent() {
		nScreenHeight = 0;
		nScreenWidth = 0;
		symbolsColorsList = NULL;
		symbolsList = NULL;
	}

ScreenContent::ScreenContent(int nScreenWidth, int nScreenHeight) {



		this->nScreenHeight = nScreenHeight;
		this->nScreenWidth = nScreenWidth;

		symbolsList = new wchar_t[nScreenHeight * nScreenWidth + 1];
		symbolsColorsList = new WORD[nScreenHeight * nScreenWidth + 1];

		std::fill_n(symbolsColorsList, nScreenHeight * nScreenWidth, FOREGROUND_WHITE);

		this->setAllSymbols('#');
		this->setAllSymbolsAttributes(FOREGROUND_WHITE);
	}
	COORD ScreenContent::getScreenDimensions() {
		//Returns the writeable screen dimensions
		return { (short)nScreenWidth,(short)nScreenHeight };
	}
	wchar_t* ScreenContent::getSymbolsBuffer() {
		/*
			Grants access to the symbols buffers
			Much faster access
		*/
		return this->symbolsList;
	}
	WORD* ScreenContent::getSymbolsAttributesBuffer() {
		/*Grants access to the attribute buffer
		The available attributes are the ones defined in windows.h
		*/
		return this->symbolsColorsList;
	}

	int ScreenContent::getFullSymbolSize() {
		//Returns the size of the writeable symbols buffer
		return nScreenWidth * nScreenHeight;
	}
	void ScreenContent::setAllSymbolsAttributes(WORD color) {
		//Sets all the attributes to color
		std::fill_n(symbolsColorsList, nScreenHeight * nScreenWidth, color);
	}
	void ScreenContent::setAllSymbols(wchar_t symbol) {
		//Sets all the symbols to symbol
		std::fill_n(symbolsList, nScreenHeight * nScreenWidth, symbol);
	}
	void ScreenContent::setSymbolsBuffer(wchar_t* newList) {
		/*Changes the symbol buffer
		Gotta have the same dimmensions as the previous
		*/

		free(this->symbolsList);
		this->symbolsList = newList;
	}
	void ScreenContent::setAsRigidColorAtPosition(WORD Color, int pos) {
		/*
		Turns the position into a square of the desired color
		*/
		symbolsList[pos] = ' ';
		symbolsColorsList[pos] = Color;

	}
	void ScreenContent::setAsRigidColorAtPosition(WORD Color, COORD coord) {
		//Same as above
		symbolsList[coord.X * nScreenWidth + coord.Y] = ' ';
		symbolsColorsList[coord.X * nScreenWidth + coord.Y] = Color;
		wchar_t** l = (wchar_t**)symbolsList;


	}
	void ScreenContent::DrawLineAbsolute(WORD attribute, wchar_t symbol, COORD cStart, COORD  cFinish) {
		//Bresenham Algorithm
		int x2 = cFinish.X;
		int y2 = cFinish.Y;
		int x1 = cStart.X;
		int y1 = cStart.Y;

		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
		int sx = x1 < x2 ? 1 : -1;
		int sy = y1 < y2 ? 1 : -1;
		int err = dx - dy;

		while (true) {
			symbolsList[y1 * nScreenWidth + x1] = symbol;
			symbolsColorsList[y1 * nScreenWidth + x1] = attribute;
			if (x1 == x2 && y1 == y2) break;
			int e2 = 2 * err;
			if (e2 > -dy) {
				err -= dy;
				x1 += sx;
			}
			if (e2 < dx) {
				err += dx;
				y1 += sy;
			}
		}

	}
	void ScreenContent::DrawTriangleAbsolute(WORD attribute, wchar_t symbol, COORD v1, COORD  v2, COORD v3) {
		/*
			Given the edges, draw a triangle
		*/
		DrawLineAbsolute(attribute, symbol, v1, v2);
		DrawLineAbsolute(attribute, symbol, v2, v3);
		DrawLineAbsolute(attribute, symbol, v3, v1);

	}
	void ScreenContent::DrawSquareAbsolute(WORD attribute, wchar_t symbol, COORD a1, COORD a2) {

		DrawLineAbsolute(attribute, symbol, a1, { a2.X,a1.Y });
		DrawLineAbsolute(attribute, symbol, { a2.X,a1.Y }, a2);
		DrawLineAbsolute(attribute, symbol, a2, { a1.X,a2.Y });
		DrawLineAbsolute(attribute, symbol, { a1.X,a2.Y }, a1);

	}
	void ScreenContent::insertStringOnScreen(string str, WORD attribute, COORD pos) {
		/*
			Given a string, inserts it into the screen on the desired position
			\n are not inserted and are considered a newline
			reads until \0
			Out of bounds symbols aren't rendered
		*/
		int y = pos.Y;
		int x = pos.X;
		int i = 0;
		while (str[i] != '\0') {

			int listPos = y * (nScreenWidth)+x;
			if (str[i] == '\n') {
				y++;
				x = pos.X;
				i++;
				continue;
			}
			else if (listPos <= nScreenWidth * nScreenHeight) {

				symbolsList[listPos] = str[i];
				symbolsColorsList[listPos] = attribute;

			}
			x++;
			i++;


		}


	}




	TerminalScreen::TerminalScreen(int nScreenWidth, int nScreenHeight) {

		this->screenContent = ScreenContent(nScreenWidth, nScreenHeight);
		this->dwBytesWritten = 0;
		this->hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);


	}
	void TerminalScreen::DisplayBuffer() {
		/*
		Updates the terminal to display the content of the buffer
		The current Terminal should be the active one

		*/
		wchar_t* symbolList = screenContent.getSymbolsBuffer();
		WORD* attributeList = screenContent.getSymbolsAttributesBuffer();
		WriteConsoleOutputCharacter(hConsole, symbolList, screenContent.getFullSymbolSize(), { 0,0 }, &dwBytesWritten);
		WriteConsoleOutputAttribute(hConsole, attributeList, screenContent.getFullSymbolSize(), { 0,0 }, &dwBytesWritten);

	}
	void TerminalScreen::setAsActiveTerminal() {
		/*
			Set the current terminal as the active one
		*/
		SetConsoleActiveScreenBuffer(hConsole);
	}
	ScreenContent TerminalScreen::getScreenContent() {
		/*
			Grants access to the screenContent, making symbols changes faster
		*/
		return screenContent;
	}


	Square::Square(COORD startPos, short xSpeed, short ySpeed, WORD color) {

		i = startPos.X;
		j = startPos.Y;

		di = xSpeed;
		dj = ySpeed;

		this->color = color;
	}
	void Square::DrawOnScreen(ScreenContent screenContent, bool bClearTrace) {

		screenContent.DrawSquareAbsolute(color, ' ', { i,j }, { short(10 + i),short(10 + j) });
	}
	void Square::Move(ScreenContent screenContent, bool bCleanTrail) {
		if (bCleanTrail)
			Clear(screenContent);
		j += dj;
		i += di;
		if (i < 0 || i + 10 > screenContent.getScreenDimensions().X - 1) {
			di *= -1;
			i += di;
		}
		if (j < 0 || j + 10 > screenContent.getScreenDimensions().Y - 1) {
			dj *= -1;
			j += dj;
		}
		DrawOnScreen(screenContent);
	}
	void Square::Clear(ScreenContent screenContent) {

		screenContent.DrawSquareAbsolute(NULL, ' ', { i,j }, { short(10 + i),short(10 + j) });
	}
