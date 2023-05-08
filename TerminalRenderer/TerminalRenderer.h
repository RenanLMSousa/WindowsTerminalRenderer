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

	//Returns the writeable screen dimensions
	COORD getScreenDimensions();

	/*
	Grants access to the symbols buffers
	Much faster access
	*/
	wchar_t* getSymbolsBuffer();

	/*Grants access to the attribute buffer
	The available attributes are the ones defined in windows.h
	*/
	WORD* getSymbolsAttributesBuffer();

	//Returns the size of the writeable symbols buffer
	int getFullSymbolSize();

	//Sets all the attributes to color
	void setAllSymbolsAttributes(WORD color);

	//Sets all the symbols to symbol
	void setAllSymbols(wchar_t symbol);

	/*Changes the symbol buffer
		Gotta have the same dimmensions as the previous
	*/
	void setSymbolsBuffer(wchar_t* newList);

	/*
	Turns the position into a square of the desired color
	*/
	void setAsRigidColorAtPosition(WORD Color, int pos);
	void setAsRigidColorAtPosition(WORD Color, COORD coord);

	//Draws a line using Bresenham's Algorithm given the pixel coordinates
	void DrawLineAbsolute(WORD attribute, wchar_t symbol, COORD cStart, COORD  cFinish);

	//Draws a triangle given the pixel coordinates
	void DrawTriangleAbsolute(WORD attribute, wchar_t symbol, COORD v1, COORD  v2, COORD v3);

	//Draws a quare guven the pixel coordinates
	void DrawSquareAbsolute(WORD attribute, wchar_t symbol, COORD a1, COORD a2);

	/*
		Given a string, inserts it into the screen on the desired position
		\n are not inserted and are considered a newline
		reads until \0
		Out of bounds symbols aren't rendered
	*/
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
	/*
	Used for fun and testing redering capabilities
	*/
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