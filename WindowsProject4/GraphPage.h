#include <windows.h>
#include <CommCtrl.h> // Для использования элемента управления TabControl
#include <vector> // Для работы с данными для графиков и диаграмм
#include <cmath> // Для математических функций
#include <tchar.h>
#include <string>

#pragma comment(lib, "Comctl32.lib") // Подключаем Comctl32.lib

#define ID_CHECKBOX_1 2001
#define ID_CHECKBOX_2 2002
#define ID_CHECKBOX_3 2003

// объявление переменных
extern HINSTANCE hInst;
extern HWND hGraphPage;
extern LPCWSTR CheckBoxNames[3];
extern int CheckBoxIDs[3];
extern HWND CheckBox[3];
extern WNDPROC g_pGraphPageProc; //указатель на функцию обработки окна
extern double** x; //массив данных для построения графиков
extern const int NUM; // количесто точек для построения графика

// прототипы функций
LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // функция рисования прямой линии
void DrawGraph(HDC hdc, RECT rectClient, double** x, int n, int numrow);
void DrawTextOnGraphPage(HDC hdc, RECT rectClient); // функция рисования текста
double** getGraphData(int size);
