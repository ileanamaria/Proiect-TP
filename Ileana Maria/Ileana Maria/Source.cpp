#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include "Header.h"

struct Date_Carte
{
	char Titlu[100];
	char Autor[100];
	char Gen_Carte[100];
	char Editura[100];
};
struct ThreadedBST
{
	Date_Carte date_carte;
	ThreadedBST *left;
	ThreadedBST *right;
	bool is_right;
	bool is_left;
};


HWND _hwnd;
HWND _libraryhwnd, _bookmenuwnd;
HWND Titlu_Carte, Autor_Carte, Gen_Carte, Editura_Carte;
HINSTANCE _hInstance;
MSG  msg;
int _nCmdShow;
int nr_carti = 0;
bool check_WndLibrary = false, check_bookmenu = false;

//Pentru a seta fontul
HDC hDC, MemDCExercising;
PAINTSTRUCT Ps;
HFONT	    font;
LOGFONT LogFont;
HBITMAP bmpExercising;

//Algoritmii pentru TBT
ThreadedBST *root1;
bool cic1 = false;
Date_Carte aux_buff;
char colectie1[1000];

//TBT pentru Titlu
void insert_nodeTitlu(char *titlu){
	ThreadedBST *temp = root1;
	while (1){
		if (strcmp(temp->date_carte.Titlu, titlu)>0){
			if (temp->is_left)
				break;
			temp = temp->left;
		}
		else if (strcmp(temp->date_carte.Titlu, titlu) <= 0){
			if (temp->is_right)
				break;
			temp = temp->right;
		}
		else{
			return;
		}
	}
	//Alocam dinamic
	ThreadedBST *new_node = new ThreadedBST();
	strcpy(new_node->date_carte.Titlu, titlu);
	strcpy(new_node->date_carte.Autor, aux_buff.Autor);
	strcpy(new_node->date_carte.Gen_Carte, aux_buff.Gen_Carte);
	strcpy(new_node->date_carte.Editura, aux_buff.Editura);

	new_node->is_right = new_node->is_left = true;
	if (strcmp(temp->date_carte.Titlu, titlu)<0){
		new_node->left = temp;
		new_node->right = temp->right;
		temp->right = new_node;
		temp->is_right = false;
	}
	else{
		new_node->left = temp->left;
		new_node->right = temp;
		temp->left = new_node;
		temp->is_left = false;
	}
}
void inorder_Titlu(int number){
	int k = 0;
	ThreadedBST *temp = root1;
	ThreadedBST *aux;
	while (1){
		aux = temp;
		temp = temp->right;
		if (!aux->is_right){
			while (!temp->is_left)
				temp = temp->left;
		}
		if (temp == root1)
			break;
		//Cautam elementul
		k++;
		if (number == k)
			sprintf(colectie1, "Title:               %s\nAuthor:          %s\nStyle:              %s\nPublisher:     %s", temp->date_carte.Titlu, temp->date_carte.Autor, temp->date_carte.Gen_Carte, temp->date_carte.Editura);
	}
}

