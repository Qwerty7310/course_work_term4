#pragma once

#include <windows.h>
#include <CommCtrl.h> // Для использования элемента управления TabControl
#include <vector> // Для работы с данными для графиков и диаграмм
#include <cmath> // Для математических функций
#include <tchar.h>

#pragma comment(lib, "Comctl32.lib") // Подключаем Comctl32.lib

#define IDC_TABCONTROL 1001 // Примерный номер идентификатора для TabControl
#define ID_CHECKBOX_1 2001
#define ID_CHECKBOX_2 2002
#define ID_CHECKBOX_3 2003

// Глобальные переменные
HINSTANCE hInst;
HWND hTabControl;
HWND hGraphPage;
HWND hHistogramPage;
HWND hPieChartPage;
HWND hButton;
HWND hCheckBox1;
HWND hCheckBox2;
HWND hCheckBox3;
WNDPROC g_pTabCtrlProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pGraphPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pHistogramPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pPieChartPageProc = nullptr; // Объявление и инициализация указателя

// Прототипы функций
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TabControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии

void DrawGraph(HDC hdc, RECT rectClient, double** x, int n, int numrow);
void DrawTextOnGraphPage(HDC hdc, RECT rectClient); // текст
void DrawHistogram(HWND hWnd);
void DrawPieChart(HWND hWnd);
double** getData(int size);
//int p = 0;


double** x; // массив данных, глобальный

const int NUM = 70; // Примерное количество точек для графика

HDC hdc;
