//#pragma once

#include <windows.h>
#include <CommCtrl.h> // Для использования элемента управления TabControl
#include <vector> // Для работы с данными для графиков и диаграмм
#include <cmath> // Для математических функций
#include <tchar.h>
#include <string>
#include <cstdlib>

#pragma comment(lib, "Comctl32.lib") // Подключаем Comctl32.lib

//#define IDC_TABCONTROL 1001 // Примерный номер идентификатора для TabControl
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
extern HWND hHistogramPage;

extern int numHistTextBox;
extern int HistTextBoxIDs[5][2];
extern HWND HistTextBox[5][2];

extern WNDPROC g_pTabCtrlProc; // Объявление и инициализация указателя
extern WNDPROC g_pGraphPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pHistogramPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pPieChartPageProc; // Объявление и инициализация указателя

extern HWND addButtonHist;
extern HWND deleteButtonHist;
extern HWND createButtonHist;

// Прототипы функций
LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии
void DrawHistogram(HDC hdc, RECT rectClient);
void DrawTextOnHistogramPage(HWND hWnd, HDC hdc, RECT rectClient); // текст
int getHistogramData();
bool containsLetters(TCHAR* str);

extern bool flagDrawHist;
extern TCHAR* HistText[5][2];
extern double* histData; // массив данных, глобальный
extern int colors[5];
extern int numColumns;
extern double maxLevel;
