/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: WinMain.cpp - An application that will allow the user to send and receive characters through
-- the serial port and display it on the screen.
--
-- PROGRAM: DataCommAssignment02
--
-- FUNCTIONS:
-- int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
-- DWORD WINAPI ConnectModeThread(LPVOID n)
-- LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
-- void drawTag(vector<Tag> tTags, int index)
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe, Dhivya Manohar
--
-- PROGRAMMER: Carson Roscoe, Dhivya Manohar
--
-- NOTES:
--
----------------------------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include "WinMain.h"
#include "ConnectMode.h"

//Name of the assignment to be drawn on the top.
LPCWSTR Name = TEXT("RFID Reader");

//Handle to the window
HWND hwnd;

//Handle to the ConnectMode thread.
HANDLE hThread;

//ConnectMode thread ID.
DWORD threadID;

//Reference to the ConnectMode object used when reading from the RFID.
ConnectMode connectMode;

//Vector of all tags currently read by the RFID.
std::vector<Tag> lastTags;

#pragma warning (disable: 4096)
#pragma warning (disable: 4996)

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinMain
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
--
-- RETURNS: int.
--
-- NOTES:
-- Starting point of the application. Used to setup the window and then trigger the ConnectMode thread which will
-- establish the connection to the serial port and handle reading/writing to the serial port.
----------------------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
	LPSTR lspszCmdParam, int nCmdShow)
{
	MSG Msg;
	WNDCLASSEX Wcl;

	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = CS_HREDRAW|CS_VREDRAW;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = Name;

	Wcl.lpszMenuName = TEXT("MYMENU"); // The menu Class
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	if (!RegisterClassEx(&Wcl))
		return 0;
	

	hwnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW, 10, 10, 600, 400, NULL, NULL, hInst, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	connectMode = ConnectMode(hwnd);
	hThread = CreateThread(NULL, 0, &ConnectModeThread, 0, 0, &threadID);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ConnectModeThread
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: DWORD WINAPI ConnectModeThread(LPVOID n)
--
-- RETURNS: DWORD.
--
-- NOTES:
-- Function called when creating the ConnectMode thread. This method makes the connectMode object establish
-- its connection to the serial port and then send it into a loop to read from the port.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI ConnectModeThread(LPVOID n) {
	connectMode.EstablishConnection();
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--
-- RETURNS: LRESULT.
--
-- NOTES:
-- Function that is used to take the user input from the menu and painting to the screen.
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT paintstruct;
	RECT rect;
	TEXTMETRIC tm;
	SIZE size;

	int response;
	static unsigned k = 0;

	switch (Message)
	{

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_HELP:
			MessageBox(NULL, TEXT("Click connect to start reading tags. Click disconnect to stop reading.")
				, TEXT("Terminal Emulator Help"), NULL);
			break;
		case IDM_CONNECT:
			if (hThread == NULL) {
				connectMode = ConnectMode(hwnd);
				hThread = CreateThread(NULL, 0, &ConnectModeThread, 0, 0, &threadID);
			}
			else {
				MessageBox(NULL, TEXT("The device is already connected."), TEXT("Already Connected"), NULL);
			}
			break;
		case IDM_DISCONNECT:
			TerminateThread(hThread, 0);
			CloseHandle(hThread);
			hThread = NULL;
			break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &paintstruct);
		GetClientRect(hwnd, &rect);
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 100, 100, NULL);
		SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);
		drawTag();
		EndPaint(hwnd, &paintstruct);
		return 0L;
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: drawTag
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void drawTag(
--					vector<Tag> tTags is our list of Tag structure found in WinMain.h
--					int index of the tag to be highlighted. Set to -1 if none.)
--
-- RETURNS: void.
--
-- NOTES:
-- Takes in a vector of Tags to be draw to the screen. These each contain a friendly (the name to be drawn) and
-- a type, both as string values. If index is not -1, it will highlight the tag's info assiciated with that index.
----------------------------------------------------------------------------------------------------------------------*/
void drawTag(std::vector<Tag> tTags, int index) {
	HDC hdc = GetDC(hwnd); // get device context
	int y = 0;
	for (int i = 0; i < tTags.size(); i++) {
		SIZE size;

		std::wstring stemp = std::wstring(tTags[i].friendly.begin(), tTags[i].friendly.end());
		LPCWSTR sw = stemp.c_str();

		GetTextExtentPoint32(hdc, sw, 1, &size);

		if (i == index)
			SetBkColor(hdc, RGB(15, 255, 15));
		else
			SetBkColor(hdc, RGB(255, 255, 255));

		TextOut(hdc, 1, 1 + y, sw, tTags[0].friendly.size());
		y += size.cy + 1;

		stemp = std::wstring(tTags[i].type.begin(), tTags[i].type.end());
		sw = stemp.c_str();

		TextOut(hdc, 20, 1 + y, sw, tTags[0].type.size());
		y += size.cy + 4;
	}
	lastTags = tTags;
	ReleaseDC(hwnd, hdc); // Release device context
}

//FUNCTION OVERLOAD: Being called without specifying the 2nd pramater (the index of which one to highlight)
//results in drawTag being called with a highlight index of -1 (none will be highlighted)
void drawTag(std::vector<Tag> tTags) {
	drawTag(tTags, -1);
}

//FUNCTION OVERLOAD: Being called without any parameters calls the previous version and sends the last known
//tags to be drawn. Essentially reloads the last version called.
void drawTag() {
	drawTag(lastTags);
}