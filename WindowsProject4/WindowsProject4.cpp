//#pragma once

#include "GraphPage.h"

//#include <windows.h>
//#include <CommCtrl.h> // Для использования элемента управления TabControl
//#include <vector> // Для работы с данными для графиков и диаграмм
//#include <cmath> // Для математических функций
//#include <tchar.h>

//#pragma comment(lib, "Comctl32.lib") // Подключаем Comctl32.lib

//#define IDC_TABCONTROL 1001 // Примерный номер идентификатора для TabControl
//#define ID_CHECKBOX_1 2001
//#define ID_CHECKBOX_2 2002
//#define ID_CHECKBOX_3 2003

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

LPCWSTR CheckBoxNames[3] = { L"y = sin(x)", L"y = πx / 10 - 1", L"y = cos(x)" };
int CheckBoxIDs[3] = { 2001, 2002, 2003 };
HWND CheckBox[3];

WNDPROC g_pTabCtrlProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pGraphPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pHistogramPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pPieChartPageProc = nullptr; // Объявление и инициализация указателя

// Прототипы функций
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TabControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); // рисование отрезка прямой линии

void DrawGraph(HDC hdc, RECT rectClient, double** x, int n, int numrow);
void DrawTextOnGraphPage(HDC hdc, RECT rectClient); // текст
void DrawHistogram(HWND hWnd);
void DrawPieChart(HWND hWnd);
double** getData(int size);
void line(HDC hdc, int Xs, int Ys, int Xf, int Yf);
//int p = 0;


double** x; // массив данных, глобальный

const int NUM = 70; // Примерное количество точек для графика

//HDC hdc;

// Функция WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Регистрация класса окна
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //wcex.hbrBackground = CreateSolidBrush(0x00FFFFFF);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"MainWindow";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    x = getData(NUM); // задание исходны данных

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Создание окна
    if (!InitInstance(hInstance, nCmdShow))
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Функция инициализации экземпляра
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindow(L"MainWindow", L"2D графики", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1000, 600, NULL, NULL, hInstance, NULL);// 0, 800, 600,

    if (!hWnd)
    {
        return FALSE;
    }
    // Показать окно и обновить его
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

// Функция обработки сообщений окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_NOTIFY:
    {
        switch (((LPNMHDR)lParam)->code)
        {
        case TCN_SELCHANGING:
        {
            // Return FALSE to allow the selection to change.
            return FALSE;
        }

        case TCN_SELCHANGE:
        {
            OutputDebugString(L"It is OK! \n");
            int selectedIndex = TabCtrl_GetCurSel(hTabControl);
            switch (selectedIndex)
            {
            case 0:
                ShowWindow(hGraphPage, SW_SHOW);
                ShowWindow(hHistogramPage, SW_HIDE);
                ShowWindow(hPieChartPage, SW_HIDE);
                break;
            case 1:
                ShowWindow(hGraphPage, SW_HIDE);
                ShowWindow(hHistogramPage, SW_SHOW);
                ShowWindow(hPieChartPage, SW_HIDE);
                break;
            case 2:
                ShowWindow(hGraphPage, SW_HIDE);
                ShowWindow(hHistogramPage, SW_HIDE);
                ShowWindow(hPieChartPage, SW_SHOW);
                break;
            }

            break;
        }
        }
        return TRUE;
    }
    case WM_CREATE:
    {
        //OutputDebugString(L"Is called111111\n");
        // Создаем элемент управления TabControl
        hTabControl = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            10, 30, 760, 540, hWnd, (HMENU)IDC_TABCONTROL, hInst, NULL);

        // Создаем дочерние окна для каждой вкладки и связываем их с элементом управления TabControl
        //hGraphPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        //    0, 20, 760, 520, hWnd, (HMENU)hTabControl, hInst, nullptr);
        hGraphPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE,
            0, 20, 760, 540, hWnd, nullptr, hInst, nullptr);

        if (hGraphPage)
        {
            //OutputDebugString(L"GraphPageProc\n");
            g_pGraphPageProc = (WNDPROC)SetWindowLongPtr(hGraphPage, GWLP_WNDPROC, (LONG_PTR)GraphPageProc); // Заменяем процедуру окна
        }

        hHistogramPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE,
            0, 20, 760, 540, hWnd, nullptr, hInst, nullptr);

        if (hHistogramPage)
        {
            //OutputDebugString(L"GraphPageProc\n");
            g_pHistogramPageProc = (WNDPROC)SetWindowLongPtr(hHistogramPage, GWLP_WNDPROC, (LONG_PTR)HistogramPageProc); // Заменяем процедуру окна
        }

        hPieChartPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE,
            0, 20, 760, 540, hWnd, nullptr, hInst, nullptr);

        if (hPieChartPage)
        {
            //OutputDebugString(L"GraphPageProc\n");
            g_pPieChartPageProc = (WNDPROC)SetWindowLongPtr(hPieChartPage, GWLP_WNDPROC, (LONG_PTR)PieChartPageProc); // Заменяем процедуру окна
        }

        // Добавляем вкладки на TabControl и связываем их с соответствующими окнами
        TCITEM tie;
        tie.mask = TCIF_TEXT | TCIF_PARAM;
        tie.pszText = const_cast<LPTSTR>(L"График");
        tie.lParam = (LPARAM)hGraphPage;
        TabCtrl_InsertItem(hTabControl, 0, &tie);

        
        tie.pszText = const_cast<LPTSTR>(L"Гистограмма");
        tie.lParam = (LPARAM)hHistogramPage;
        TabCtrl_InsertItem(hTabControl, 1, &tie);

        tie.pszText = const_cast<LPTSTR>(L"Круговая диаграмма");
        tie.lParam = (LPARAM)hPieChartPage;
        TabCtrl_InsertItem(hTabControl, 2, &tie);

        // Установка нового обработчика сообщений для элемента управления TabControl
        //g_pTabCtrlProc = (WNDPROC)SetWindowLongPtr(hTabControl, GWLP_WNDPROC, (LONG_PTR)TabControlProc);

        ShowWindow(hGraphPage, SW_SHOW);
        ShowWindow(hHistogramPage, SW_HIDE);
        ShowWindow(hPieChartPage, SW_HIDE);

        // Рисуем прямоугольник внутри окна GraphPage
        //OutputDebugString(L"Is called 22222222222\n");
       

        break;
    }
    break;
    case WM_PAINT:
    {
        /*
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
        r.left = 100; //левый верхний угол
        r.top = 100;
        r.right = 200; //правый нижний
        r.right = 300;

        //Заполняем прямоугольник
        FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(RGB(70, 200, 100)));
        EndPaint(hWnd, &ps);*/

    }
    break;
    case WM_SIZE:
    {
        // Изменяем размеры элемента управления TabControl при изменении размеров окна
        RECT rcClient;
        GetClientRect(hWnd, &rcClient);
        SetWindowPos(hTabControl, NULL, 10, 30, rcClient.right - 20, rcClient.bottom - 40, SWP_NOZORDER);
        SetWindowPos(hGraphPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
        SetWindowPos(hHistogramPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
        SetWindowPos(hPieChartPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
        break;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 300;
        lpMMI->ptMinTrackSize.y = 300;
        break;
    }
    case WM_DESTROY:
        // Отменяем присвоение обработчика сообщений TabControl
        SetWindowLongPtr(hTabControl, GWLP_WNDPROC, (LONG_PTR)g_pTabCtrlProc);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Обработчик события смены вкладок
LRESULT CALLBACK TabControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        /*
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
        r.left = 50; //левый верхний угол
        r.top = 100;
        r.right = 200; //правый нижний
        r.right = 300;

        //Заполняем прямоугольник
        FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(RGB(255, 0, 0)));*/
        break;
    }
    }

    // Вызываем старую функцию обработки сообщений
    if (g_pTabCtrlProc)
        return CallWindowProc(g_pTabCtrlProc, hWnd, message, wParam, lParam);
    else
        return DefWindowProc(hWnd, message, wParam, lParam);
}

