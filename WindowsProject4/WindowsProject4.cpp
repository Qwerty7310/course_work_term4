//#pragma once

#include "GraphPage.h"
#include "HistogramPage.h"
#include "PieChartPage.h"

// Глобальные переменные
HINSTANCE hInst;

HWND hTabControl;
HWND hGraphPage;
HWND hHistogramPage;
HWND hPieChartPage;

WNDPROC g_pTabCtrlProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pGraphPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pHistogramPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pPieChartPageProc = nullptr; // Объявление и инициализация указателя

//Для построения графиков
LPCWSTR CheckBoxNames[3] = { L"y = cos(x)", L"y = πx / 10 - 1", L"y = sin(x)" };
int CheckBoxIDs[3] = { 2001, 2002, 2003 };
HWND CheckBox[3];
const int NUM = 70; // Примерное количество точек для графика
double** x; // массив данных для построения графиков

//Для построения гистограммы
int numHistTextBox = 3;
int HistTextBoxIDs[5][2] = {
    {3001, 3002},
    {3011, 3012},
    {3021, 3022},
    {3031, 3032},
    {3041, 3042}
};
int numColumns = 3;
HWND HistTextBox[5][2];
TCHAR* HistText[5][2];

HWND addButtonHist;
HWND deleteButtonHist;
HWND createButtonHist;

bool flagDrawHist = false;
double maxLevel;
double* histData; // массив данных для построения гистограммы

//Для построения круговой диаграммы
int numPieTextBox = 3;
int pieTextBoxIDs[5][2] = {
    {3001, 3002},
    {3011, 3012},
    {3021, 3022},
    {3031, 3032},
    {3041, 3042}
};
int numSectors = 3;
HWND pieTextBox[5][2];
TCHAR* pieText[5][2];

HWND addButtonPie;
HWND deleteButtonPie;
HWND createButtonPie;

bool flagDrawPie = false;
double* pieData; // массив данных для построения гистограммы


// Прототипы функций
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK TabControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); //отрезок прямой
bool containsLetters(TCHAR* str); //проверка на буквы

//массив цветов для диаграмм
int colors[5] = {   RGB(204, 0, 0),
                    RGB(255, 255, 0),
                    RGB(51, 102, 0),
                    RGB(0, 0, 255),
                    RGB(153, 0, 153) };


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

    x = getGraphData(NUM); // задание исходных данных

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
        return FALSE;

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
        // Создаем элемент управления TabControl
        hTabControl = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            10, 30, 760, 540, hWnd, NULL, hInst, NULL);

        // Создаем дочерние окна для каждой вкладки
        hGraphPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE,
            0, 20, 760, 540, hWnd, nullptr, hInst, nullptr);

        if (hGraphPage)
            g_pGraphPageProc = (WNDPROC)SetWindowLongPtr(hGraphPage, GWLP_WNDPROC, (LONG_PTR)GraphPageProc); // Заменяем процедуру окна

        hHistogramPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE,
            0, 20, 760, 540, hWnd, nullptr, hInst, nullptr);

        if (hHistogramPage)
            g_pHistogramPageProc = (WNDPROC)SetWindowLongPtr(hHistogramPage, GWLP_WNDPROC, (LONG_PTR)HistogramPageProc); // Заменяем процедуру окна


        hPieChartPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE,
            0, 20, 760, 540, hWnd, nullptr, hInst, nullptr);

        if (hPieChartPage)
            g_pPieChartPageProc = (WNDPROC)SetWindowLongPtr(hPieChartPage, GWLP_WNDPROC, (LONG_PTR)PieChartPageProc); // Заменяем процедуру окна


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

        ShowWindow(hGraphPage, SW_SHOW);
        ShowWindow(hHistogramPage, SW_HIDE);
        ShowWindow(hPieChartPage, SW_HIDE);

        break;
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.

        //Создание надписи
        HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Arial");

        // серый цвет
        SetTextColor(hdc, RGB(0, 0, 0));
        SelectObject(hdc, hFont); // выбираем шрифт
        SetBkMode(hdc, TRANSPARENT); // фон прозрачный

        RECT rectText = { ps.rcPaint.left, 5, ps.rcPaint.right, ps.rcPaint.bottom };
        DrawText(hdc, L"2D ГРАФИКИ", -1, &rectText, DT_CENTER);
        break;
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
        //Устанавливаем минимальные и максимальные размеры
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 700;
        lpMMI->ptMinTrackSize.y = 370;
        break;
    }
    case WM_DESTROY:
        // Отменяем присвоение обработчика сообщений TabControl
        delete[] histData;
        SetWindowLongPtr(hTabControl, GWLP_WNDPROC, (LONG_PTR)g_pTabCtrlProc);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void line(HDC hdc, int Xs, int Ys, int Xf, int Yf) // рисование отрезка прямой линии
{
    MoveToEx(hdc, Xs, Ys, 0); // перемещение из точки (Xs, Ys)
    LineTo(hdc, Xf, Yf); // в точку (Xt, Yf)
}

bool containsLetters(TCHAR* str) {
    while (*str) {
        //if (_istalpha(*str) || *str == ',') {
        if ((*str < '0' || *str > '9') && (*str != '.'))
            return true; // Если найдена буква, возвращаем true
        str++; // Переходим к следующему символу
    }
    return false; // Если не найдено ни одной буквы, возвращаем false
}
