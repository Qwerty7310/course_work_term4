//#pragma once

#include "GraphPage.h"

/*----------------------------------- Процедура окна -----------------------------------*/

LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //OutputDebugString(L"GraphPageProc 1\n");
    //PAINTSTRUCT ps;
    UINT state;
    

    RECT rectClient;
    GetClientRect(hGraphPage, &rectClient);

    switch (message)
    {
    case WM_CREATE: {
        
        for (int i = 0; i < 3; i++) {
            CheckBox[i] = CreateWindowEx(NULL, L"BUTTON", CheckBoxNames[i], WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                rectClient.right - 190, 150 + i * 60, 110, 30, hGraphPage, (HMENU)CheckBoxIDs[i], NULL, NULL);
        }
        if (CheckBox[0]) MessageBox(hWnd, L"AAAAAAAAAA", L"AAAAAAAAAA", MB_OK);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hGraphPage, &ps); // Используйте hdc, объявленный локально;
        OutputDebugString(L"WM_PAINT_123!\n");

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW));

        SetTextColor(hdc, 0x00FF0000); // синий цвет букв
        DrawGraph(hdc, ps.rcPaint, x, NUM, 3); // построение графика
        DrawTextOnGraphPage(hdc, ps.rcPaint); // текст
        EndPaint(hGraphPage, &ps);
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        int color = 0xFF0000; // синее перо для первого графика
        for (int i = 0; i < 3; i++)
        {
            if ((HWND)lParam == CheckBox[i])
            {
                HDC hdc = (HDC)wParam;
                SetBkMode(hdc, TRANSPARENT);
                //SetTextColor(hdc, RGB(255, 0, 0));
                SetTextColor(hdc, color);
                return (LRESULT)GetStockObject(NULL_BRUSH);   
                
            }
            color = color >> 8; // изменение цвета пера для следующего ряда
        }
    }
    break;
    case WM_COMMAND:
    {
        // Получаем ID чекбокса, который был нажат
        int wmId = LOWORD(wParam);
        // Получаем дескриптор (HWND) чекбокса
        HWND hwndCtl = (HWND)lParam;
        HBRUSH hBrush;
        // Обрабатываем нажатие на чекбокс в зависимости от его ID
        switch (wmId)
        {
        case ID_CHECKBOX_1:
            // Обработка нажатия на чекбокс 1
            OutputDebugString(L"ID_CHECKBOX_1!\n");

            //hBrush = CreateSolidBrush(RGB(100, 100, 100)); // Создаем кисть с белым цветом
            //FillRect(hdc, &rectClient, hBrush); // Заливаем всю клиентскую область белым цветом
            //DeleteObject(hBrush); // Освобождаем ресурсы кисти


            //RedrawWindow(hGraphPage, &rectClient, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);

            //DestroyWindow(hGraphPage);
            //hGraphPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE,
                //0, 20, 760, 540, hWnd, nullptr, hInst, nullptr);

            InvalidateRect(hGraphPage, NULL, TRUE);
            //RedrawWindow(hGraphPage, &rectClient, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
            //UpdateWindow(hGraphPage);

            //DrawGraph(hdc, rectClient, x, NUM, 3); // построение графика

            /*
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
            }*/
            break;
        case ID_CHECKBOX_2:
            // Обработка нажатия на чекбокс 2
            OutputDebugString(L"ID_CHECKBOX_2!\n");

            //hBrush = CreateSolidBrush(RGB(255, 255, 255)); // Создаем кисть с белым цветом
            //FillRect(hdc, &rectClient, hBrush); // Заливаем всю клиентскую область белым цветом
            //DeleteObject(hBrush); // Освобождаем ресурсы кисти

            InvalidateRect(hGraphPage, NULL, TRUE);

            /*
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
            }*/
            break;
        case ID_CHECKBOX_3:
            // Обработка нажатия на чекбокс 3
            OutputDebugString(L"ID_CHECKBOX_3!\n");
            
            //hBrush = CreateSolidBrush(RGB(255, 255, 255)); // Создаем кисть с белым цветом
            //FillRect(hdc, &rectClient, hBrush); // Заливаем всю клиентскую область белым цветом
            //DeleteObject(hBrush); // Освобождаем ресурсы кисти

            InvalidateRect(hGraphPage, NULL, TRUE);

            /*
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
            }*/
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
    //TextOut(hdc, 21, 7, _T("Y = sin (X)"), 13); // Вывод текста y=sin(x) 8 символов
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
        //hpen = CreatePen(PS_SOLID, 3, color); // формирование пера толщиной 3 пикселя
        if (SendMessage(CheckBox[j - 1], BM_GETCHECK, 0, 0) == BST_CHECKED)
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
                //color = color >> 8; // изменение цвета пера для следующего ряда
                DeleteObject(hpen); // удаление текущего пера
                break;
            }
        }
        color = color >> 8; // изменение цвета пера для следующего ряда
        //DeleteObject(hpen); // удаление текущего пера
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

    RECT rectText = { rectClient.right - 180, 20, rectClient.right, rectClient.bottom };
    DrawText(hdc, L"Выберите графики,\nкоторые вы хотите увидеть:", -1, &rectText, DT_WORDBREAK);

    //state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    //DestroyWindow(hCheckBox1); //Удаление CheckBox, чтобы заново нарисовать в другом месте
    //hCheckBox1 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_1", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    //    rectClient.right - 190, 90, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_1, NULL, NULL);
    //if (state == BST_CHECKED) SendMessage(hCheckBox1, BM_SETCHECK, BST_CHECKED, 0);

    //state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
    //DestroyWindow(hCheckBox2); //Удаление CheckBox, чтобы заново нарисовать в другом месте
    //hCheckBox2 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_2", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    //    rectClient.right - 190, 120, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_2, NULL, NULL);
    //if (state == BST_CHECKED) SendMessage(hCheckBox2, BM_SETCHECK, BST_CHECKED, 0);

    //state = SendMessage(hCheckBox3, BM_GETCHECK, 0, 0);
    //DestroyWindow(hCheckBox3); //Удаление CheckBox, чтобы заново нарисовать в другом месте
    //hCheckBox3 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_3", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    //    rectClient.right - 190, 150, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_3, NULL, NULL);
    //if (state == BST_CHECKED) SendMessage(hCheckBox3, BM_SETCHECK, BST_CHECKED, 0);

    for (int i = 0; i < 3; i++) {
        state = SendMessage(CheckBox[i], BM_GETCHECK, 0, 0);
        DestroyWindow(CheckBox[i]); //Удаление CheckBox, чтобы заново нарисовать в другом месте
        CheckBox[i] = CreateWindowEx(NULL, L"BUTTON", CheckBoxNames[i], WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            rectClient.right - 170, 100 + i * 30, 110, 30, hGraphPage, (HMENU)CheckBoxIDs[i], NULL, NULL);
        if (state == BST_CHECKED) SendMessage(CheckBox[i], BM_SETCHECK, BST_CHECKED, 0);
    }

    // Освобождение шрифта
    DeleteObject(hFont);

}