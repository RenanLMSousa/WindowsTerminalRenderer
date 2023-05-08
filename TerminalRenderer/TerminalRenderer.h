#pragma once
#include <iostream>
using namespace std;

#include <Windows.h>
#include "Attributes.h"


class ScreenContent {

	/*
	Stores the screen state , such as character, size and attributes
	*/
private:

	int nScreenWidth, nScreenHeight;
	wchar_t* symbolsList;
	WORD* symbolsColorsList;

public:

	ScreenContent();


	ScreenContent(int nScreenWidth, int nScreenHeight);
	COORD getScreenDimensions();
	wchar_t* getSymbolsBuffer();
	WORD* getSymbolsAttributesBuffer();

	int getFullSymbolSize();
	void setAllSymbolsAttributes(WORD color);
	void setAllSymbols(wchar_t symbol);
	void setSymbolsBuffer(wchar_t* newList);
	void setAsRigidColorAtPosition(WORD Color, int pos);
	void setAsRigidColorAtPosition(WORD Color, COORD coord);
	void DrawLineAbsolute(WORD attribute, wchar_t symbol, COORD cStart, COORD  cFinish);
	void DrawTriangleAbsolute(WORD attribute, wchar_t symbol, COORD v1, COORD  v2, COORD v3);
	void DrawSquareAbsolute(WORD attribute, wchar_t symbol, COORD a1, COORD a2);
	void insertStringOnScreen(string str, WORD attribute, COORD pos);

};

class TerminalScreen {

private:
	ScreenContent screenContent;
	DWORD dwBytesWritten;
	HANDLE hConsole;

public:
	TerminalScreen(int nScreenWidth, int nScreenHeight);
	void DisplayBuffer();
	void setAsActiveTerminal();
	ScreenContent getScreenContent();

};
class Square {

private:
	short i;
	short j;

	short di;
	short dj;

	WORD color;

public:
	Square(COORD startPos, short xSpeed, short ySpeed, WORD color);
	void DrawOnScreen(ScreenContent screenContent, bool bClearTrace = true);
	void Move(ScreenContent screenContent, bool bCleanTrail = true);
	void Clear(ScreenContent screenContent);
};