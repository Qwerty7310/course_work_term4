//#pragma once

#include <windows.h>
#include <CommCtrl.h> // ��� ������������� �������� ���������� TabControl
#include <vector> // ��� ������ � ������� ��� �������� � ��������
#include <cmath> // ��� �������������� �������
#include <tchar.h>
#include <string>

#pragma comment(lib, "Comctl32.lib") // ���������� Comctl32.lib

#define IDC_TABCONTROL 1001 // ��������� ����� �������������� ��� TabControl
#define ID_CHECKBOX_1 2001
#define ID_CHECKBOX_2 2002
#define ID_CHECKBOX_3 2003

// ���������� ����������
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
extern WNDPROC g_pTabCtrlProc; // ���������� � ������������� ���������
extern WNDPROC g_pGraphPageProc; // ���������� � ������������� ���������
extern WNDPROC g_pHistogramPageProc; // ���������� � ������������� ���������
extern WNDPROC g_pPieChartPageProc; // ���������� � ������������� ���������

// ��������� �������
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK TabControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/// <summary>
/// ��������� ����
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // ��������� ������� ������ �����

void DrawGraph(HDC hdc, RECT rectClient, double** x, int n, int numrow);
void DrawTextOnGraphPage(HDC hdc, RECT rectClient); // �����
void DrawHistogram(HWND hWnd);
void DrawPieChart(HWND hWnd);
double** getData(int size);
//int p = 0;


extern double** x; // ������ ������, ����������

extern const int NUM; // ��������� ���������� ����� ��� �������

//extern HDC hdc;
