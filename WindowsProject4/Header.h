#pragma once

#include <windows.h>
#include <CommCtrl.h> // ��� ������������� �������� ���������� TabControl
#include <vector> // ��� ������ � ������� ��� �������� � ��������
#include <cmath> // ��� �������������� �������
#include <tchar.h>

#pragma comment(lib, "Comctl32.lib") // ���������� Comctl32.lib

#define IDC_TABCONTROL 1001 // ��������� ����� �������������� ��� TabControl
#define ID_CHECKBOX_1 2001
#define ID_CHECKBOX_2 2002
#define ID_CHECKBOX_3 2003

// ���������� ����������
HINSTANCE hInst;
HWND hTabControl;
HWND hGraphPage;
HWND hHistogramPage;
HWND hPieChartPage;
HWND hButton;
HWND hCheckBox1;
HWND hCheckBox2;
HWND hCheckBox3;
WNDPROC g_pTabCtrlProc = nullptr; // ���������� � ������������� ���������
WNDPROC g_pGraphPageProc = nullptr; // ���������� � ������������� ���������
WNDPROC g_pHistogramPageProc = nullptr; // ���������� � ������������� ���������
WNDPROC g_pPieChartPageProc = nullptr; // ���������� � ������������� ���������

// ��������� �������
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TabControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // ��������� ������� ������ �����

void DrawGraph(HDC hdc, RECT rectClient, double** x, int n, int numrow);
void DrawTextOnGraphPage(HDC hdc, RECT rectClient); // �����
void DrawHistogram(HWND hWnd);
void DrawPieChart(HWND hWnd);
double** getData(int size);
//int p = 0;


double** x; // ������ ������, ����������

const int NUM = 70; // ��������� ���������� ����� ��� �������

HDC hdc;
