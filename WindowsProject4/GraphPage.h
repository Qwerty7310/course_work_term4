//#pragma once

#include <windows.h>
#include <CommCtrl.h> // Для использования элемента управления TabControl
#include <vector> // Для работы с данными для графиков и диаграмм
#include <cmath> // Для математических функций
#include <tchar.h>
#include <string>

#pragma comment(lib, "Comctl32.lib") // Подключаем Comctl32.lib

#define IDC_TABCONTROL 1001 // Примерный номер идентификатора для TabControl
#define ID_CHECKBOX_1 2001
#define ID_CHECKBOX_2 2002
#define ID_CHECKBOX_3 2003

// Глобальные переменные
extern HINSTANCE hInst;
extern HWND hTabControl;
extern HWND hGraphPage;
extern HWND hHistogramPage;
extern HWND hPieChartPage;
extern HWND hButton;
extern HWND hCheckBox1;
extern HWND hCheckBox2;
extern HWND hCheckBox3;
extern LPCWSTR CheckBoxNames[3];
extern int CheckBoxIDs[3];
extern HWND CheckBox[3];
extern WNDPROC g_pTabCtrlProc; // Объявление и инициализация указателя
extern WNDPROC g_pGraphPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pHistogramPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pPieChartPageProc; // Объявление и инициализация указателя

// Прототипы функций
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK TabControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Процедура окна
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии

void DrawGraph(HDC hdc, RECT rectClient, double** x, int n, int numrow);
void DrawTextOnGraphPage(HDC hdc, RECT rectClient); // текст
void DrawHistogram(HWND hWnd);
void DrawPieChart(HWND hWnd);
double** getData(int size);
//int p = 0;


extern double** x; // массив данных, глобальный

extern const int NUM; // Примерное количество точек для графика

//extern HDC hdc;
