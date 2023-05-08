
#include "TerminalRenderer.h"

/*
Windows Only

Can be used to turn the Terminal into a render screen
The Terminal has a type ScreenContent that stores the symbols buffer and attributes


The terminal should have the same Width and Height as the terminal screen for better results


*/


int main() {
	//DEMO


	int nScreenWidth = 400;
	int nScreenHeight = 120;


	//Initializing
	TerminalScreen terminal = TerminalScreen(nScreenWidth, nScreenHeight);
	terminal.setAsActiveTerminal();
	ScreenContent screenContent = terminal.getScreenContent();
	screenContent.setAllSymbols(' ');

	//More functions
	//screenContent.DrawLineAbsolute(BACKGROUND_WHITE, ' ', { 0,60 }, { 300,100 });
	//screenContent.DrawTriangleAbsolute(BACKGROUND_GREEN, ' ', { 60,60 }, { 300,50 }, { 60,0 });


	//Using to test
	Square sqr = Square({0,0},2,3,BACKGROUND_BLUE);
	Square sqr2 = Square({ 0,0 }, 2, 1, BACKGROUND_RED);
	Square sqr3 = Square({ 100,100 }, 1, 1, BACKGROUND_GREEN);
	Square sqr4 = Square({ 0,0 }, 3, 2, BACKGROUND_BLUE | BACKGROUND_RED);
	while (true)
	{
	
		sqr.Move(screenContent);
		sqr2.Move(screenContent);
		sqr3.Move(screenContent);
		sqr4.Move(screenContent);

		terminal.DisplayBuffer();
	}

	return 0;
}