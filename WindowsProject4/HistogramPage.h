//#pragma once

#include <windows.h>
#include <CommCtrl.h> // Для использования элемента управления TabControl
#include <vector> // Для работы с данными для графиков и диаграмм
#include <cmath> // Для математических функций
#include <tchar.h>
#include <string>
#include <cstdlib>

#pragma comment(lib, "Comctl32.lib") // Подключаем Comctl32.lib

#define ID_ADD_BUTTON 4001
#define ID_DELETE_BUTTON 4002
#define ID_CREATE_BUTTON 4003

// Глобальные переменные
extern HINSTANCE hInst;
extern HWND hHistogramPage;
extern HWND hChildHistogramPage;

extern WNDPROC g_pHistogramPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pChildPageProc; // Объявление и инициализация указателя

#define ID_ADD_BUTTON 7001
#define ID_DELETE_BUTTON 7002
#define ID_CREATE_BUTTON 7003
#define ID_UP_BUTTON 7004
#define ID_DOWN_BUTTON 7005

#define MAX_HISTOGRAM_TEXTBOX 20

extern int histChildScroll;
extern int numHistTextBox;
extern int numHistColumns;
extern HWND histTextBox[20][2];
extern TCHAR* histText[20][2];

extern HWND addButtonHist;
extern HWND deleteButtonHist;
extern HWND createButtonHist;
extern HWND upButtonHist;
extern HWND downButtonHist;

extern HWND hChildHistPage;
extern int scrollStep;
extern int curHistChildWidth;

extern bool flagDrawHist;
extern int colors[15];
extern double maxHistLevel;
extern double* histData; // массив данных для построения гистограммы

extern WNDPROC g_pChildHistPageProc; // Объявление и инициализация указателя

// Прототипы функций
LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildHistPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии
void DrawHistogram(HDC hdc, RECT rectClient);
int getHistogramData();
bool containsLetters(TCHAR* str);
void HistChildScrollUp(HWND hWnd);
void HistChildScrollDown(HWND hWnd);
