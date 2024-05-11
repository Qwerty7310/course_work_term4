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
#define ID_TEXTBOX_5_1 5001
#define ID_TEXTBOX_6_1 5011
#define ID_TEXTBOX_7_1 5021
#define ID_TEXTBOX_8_1 5031
#define ID_TEXTBOX_9_1 5041

#define ID_TEXTBOX_5_2 5002
#define ID_TEXTBOX_6_2 5012
#define ID_TEXTBOX_7_2 5022
#define ID_TEXTBOX_8_2 5032
#define ID_TEXTBOX_9_2 5042

#define ID_ADD_BUTTON_2 6001
#define ID_DELETE_BUTTON_2 6002
#define ID_CREATE_BUTTON_2 6003

extern WNDPROC g_pGraphPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pHistogramPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pPieChartPageProc; // Объявление и инициализация указателя

// Глобальные переменные
extern HINSTANCE hInst;
extern HWND hPieChartPage;

extern int numPieTextBox;
extern int pieTextBoxIDs[5][2];
extern int numSectors;
extern HWND pieTextBox[5][2];
extern TCHAR* pieText[5][2];

extern HWND addButtonPie;
extern HWND deleteButtonPie;
extern HWND createButtonPie;

// Прототипы функций
LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии
void DrawPieChart(HDC hdc, RECT rectClient);
void DrawTextOnPieChartPage(HWND hWnd, HDC hdc, RECT rectClient); // текст
int getPieChartData();
bool containsLetters(TCHAR* str);
extern bool flagDrawPie;

extern TCHAR* pieText[5][2];
extern double* pieData; // массив данных, глобальный
extern int colors[5];
