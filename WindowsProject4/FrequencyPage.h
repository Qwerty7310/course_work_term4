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
extern HWND hFrequencyPage;
extern HWND hChildFrequencyPage;

extern WNDPROC g_pFrequencyPageProc; // Объявление и инициализация указателя
extern WNDPROC g_pChildPageProc; // Объявление и инициализация указателя

#define ID_ADD_BUTTON 8001
#define ID_DELETE_BUTTON 8002
#define ID_CREATE_BUTTON 8003
#define ID_UP_BUTTON 8004
#define ID_DOWN_BUTTON 8005

#define MAX_FREQUENCY_TEXTBOX 21

extern int freqChildScroll;
extern int numFreqTextBox;
extern int numFreqColumns;
extern HWND freqTextBox[20][2];
extern TCHAR* freqText[20][2];

extern HWND addButtonFreq;
extern HWND deleteButtonFreq;
extern HWND createButtonFreq;
extern HWND upButtonFreq;
extern HWND downButtonFreq;

extern HWND hChildFreqPage;
extern int scrollStep;
extern int curFreqChildWidth;

extern bool flagDrawFreq;
extern int colors[15];
extern double maxFreqLevel;
extern double* freqData; // массив данных для построения гистограммы
extern double maxPos;
extern double minNig;

extern WNDPROC g_pChildFreqPageProc; // Объявление и инициализация указателя

// Прототипы функций
LRESULT CALLBACK FrequencyPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildFreqPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии
void DrawFrequency(HDC hdc, RECT rectClient);
int getFrequencyData();
bool containsLetters(TCHAR* str);
void FreqChildScrollUp(HWND hWnd);
void FreqChildScrollDown(HWND hWnd);
