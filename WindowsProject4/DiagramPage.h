//#pragma once

#include <windows.h>
#include <CommCtrl.h> // Для использования элемента управления TabControl
#include <vector> // Для работы с данными для графиков и диаграмм
#include <cmath> // Для математических функций
#include <tchar.h>
#include <string>
#include <cstdlib>

#pragma comment(lib, "Comctl32.lib") // Подключаем Comctl32.lib

#define ID_TEXTBOX_0_1 3001
#define ID_TEXTBOX_1_1 3011
#define ID_TEXTBOX_2_1 3021
#define ID_TEXTBOX_3_1 3031
#define ID_TEXTBOX_4_1 3041

#define ID_TEXTBOX_0_2 3002
#define ID_TEXTBOX_1_2 3012
#define ID_TEXTBOX_2_2 3022
#define ID_TEXTBOX_3_2 3032
#define ID_TEXTBOX_4_2 3042

#define ID_ADD_BUTTON 4001
#define ID_DELETE_BUTTON 4002
#define ID_CREATE_BUTTON 4003

// Глобальные переменные
extern HINSTANCE hInst;
extern HWND hDiagramPage;
extern HWND hChildDiaPage;
//
//extern int numHistTextBox;
//extern int HistTextBoxIDs[5][2];
//extern HWND HistTextBox[5][2];

//extern WNDPROC g_pTabCtrlProc; // Объявление и инициализация указателя
//extern WNDPROC g_pGraphPageProc; // Объявление и инициализация указателя
//extern WNDPROC g_pHistogramPageProc; // Объявление и инициализация указателя
//extern WNDPROC g_pPieChartPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pDiagramPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pChildPageProc; // Объявление и инициализация указателя

//HWND addButton;
//HWND deleteButton;
//HWND createButton;

//#define ID_TEXTBOX_0_1 5001
//#define ID_TEXTBOX_1_1 5011
//#define ID_TEXTBOX_2_1 5021
//#define ID_TEXTBOX_3_1 5031
//#define ID_TEXTBOX_4_1 5041
//
//#define ID_TEXTBOX_0_2 5002
//#define ID_TEXTBOX_1_2 5012
//#define ID_TEXTBOX_2_2 5022
//#define ID_TEXTBOX_3_2 5032
//#define ID_TEXTBOX_4_2 5042

#define ID_ADD_BUTTON 6001
#define ID_DELETE_BUTTON 6002
#define ID_CREATE_BUTTON 6003
#define ID_UP_BUTTON 6004
#define ID_DOWN_BUTTON 6005

#define MAX_DIAGRAM_TEXTBOX 20

extern int diaChildScroll;
extern int numDiaTextBox;
extern int numDiaColumns;
extern HWND diaTextBox[20][2];
extern TCHAR* diaText[20][2];

extern HWND addButtonDia;
extern HWND deleteButtonDia;
extern HWND createButtonDia;
extern HWND upButtonDia;
extern HWND downButtonDia;

extern HWND hChildDiaPage;
extern int scrollStep;
extern int curDiaChildWidth;

extern bool flagDrawDia;
extern int colors[15];
extern double maxDiaLevel;
extern double* diaData; // массив данных для построения гистограммы


extern WNDPROC g_pChildDiaPageProc; // Объявление и инициализация указателя


// Прототипы функций
LRESULT CALLBACK DiagramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildDiaPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии
void DrawDiagram(HDC hdc, RECT rectClient);
void DrawTextOnDiagramPage(HWND hWnd, HDC hdc, RECT rectClient); // текст
int getDiagramData();
bool containsLetters(TCHAR* str);
void DiaChildScrollUp(HWND hWnd);
void DiaChildScrollDown(HWND hWnd);

//extern bool flagDrawHist;
//extern TCHAR* HistText[5][2];
//extern double* histData; // массив данных, глобальный
//extern int colors[5];
//extern int numColumns;
//extern double maxLevel;
