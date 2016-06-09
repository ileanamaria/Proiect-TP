#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>

#define IDB_BITMAP1					3001
#define IDC_MAIN_INSERT_BUTTON		3002
#define IDC_MAIN_TITLE_SORT			3003
#define IDC_MAIN_AUTHOR_SORT		3004
#define IDC_MAIN_RESET				3005
#define IDC_MAIN_INST				3006

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


int nr_carti = 0;
ThreadedBST *root1, *root2;
bool cic1 = false;
Date_Carte aux_buff;
char colectie1[1000];

HWND Titlu_Carte, Autor_Carte, Gen_Carte, Editura_Carte;

HWND _hwnd;
HINSTANCE _hInstance;
HBITMAP bmpExercising;

MSG  msg;
int _nCmdShow;
HDC hDC, MemDCExercising;
PAINTSTRUCT Ps;
HFONT	    font;
LOGFONT LogFont;

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
		
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void Paint_Wnd(){
	//incepem sa desanam in fereastra principala
	//HDC=handle device context
	hDC = BeginPaint(_hwnd, &Ps);
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;


	//Setam Datele Fontului
	LogFont.lfStrikeOut = 0;
	LogFont.lfUnderline = 1;                        // setare subliniere
	LogFont.lfWeight = FW_BOLD;						//Setam sa fie bold
	LogFont.lfHeight = 42;							//inaltimea textului
	LogFont.lfEscapement = 0;
	LogFont.lfItalic = TRUE;

	//alegem culoarea si fontul cu care vom insera text pe ecran
	qLineColor = RGB(165, 42, 42);
	font = CreateFontIndirect(&LogFont);
	SelectObject(hDC, font);						//Selectam Fontul
	SetTextColor(hDC, qLineColor);					//Setam coloarea
	TextOut(hDC, 0, 0, "My Virtual Library", 18);   //afisam textul in pozitia 0,0, 18 caractere

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

	//desenam cele petru meniuri de inserare
	CreateWindowW(L"static", L"Title: ", WS_CHILD | WS_VISIBLE, 10, 100, 50, 25, hwnd, (HMENU)1, NULL, NULL);
	Titlu_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 99, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Author: ", WS_CHILD | WS_VISIBLE, 10, 130, 75, 25, hwnd, (HMENU)1, NULL, NULL);
	Autor_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 130, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Style:", WS_CHILD | WS_VISIBLE, 10, 160, 80, 25, hwnd, (HMENU)1, NULL, NULL);
	Gen_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 160, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Publiser:", WS_CHILD | WS_VISIBLE, 10, 190, 80, 25, hwnd, (HMENU)1, NULL, NULL);
	Editura_Carte = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 93, 190, 320, 25, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	//desenam cele patru butoane cu identificatorii respectivi pentru a executa comenzi atunci cand sunt apasate
	CreateWindowEx(NULL, "BUTTON", "Insert the book", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 420, 205, 175, 32, hwnd, (HMENU)IDC_MAIN_INSERT_BUTTON, GetModuleHandle(NULL), NULL);
	CreateWindowEx(NULL, "BUTTON", "The name of the book", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 25, 300, 225, 32, hwnd, (HMENU)IDC_MAIN_TITLE_SORT, GetModuleHandle(NULL), NULL);
	CreateWindowEx(NULL, "BUTTON", "Reset Library", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 605, 205, 175, 32, hwnd, (HMENU)IDC_MAIN_RESET, GetModuleHandle(NULL), NULL);
	CreateWindowEx(NULL, "BUTTON", "Instructions", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 605, 10, 175, 32, hwnd, (HMENU)IDC_MAIN_INST, GetModuleHandle(NULL), NULL);

}