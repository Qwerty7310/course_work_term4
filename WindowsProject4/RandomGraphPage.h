//#pragma once

#include <windows.h>
#include <CommCtrl.h> // Для использования элемента управления TabControl
#include <vector> // Для работы с данными для графиков и диаграмм
#include <cmath> // Для математических функций
#include <tchar.h>
#include <string>
#include <cstdlib>

#pragma comment(lib, "Comctl32.lib") // Подключаем Comctl32.lib

// Глобальные переменные
extern HINSTANCE hInst;
extern HWND hRandomGraphPage;
//extern HWND hChildRandomPage;

extern WNDPROC g_pRandomGraphPageProc; // Объявление и инициализация указателя
//extern WNDPROC g_pChildPageProc; // Объявление и инициализация указателя

#define ID_GENERATE_BUTTON 9001

extern HWND generateButtonRand;

extern bool flagDrawRand;
extern int randData[21]; // массив данных для построения

// Прототипы функций
LRESULT CALLBACK RandomGraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии
void DrawRandomGraph(HDC hdc, RECT rectClient);
void getRandomData();