//Procedura principala
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Paint_Wnd();
void Create_InsertMenu(HWND hwnd);
//Biblioteca
LRESULT CALLBACK WndProc_ShowLibrary(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void RegisterLibrary();
void Paint_Library();
void Paint_Books();
void Books_Buttons(HWND hwnd);
void Library_Buttons(WPARAM wParam, HWND hwnd);
//BookMenu
LRESULT CALLBACK WndProc_BookMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void RegisterBookMenu();

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
		Create_InsertMenu(hwnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_MAIN_INSERT_BUTTON:
			if (nr_carti == 24)
			{
				MessageBoxW(hwnd, L"Ati depasit numarul maxim de 24 de carti!", L"ERROR", MB_ICONERROR);
			}
			else
			{
				//aux_buff e declarat global
				SendMessage(Titlu_Carte, WM_GETTEXT, sizeof(aux_buff.Titlu) / sizeof(char), reinterpret_cast<LPARAM>(aux_buff.Titlu));
				SendMessage(Autor_Carte, WM_GETTEXT, sizeof(aux_buff.Autor) / sizeof(char), reinterpret_cast<LPARAM>(aux_buff.Autor));
				SendMessage(Gen_Carte, WM_GETTEXT, sizeof(aux_buff.Gen_Carte) / sizeof(char), reinterpret_cast<LPARAM>(aux_buff.Gen_Carte));
				SendMessage(Editura_Carte, WM_GETTEXT, sizeof(aux_buff.Editura) / sizeof(char), reinterpret_cast<LPARAM>(aux_buff.Editura));
				if (strlen(aux_buff.Titlu) == 0 || strlen(aux_buff.Autor) == 0 || strlen(aux_buff.Gen_Carte) == 0 || strlen(aux_buff.Editura) == 0 || aux_buff.Autor[0] == ' ' || aux_buff.Titlu[0] == ' ' || aux_buff.Gen_Carte[0] == ' ' || aux_buff.Editura[0] == ' ')
					MessageBoxW(hwnd, L"Introduceti date valide!", L"ERROR", MB_ICONERROR);
				else
				{
					//Golim Spatiile dupa inserare
					DestroyWindow(Titlu_Carte);
					Titlu_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 99, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
					DestroyWindow(Autor_Carte);
					Autor_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 130, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
					DestroyWindow(Gen_Carte);
					Gen_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 160, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
					DestroyWindow(Editura_Carte);
					Editura_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 190, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

					if (cic1 == false)
					{
						root1 = new ThreadedBST();
						root1->left = root1->right = root1;
						strcpy(root1->date_carte.Titlu, "zzzz");
						root1->is_left = true;
						cic1 = true;
						insert_nodeTitlu(aux_buff.Titlu);
					}
					else
					{
						insert_nodeTitlu(aux_buff.Titlu);
					}
					nr_carti++;
				}
			}
			break;
		case IDC_MAIN_TITLE_SORT:
		{
			if (!check_WndLibrary)
			{
				check_WndLibrary = true;
				RegisterLibrary();
				_libraryhwnd = CreateWindowW(L"LibraryWnd", L"My Library", WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE, 500, 0, 800, 450, hwnd, NULL, _hInstance, NULL);
				ShowWindow(_libraryhwnd, _nCmdShow);
				UpdateWindow(_libraryhwnd);
			}
			else
			{
				check_WndLibrary = false;
				DestroyWindow(_libraryhwnd);
			}

		}
		break;
		case IDC_MAIN_RESET:
			root1 = NULL;
			cic1 = false;
			nr_carti = 0;
			break;
		}
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

	//Imaginea Bitmap
	bmpExercising = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	MemDCExercising = CreateCompatibleDC(hDC);
	SelectObject(MemDCExercising, bmpExercising);
	BitBlt(hDC, 0, 245, 800, 476, MemDCExercising, 0, 0, SRCCOPY);

	DeleteDC(MemDCExercising);
	DeleteObject(bmpExercising);

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
void Create_InsertMenu(HWND hwnd){

	CreateWindowW(L"static", L"Title: ", WS_CHILD | WS_VISIBLE, 10, 100, 50, 25, hwnd, (HMENU)1, NULL, NULL);
	Titlu_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 99, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Author: ", WS_CHILD | WS_VISIBLE, 10, 130, 75, 25, hwnd, (HMENU)1, NULL, NULL);
	Autor_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 130, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Style:", WS_CHILD | WS_VISIBLE, 10, 160, 80, 25, hwnd, (HMENU)1, NULL, NULL);
	Gen_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 160, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Publiser:", WS_CHILD | WS_VISIBLE, 10, 190, 80, 25, hwnd, (HMENU)1, NULL, NULL);
	Editura_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 190, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowEx(NULL, "BUTTON", "Insert the book", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 420, 205, 175, 32, hwnd, (HMENU)IDC_MAIN_INSERT_BUTTON, GetModuleHandle(NULL), NULL);

	//CreateWindowW(L"static", L"Show library sorten by", WS_CHILD | WS_VISIBLE, 10, 190, 80, 25, hwnd, (HMENU)1, NULL, NULL);
	CreateWindowEx(NULL, "BUTTON", "The name of the book", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 25, 300, 225, 32, hwnd, (HMENU)IDC_MAIN_TITLE_SORT, GetModuleHandle(NULL), NULL);
	//CreateWindowEx(NULL, "BUTTON", "The name of the author", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 25, 350, 225, 32, hwnd, (HMENU)IDC_MAIN_AUTHOR_SORT, GetModuleHandle(NULL), NULL);
	CreateWindowEx(NULL, "BUTTON", "Reset Library", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 605, 205, 175, 32, hwnd, (HMENU)IDC_MAIN_RESET, GetModuleHandle(NULL), NULL);

}

LRESULT CALLBACK WndProc_ShowLibrary(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_PAINT:
		Paint_Library();
		Paint_Books();
		break;

	case WM_CREATE:
		Books_Buttons(hwnd);
		break;

	case WM_COMMAND:
		Library_Buttons(wParam, hwnd);
		break;

	case WM_DESTROY:
		check_WndLibrary = false;
		DestroyWindow(hwnd);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}
void RegisterLibrary(){
	WNDCLASSW wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"LibraryWnd";
	wc.hInstance = _hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc_ShowLibrary;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&wc);
}
void Paint_Library(){
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;
	hDC = BeginPaint(_libraryhwnd, &Ps);

	qLineColor = RGB(97, 51, 24);
	hLinePen = CreatePen(PS_SOLID, 15, qLineColor);
	hPenOld = (HPEN)SelectObject(hDC, hLinePen);
	MoveToEx(hDC, 0, 200, NULL);
	LineTo(hDC, 800, 200);
	MoveToEx(hDC, 0, 395, NULL);
	LineTo(hDC, 800, 395);
}
void Paint_Books(){
	//hDC = BeginPaint(_libraryhwnd, &Ps);
	HBRUSH NewBrush;
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;

	qLineColor = RGB(255, 0, 255);
	hLinePen = CreatePen(PS_SOLID, 1, qLineColor);
	hPenOld = (HPEN)SelectObject(hDC, hLinePen);

	for (int i = 1; i <= nr_carti; i++)
	{
		NewBrush = CreateSolidBrush(RGB((43 + 44 * i) % 256, (69 + 33 * i) % 256, (109 + 21 * i) % 256));
		SelectObject(hDC, NewBrush);
		if (i <= 12)
			Rectangle(hDC, 4 + 65 * (i - 1), 50, 60 + 65 * (i - 1), 195);
		else
			Rectangle(hDC, 4 + 65 * (i - 13), 250, 60 + 65 * (i - 13), 390);
	}
}
void Books_Buttons(HWND hwnd){
	for (int i = 1; i <= nr_carti; i++)
	{
		if (i <= 12)
			CreateWindowEx(NULL, "BUTTON", "...", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20 + 65 * (i - 1), 145, 25, 28, hwnd, (HMENU)(4000 + i), GetModuleHandle(NULL), NULL);
		else
			CreateWindowEx(NULL, "BUTTON", "...", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20 + 65 * (i - 13), 340, 25, 28, hwnd, (HMENU)(4000 + i), GetModuleHandle(NULL), NULL);
	}
}
void Library_Buttons(WPARAM wParam, HWND hwnd){
	switch (LOWORD(wParam))
	{

	case 4001:
		inorder_Titlu(1);
		break;
	case 4002:
		inorder_Titlu(2);
		break;
	case 4003:
		inorder_Titlu(3);
		break;
	case 4004:
		inorder_Titlu(4);
		break;
	case 4005:
		inorder_Titlu(5);
		break;
	case 4006:
		inorder_Titlu(6);
		break;
	case 4007:
		inorder_Titlu(7);
		break;
	case 4008:
		inorder_Titlu(8);
		break;
	case 4009:
		inorder_Titlu(9);
		break;
	case 4010:
		inorder_Titlu(10);
		break;
	case 4011:
		inorder_Titlu(11);
		break;
	case 4012:
		inorder_Titlu(12);
		break;
	case 4013:
		inorder_Titlu(13);
		break;
	case 4014:
		inorder_Titlu(14);
		break;
	case 4015:
		inorder_Titlu(15);
		break;
	case 4016:
		inorder_Titlu(16);
		break;
	case 4017:
		inorder_Titlu(17);
		break;
	case 4018:
		inorder_Titlu(18);
		break;
	case 4019:
		inorder_Titlu(19);
		break;
	case 4020:
		inorder_Titlu(20);
		break;
	case 4021:
		inorder_Titlu(21);
		break;
	case 4022:
		inorder_Titlu(22);
		break;
	case 4023:
		inorder_Titlu(23);
		break;
	case 4024:
		inorder_Titlu(24);
		break;
	}

	if (!check_bookmenu)
	{
		check_bookmenu = true;
		RegisterBookMenu();
		_bookmenuwnd = CreateWindowW(L"bookinfo", L"My Book", WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE, 150, 145, 370, 160, hwnd, NULL, _hInstance, NULL);
		ShowWindow(_bookmenuwnd, _nCmdShow);
		UpdateWindow(_bookmenuwnd);
	}
	else
	{
		check_bookmenu = false;
		DestroyWindow(_bookmenuwnd);
	}
}

LRESULT CALLBACK WndProc_BookMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_CREATE:
		CreateWindow("static", colectie1, WS_CHILD | WS_VISIBLE, 20, 20, 350, 140, hwnd, (HMENU)1, NULL, NULL);
		break;

	case WM_DESTROY:
		check_bookmenu = false;
		DestroyWindow(hwnd);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}
void RegisterBookMenu(){
	WNDCLASSW wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"bookinfo";
	wc.hInstance = _hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc_BookMenu;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&wc);
}