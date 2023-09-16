#pragma once

#include <Windows.h>
#include <string>


#include "../Utils/Math.h"


namespace Console {

	struct WIN_SIZE {
		int w = 250;
		int h = 250;
	}static s_WindowSize;

	std::pair<int, int> getTerminalSize() {

		CONSOLE_SCREEN_BUFFER_INFOEX consolesize;

		consolesize.cbSize = sizeof(consolesize);

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		GetConsoleScreenBufferInfoEx(hConsole, &consolesize);


		int columns = consolesize.srWindow.Right - consolesize.srWindow.Left + 1;
		int rows = consolesize.srWindow.Bottom - consolesize.srWindow.Top + 1;

		return { columns, rows };

	}

	void changeZoom(int fontX, int fontY) {

		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_FONT_INFOEX orig, current;

		current.cbSize = sizeof(CONSOLE_FONT_INFOEX);

		COORD FontSize;

		FontSize.X = fontX;
		FontSize.Y = fontY;

		current.dwFontSize = FontSize;
		current.FontWeight = 600;
		current.FontFamily = FF_ROMAN;


		if (GetCurrentConsoleFontEx(hStdin, FALSE, &orig))
			std::cout << "Got\n";
		else
			std::cout << GetLastError();

		if (SetCurrentConsoleFontEx(hStdout, FALSE, &current))
			std::cout << "\nSet";
		else
			std::cout << std::endl << GetLastError();
	}

	void setTerminalScreenResolution(int width, int height)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFOEX consolesize;
		consolesize.cbSize = sizeof(consolesize);

		GetConsoleScreenBufferInfoEx(hConsole, &consolesize);
		COORD c;
		c.X = width;
		c.Y = height;
		consolesize.dwSize = c;


		s_WindowSize.h = height;
		s_WindowSize.w = width;

		consolesize.srWindow.Left = 0;
		consolesize.srWindow.Right = width;
		consolesize.srWindow.Top = 0;
		consolesize.srWindow.Bottom = height;


		SetConsoleScreenBufferInfoEx(hConsole, &consolesize);
		SetConsoleScreenBufferSize(hConsole, c);
		SetConsoleActiveScreenBuffer(hConsole);

		CONSOLE_CURSOR_INFO     cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = FALSE; // set the cursor visibility
		SetConsoleCursorInfo(hConsole, &cursorInfo);

		system("cls");

		std::pair<int, int> winSize = getTerminalSize();



	}

	void setFullScreen() {

		system("mode con COLS=700");
		ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
		SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);


	}

	void setTitle(const std::string& str) {
		std::basic_stringstream<wchar_t> ss;
		ss << str.c_str();
		SetConsoleTitleW(ss.str().c_str());
	}

}