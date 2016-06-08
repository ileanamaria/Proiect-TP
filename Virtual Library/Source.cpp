#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>

HWND _hwnd;
HINSTANCE _hInstance;
MSG  msg;
int _nCmdShow;
HDC hDC;
PAINTSTRUCT Ps;
HFONT	    font;
LOGFONT LogFont;

//Procedura principala
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Paint_Wnd();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_hInstance = hInstance;
	_nCmdShow = nCmdShow;

	WNDCLASSW wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"MainWindow";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&wc);

	_hwnd = CreateWindowW(L"MainWindow", L"Catalog virtual",
		WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		0, 0, 800, 768, NULL, NULL, hInstance, NULL);

	ShowWindow(_hwnd, nCmdShow);
	UpdateWindow(_hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){


	switch (msg) {

	case WM_PAINT:
		Paint_Wnd();
		break;

	case WM_CREATE:
		
		break;

	case WM_COMMAND:
		
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void Paint_Wnd(){
	hDC = BeginPaint(_hwnd, &Ps);
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;


	//Setam Datele Fontului
	LogFont.lfStrikeOut = 0;
	LogFont.lfUnderline = 1;
	LogFont.lfWeight = FW_BOLD;						//Setam sa fie bold
	LogFont.lfHeight = 42;
	LogFont.lfEscapement = 0;
	LogFont.lfItalic = TRUE;

	qLineColor = RGB(165, 42, 42);
	font = CreateFontIndirect(&LogFont);
	SelectObject(hDC, font);						//Selectam Fontul
	SetTextColor(hDC, qLineColor);					//Setam coloarea
	TextOut(hDC, 0, 0, "My Virtual Library", 18);

	LogFont.lfWeight = 0;
	LogFont.lfUnderline = 0;
	LogFont.lfHeight = 35;
	qLineColor = RGB(205, 133, 63);
	font = CreateFontIndirect(&LogFont);
	SelectObject(hDC, font);
	SetTextColor(hDC, qLineColor);
	TextOut(hDC, 30, 52, "Add a book:", 11);

	LogFont.lfHeight = 32;
	SelectObject(hDC, font);
	TextOut(hDC, 470, 63, "\"A  book  is  a  gift", 21);
	TextOut(hDC, 495, 97, "you can open", 12);
	TextOut(hDC, 470, 131, "again  and  again.\"", 19);
	LogFont.lfHeight = 20;
	SelectObject(hDC, font);
	TextOut(hDC, 0, 245, "Show library sorten by:", 23);

	DeleteObject(font);
}