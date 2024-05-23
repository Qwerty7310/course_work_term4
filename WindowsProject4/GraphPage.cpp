#include "GraphPage.h"

LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT state;
    RECT rectClient;
    GetClientRect(hGraphPage, &rectClient);

    switch (message)
    {
    case WM_CREATE:
    {
        OutputDebugString(L"GraphPage_WM_CREATE\n");

        // Получите дескриптор контекста устройства для рисования
        HDC hdc = GetDC(hWnd);

        // Создание шрифта
        HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Arial");

        // Выбор цвета текста
        SetTextColor(hdc, RGB(100, 100, 100));

        // Выбор шрифта
        SelectObject(hdc, hFont);

        // Установка режима фона
        SetBkMode(hdc, TRANSPARENT);

        // Создание текста
        RECT rectText = { 0, 0, 0, 0 }; // Размеры и позиция будут уточнены позже в WM_SIZE
        HWND hStaticText = CreateWindowEx(0, L"STATIC", L"Выберите графики,\nкоторые вы хотите\nпостроить:",
            WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);

        SendMessage(hStaticText, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Сохранение дескриптора текста в пользовательских данных окна
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)hStaticText);

        // Создайте чекбоксы
        for (int i = 0; i < 3; ++i) {
            CheckBox[i] = CreateWindow(L"BUTTON", CheckBoxNames[i], WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                0, 0, 0, 0, hWnd, (HMENU)CheckBoxIDs[i], NULL, NULL);
            if (!CheckBox[i]) {
                MessageBox(NULL, L"Не удалось создать чекбокс", L"Ошибка", MB_ICONEXCLAMATION | MB_OK);
                break;
            }
        }

        // Освободите контекст устройства
        ReleaseDC(hWnd, hdc);

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hGraphPage, &ps);
        OutputDebugString(L"GraphPage_WM_PAINT\n");

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW));

        SetTextColor(hdc, 0x00FF0000); //выбираем цвет
        DrawGraph(hdc, ps.rcPaint, x, NUM, 3); //рисуем графики
        EndPaint(hGraphPage, &ps);
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        int color = 0xFF0000; //устанавливаем начальное значение цвета
        for (int i = 0; i < 3; i++)
        {
            if ((HWND)lParam == CheckBox[i])
            {
                HDC hdc = (HDC)wParam;
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, color);
                return (LRESULT)GetStockObject(NULL_BRUSH);   
            }
            color = color >> 8; //смещаем цвет на следующий
        }
    }
    break;
    case WM_CTLCOLORBTN: {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, TRANSPARENT);
        SetBkColor(hdcStatic, RGB(255, 255, 255));
        return (LRESULT)CreateSolidBrush(RGB(255, 255, 255));
    }
    case WM_SIZE:
    {
        RECT rectClient;
        GetClientRect(hWnd, &rectClient);

        // Получение дескриптора текста из пользовательских данных окна
        HWND hStaticText = (HWND)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (hStaticText)
        {
            // Установка размеров и позиции текста относительно правой границы
            SetWindowPos(hStaticText, HWND_TOP, rectClient.right - 170, 20, 160, rectClient.bottom, SWP_SHOWWINDOW);
        }

        // Получаем новые размеры окна
        int newWidth = LOWORD(lParam);
        int newHeight = HIWORD(lParam);

        // Определите координаты X и Y для первого чекбокса
        int startX = newWidth - 145; // Расстояние от правого края окна
        int startY = 100;
        int checkBoxWidth = 150;
        int checkBoxHeight = 30;
        int checkBoxSpacing = 10;

        // Переместите каждый чекбокс на новые координаты
        for (int i = 0; i < 3; ++i) {
            SetWindowPos(CheckBox[i], NULL, startX, startY + i * (checkBoxHeight + checkBoxSpacing), checkBoxWidth, checkBoxHeight, SWP_NOZORDER);
        }

        break;
    }
    case WM_COMMAND:
    {
        //фиксируем нажатие на CheckBox'ы
        int wmId = LOWORD(wParam);
        HWND hwndCtl = (HWND)lParam;
        HBRUSH hBrush;
        switch (wmId)
        {
        case ID_CHECKBOX_1:
            //обрабатываем нажатие на CheckBox 1
            OutputDebugString(L"ID_CHECKBOX_1!\n");
            InvalidateRect(hGraphPage, NULL, TRUE);
            break;
        case ID_CHECKBOX_2:
            //обрабатываем нажатие на CheckBox 2
            OutputDebugString(L"ID_CHECKBOX_2!\n");
            InvalidateRect(hGraphPage, NULL, TRUE);
            break;
        case ID_CHECKBOX_3:
            //обрабатываем нажатие на CheckBox 3
            OutputDebugString(L"ID_CHECKBOX_3!\n");
            InvalidateRect(hGraphPage, NULL, TRUE);
            break;
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

void DrawGraph(HDC hdc, RECT rectClient, double** x,
    int n, //количество точек
    int numrow = 1) //количество рядов данных (по умолчанию 1)
{
    SetBkMode(hdc, TRANSPARENT); //прозрачный фон
    double OffsetY, OffsetX; double MAX_X, MAX_Y; double ScaleX, ScaleY;
    double min, max; int height, width;
    int X, Y; //координаты на графике
    HPEN hpen;
    height = rectClient.bottom - rectClient.top;
    width = rectClient.right - rectClient.left - 200;
    //находим разброс по оси X и по оси Y
    min = x[0][0];
    max = x[0][0];
    for (int i = 0; i < n; i++)
    {
        if (x[0][i] < min) min = x[0][i];
        if (x[0][i] > max) max = x[0][i];
    }

    double temp = max - min;
    MAX_X = max - min; //разброс по оси X
    OffsetX = min * width / MAX_X; //смещение X
    ScaleX = (double)width / MAX_X; //масштабный коэффициент X

    max = 1.5; /////////!!!!!!!!!!!
    min = -1.5; ///

    MAX_Y = max - min; // разбро по оси Y
    OffsetY = max * height / (MAX_Y); // смещение Y
    ScaleY = (double)height / MAX_Y; // масштабный коэффициент Y
    //рисуем оси графика
    hpen = CreatePen(PS_SOLID, 2, 0); //создаем перо толщиной в 3 пикселя
    SelectObject(hdc, hpen);
    line(hdc, 10, OffsetY, width - 10, OffsetY); //рисуем горизонтальную ось
    
    //рисуем стрелку
    line(hdc, width - 20, OffsetY - 5, width - 10, OffsetY);
    line(hdc, width - 20, OffsetY + 5, width - 10, OffsetY);
    
    for (int i = 1; i < 9; i++) line(hdc, i * 0.118 * width, OffsetY, i * 0.118 * width, OffsetY - 3); //деления на горизонтальной шкале

    line(hdc, OffsetX + 10, 10, OffsetX + 10, height - 5); //рисуем вертикальную ось
    
    //рисуем стрелку
    line(hdc, OffsetX + 5, 20, OffsetX + 10, 10);
    line(hdc, OffsetX + 15, 20, OffsetX + 10, 10);
    
    for (int i = -2; i < 3; i++) line(hdc, OffsetX + 10, OffsetY + height / 6 * i, OffsetX + 15, OffsetY + height / 6 * i); //деления на вертикальной шкале

    DeleteObject(hpen); //удаляем выбранное перо

    //добавляем значения на оси
    int color = 0xFF0000; //выбираем цвет
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
        if (SendMessage(CheckBox[j - 1], BM_GETCHECK, 0, 0) == BST_CHECKED)
        {
            while (1)
            {
                hpen = CreatePen(PS_SOLID, 3, color); //создаем перо толщиной 3 пикселя
                SelectObject(hdc, hpen);
                X = (int)(OffsetX + x[0][0] * ScaleX); //координаты
                Y = (int)(OffsetY - x[j][0] * ScaleY);
                MoveToEx(hdc, X + 10, Y, 0);
                for (int i = 0; i < (n - 3); i++)
                {
                    X = OffsetX + 12 + x[0][i] * ScaleX;
                    Y = OffsetY - x[j][i] * ScaleY;
                    LineTo(hdc, X, Y);
                }
                DeleteObject(hpen);
                break;
            }
        }
        color = color >> 8; //сдвигаем цвет на один для следующего графика
    }
}

double** getGraphData(int n)
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
        f[1][i] = cos(x);
        f[2][i] = 3.14159265358979 * x / 10 - 1.0;
    }
    return f;
}