void line(HDC hdc, int Xs, int Ys, int Xf, int Yf) // рисование отрезка прямой линии
{
    MoveToEx(hdc, Xs, Ys, 0); // перемещение из точки (Xs, Ys)
    LineTo(hdc, Xf, Yf); // в точку (Xt, Yf)
}



// Функция рисования гистограммы на вкладке "Histogram"
void DrawHistogram(HWND hWnd)
{
    // Здесь должен быть код для рисования гистограммы
    HDC hdc = GetDC(hWnd);
    TextOut(hdc, 10, 10, L"Histogram will be drawn here", 27);
    ReleaseDC(hWnd, hdc);
}

// Функция рисования круговой диаграммы на вкладке "Pie Chart"
void DrawPieChart(HWND hWnd)
{
    // Здесь должен быть код для рисования круговой диаграммы
    HDC hdc = GetDC(hWnd);
    TextOut(hdc, 10, 10, L"Pie Chart will be drawn here", 26);
    ReleaseDC(hWnd, hdc);
}

double** getData(int n)
{
    double** f;
    f = new double* [4];
    f[0] = new double[n];
    f[1] = new double[n];
    f[2] = new double[n];
    f[3] = new double[n];
    for (int i = 0; i < n; i++)
    {
        // 3 графика (4-1)
        double x = (double)i * 0.099;
        //double x = (double)i;
        f[0][i] = x;
        f[3][i] = sin(x);
        f[1][i] = cos(x); //!!!!!
        //f[2][i] = 0.31459265358979 * x - 1.0; //!!!!!!!!!
        f[2][i] = 3.14159265358979 * x / 10 - 1.0; //!!!!!!!!!
    }
    return f;
}

/****************************************************************************************
 *                                Вкладка HistogramPage                                 *
 ****************************************************************************************/

    //Процедура окна
LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //OutputDebugString(L"HistogramPageProc 1\n");
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
        r.left = 50; //левый верхний угол
        r.top = 50;
        r.right = 100; //правый нижний
        r.bottom = 300;

        //Заполняем прямоугольник
        FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(RGB(255, 100, 255)));
        EndPaint(hWnd, &ps);
        break;
    }
    }

    if (g_pHistogramPageProc)
        return CallWindowProc(g_pHistogramPageProc, hWnd, message, wParam, lParam);
    else
        return DefWindowProc(hWnd, message, wParam, lParam);
    //return 0;
}


/****************************************************************************************
 *                                 Вкладка PieChartPage                                 *
 ****************************************************************************************/

 //Процедура окна
LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //OutputDebugString(L"PieChartPageProc 1\n");
    switch (message)
    {
    case WM_PAINT:
    {
        //OutputDebugString(L"PieChartPageProc 2 !!!!!!!!!!\n");
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
        r.left = 0; //левый верхний угол
        r.top = 0;
        r.right = 300; //правый нижний
        r.bottom = 400;

        //Заполняем прямоугольник
        FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(RGB(0, 100, 50)));
        EndPaint(hWnd, &ps);
        break;
    }
    //default:
    //    return DefWindowProc(hWnd, message, wParam, lParam);
    }

    if (g_pPieChartPageProc)
        return CallWindowProc(g_pPieChartPageProc, hWnd, message, wParam, lParam);
    else
        return DefWindowProc(hWnd, message, wParam, lParam);
}