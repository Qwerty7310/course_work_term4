#pragma once

#include "Header.h"

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
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
        r.left = 100; //левый верхний угол
        r.top = 100;
        r.right = 200; //правый нижний
        r.right = 300;

        //Заполняем прямоугольник
        FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(RGB(70, 200, 100)));
        EndPaint(hWnd, &ps);

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
    }
    break;
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

/*double** getData(int size)
{
    double** data = new double* [size];
    for (int i = 0; i < size; ++i)
    {
        data[i] = new double[2]; // Предположим, что у нас два значения x и y для каждой точки
        data[i][0] = i; // Просто заглушка для x, например, от 0 до size-1
        data[i][1] = rand() % 100; // Просто случайные значения для y
    }
    return data;
}*/

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
        f[0][i] = x;
        f[3][i] = sin(x);
        f[1][i] = cos(x); //!!!!!
        f[2][i] = 0.31459265358979 * x - 1.0; //!!!!!!!!!
    }
    return f;
}

/****************************************************************************************
 *                                  Вкладка GraphPage                                   *
 ****************************************************************************************/

/*----------------------------------- Процедура окна -----------------------------------*/

LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //OutputDebugString(L"GraphPageProc 1\n");
    //PAINTSTRUCT ps;
    UINT state;
    PAINTSTRUCT ps;

    RECT rectClient;
    switch (message)
    {
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        SetTextColor(hdc, 0x00FF0000); // синий цвет букв
        DrawGraph(hdc, ps.rcPaint, x, NUM, 3); // построение графика
        DrawTextOnGraphPage(hdc, ps.rcPaint); // текст
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND:
    {
        // Получаем ID чекбокса, который был нажат
        int wmId = LOWORD(wParam);
        // Получаем дескриптор (HWND) чекбокса
        HWND hwndCtl = (HWND)lParam;

        // Обрабатываем нажатие на чекбокс в зависимости от его ID
        switch (wmId)
        {
        case ID_CHECKBOX_1:
            // Обработка нажатия на чекбокс 1
            OutputDebugString(L"ID_CHECKBOX_1!\n");
            // Получаем текущий статус чекбокса
            state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);

            // Проверяем статус чекбокса
            switch (state)
            {
            case BST_CHECKED: // Чекбокс нажат
                // Сбрасываем состояние чекбокса
                SendMessage(hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
                break;
            case BST_UNCHECKED: // Чекбокс не нажат
                // Устанавливаем состояние чекбокса в "нажатое"
                SendMessage(hCheckBox1, BM_SETCHECK, BST_CHECKED, 0);
                break;
            default:
                // Неопределенный статус
                break;
            }
            break;
        case ID_CHECKBOX_2:
            // Обработка нажатия на чекбокс 2
            OutputDebugString(L"ID_CHECKBOX_2!\n");
            // Получаем текущий статус чекбокса
            state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);

            // Проверяем статус чекбокса
            switch (state)
            {
            case BST_CHECKED: // Чекбокс нажат
                // Сбрасываем состояние чекбокса
                SendMessage(hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
                break;
            case BST_UNCHECKED: // Чекбокс не нажат
                // Устанавливаем состояние чекбокса в "нажатое"
                SendMessage(hCheckBox2, BM_SETCHECK, BST_CHECKED, 0);
                break;
            default:
                // Неопределенный статус
                break;
            }
            break;
        case ID_CHECKBOX_3:
            // Обработка нажатия на чекбокс 3
            OutputDebugString(L"ID_CHECKBOX_3!\n");
            // Получаем текущий статус чекбокса
            state = SendMessage(hCheckBox3, BM_GETCHECK, 0, 0);

            // Проверяем статус чекбокса
            switch (state)
            {
            case BST_CHECKED: // Чекбокс нажат
                // Сбрасываем состояние чекбокса
                SendMessage(hCheckBox3, BM_SETCHECK, BST_UNCHECKED, 0);
                break;
            case BST_UNCHECKED: // Чекбокс не нажат
                // Устанавливаем состояние чекбокса в "нажатое"
                SendMessage(hCheckBox3, BM_SETCHECK, BST_CHECKED, 0);
                break;
            default:
                // Неопределенный статус
                break;
            }
            break;
            // Добавьте другие случаи по мере необходимости
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    }
    if (g_pGraphPageProc)
        return CallWindowProc(g_pGraphPageProc, hWnd, message, wParam, lParam);
    else
        return DefWindowProc(hWnd, message, wParam, lParam);
}


/*------------------------------ Функция рисования графика ------------------------------*/

void DrawGraph(HDC hdc, RECT rectClient, double** x, // массив данных
    int n, // количество точек
    int numrow = 1) // количество рядов данных (по умолчанию 1)
{
    SetBkMode(hdc, TRANSPARENT); // фон прозрачный
    double OffsetY, OffsetX; double MAX_X, MAX_Y; double ScaleX, ScaleY;
    double min, max; int height, width;
    int X, Y; // координаты в окне (в рх)
    HPEN hpen;
    height = rectClient.bottom - rectClient.top;
    width = rectClient.right - rectClient.left - 200;
    // Область допустимых значений Х
    min = x[0][0];
    max = x[0][0];
    for (int i = 0; i < n; i++)
    {
        if (x[0][i] < min) min = x[0][i];
        if (x[0][i] > max) max = x[0][i];
    }

    double temp = max - min;
    MAX_X = max - min; // Разброс по оси Х
    OffsetX = min * width / MAX_X; // смещение X
    ScaleX = (double)width / MAX_X; // масштабный коэффициент Х
    // Область допустимых значений Y
    max = 1.5; /////////!!!!!!!!!!!
    min = -1.5; ///

    MAX_Y = max - min; // Разброс по оси Y
    OffsetY = max * height / (MAX_Y); // смещение Y
    ScaleY = (double)height / MAX_Y; // масштабный коэффициент Y
    // Отрисовка осей координат
    hpen = CreatePen(PS_SOLID, 3, 0); // чёрное перо толщиной в 3 пикселя
    SelectObject(hdc, hpen);
    line(hdc, 10, OffsetY, width - 10, OffsetY); // рисование горизонтальной оси line(hdc, width - 10, OffsetY, width - 15, OffsetY - 5); // верхнее горизонтальное оперение line(hdc, width - 10, OffsetY, width - 15, OffsetY + 5); // нижнее горизонтальное оперение
    for (int i = 0; i < 9; i++) line(hdc, i * 0.118 * width, OffsetY, i * 0.118 * width, OffsetY - 3);
    // отметки на горизонтальной оси
    line(hdc, OffsetX + 10, 0, OffsetX + 10, height - 5); // рисование вертикальной оси line(hdc, OffsetX + 10, 0, OffsetX + 15, 10); // рисование правого вертикального оперения line(hdc, OffsetX + 10, 0, OffsetX + 5, 10); // рисование левого вертикального оперения
    for (int i = -2; i < 3; i++) line(hdc, OffsetX + 10, OffsetY + height / 6 * i, OffsetX + 15, OffsetY + height / 6 * i);
    // отметки на вертикальной оси
    DeleteObject(hpen); // удаление черного пера

                // Отрисовка графика функции
    int color = 0xFF0000; // синее перо для первого ряда данных
    TextOut(hdc, 21, 7, _T("Y = sin (X)"), 13); // Вывод текста y=sin(x) 8 символов
    TextOut(hdc, 27, height / 6 - 12, _T("1,0"), 3);
    TextOut(hdc, 27, height / 3 - 12, _T("0,5"), 3);
    TextOut(hdc, 27, 2 * height / 3 - 12, _T("-0,5"), 4);
    TextOut(hdc, 27, 5 * height / 6 - 12, _T("-1,0"), 4);
    TextOut(hdc, 15, 5 + 0.5 * height, _T("0,0"), 3);
    TextOut(hdc, 0.92 * width * 0.125, 5 + 0.5 * height, _T("0,78"), 4);
    TextOut(hdc, 0.92 * width * 0.25, 5 + 0.5 * height, _T("1,57"), 4);
    TextOut(hdc, 0.92 * width * 0.375, 5 + 0.5 * height, _T("2,35"), 4);
    TextOut(hdc, 0.92 * width * 0.5, 5 + 0.5 * height, _T("3,14"), 4);
    TextOut(hdc, 0.92 * width * 0.625, 5 + 0.5 * height, _T("3,92"), 4);
    TextOut(hdc, 0.92 * width * 0.75, 5 + 0.5 * height, _T("4,71"), 4);
    TextOut(hdc, 0.92 * width * 0.875, 5 + 0.5 * height, _T("5,49"), 4);
    TextOut(hdc, 0.92 * width, 5 + 0.5 * height, _T("6,28"), 4);
    TextOut(hdc, 0.98 * width, 5 + 0.5 * height, _T("x"), 1);
    for (int j = 1; j <= numrow; j++)
    {
        while (1)
        {
            hpen = CreatePen(PS_SOLID, 3, color); // формирование пера толщиной 3 пикселя
            SelectObject(hdc, hpen);
            X = (int)(OffsetX + x[0][0] * ScaleX); // начальная точка графика
            Y = (int)(OffsetY - x[j][0] * ScaleY);
            MoveToEx(hdc, X + 10, Y, 0); // перемещение в начальную точку
            for (int i = 0; i < (n - 3); i++)
            {
                X = OffsetX + 12 + x[0][i] * ScaleX;
                Y = OffsetY - x[j][i] * ScaleY;
                LineTo(hdc, X, Y);
            }
            color = color >> 8; // изменение цвета пера для следующего ряда
            DeleteObject(hpen); // удаление текущего пера
            break;
        }
    }
}

void DrawTextOnGraphPage(HDC hdc, RECT rectClient) {
    UINT state;

    //Создание надписи
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");

    // серый цвет
    SetTextColor(hdc, RGB(100, 100, 100));
    SelectObject(hdc, hFont); // выбираем шрифт
    SetBkMode(hdc, TRANSPARENT); // фон прозрачный

    // Рисуем текст с прозрачным фоном
    //TextOut(hdc, rectClient.right - 200, 20, L"Выберите графики,\nкоторые вы хотите увидеть:", wcslen(L"Выберите графики,\nкоторые вы хотите увидеть:"));

    RECT rectText = { rectClient.right - 200, 20, rectClient.right, rectClient.bottom };
    DrawText(hdc, L"Выберите графики,\nкоторые вы хотите увидеть:", -1, &rectText, DT_WORDBREAK);

    state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    DestroyWindow(hCheckBox1); //Удаление CheckBox, чтобы заново нарисовать в другом месте
    hCheckBox1 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_1", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
        rectClient.right - 190, 90, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_1, NULL, NULL);
    if (state == BST_CHECKED) SendMessage(hCheckBox1, BM_SETCHECK, BST_CHECKED, 0);

    state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
    DestroyWindow(hCheckBox2); //Удаление CheckBox, чтобы заново нарисовать в другом месте
    hCheckBox2 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_2", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
        rectClient.right - 190, 120, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_2, NULL, NULL);
    if (state == BST_CHECKED) SendMessage(hCheckBox2, BM_SETCHECK, BST_CHECKED, 0);

    state = SendMessage(hCheckBox3, BM_GETCHECK, 0, 0);
    DestroyWindow(hCheckBox3); //Удаление CheckBox, чтобы заново нарисовать в другом месте
    hCheckBox3 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_3", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
        rectClient.right - 190, 150, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_3, NULL, NULL);
    if (state == BST_CHECKED) SendMessage(hCheckBox3, BM_SETCHECK, BST_CHECKED, 0);

    // Освобождение шрифта
    DeleteObject(hFont);

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
        hdc = BeginPaint(hWnd, &ps);
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
        hdc = BeginPaint(hWnd, &ps);
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