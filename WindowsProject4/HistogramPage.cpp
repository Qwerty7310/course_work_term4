#include "HistogramPage.h"

/*----------------------------------- Процедура окна -----------------------------------*/

LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //OutputDebugString(L"GraphPageProc 1\n");
    //PAINTSTRUCT ps;
    UINT state;


    RECT rectClient;
    GetClientRect(hHistogramPage, &rectClient);

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hHistogramPage, &ps); // Используйте hdc, объявленный локально;
        //OutputDebugString(L"WM_HISTOGRAM_PAINT!\n");

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW));

        //SetTextColor(hdc, 0x00FF0000); // синий цвет букв
        //DrawHistogram(hdc, ps.rcPaint, x, NUM, 3); // построение графика
        DrawTextOnHistogramPage(hWnd, hdc, ps.rcPaint); // текст
        EndPaint(hHistogramPage, &ps);
        break;
    }
    break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            // ID вашей кнопки "Добавить"
        case ID_ADD_BUTTON:
            EnableWindow(deleteButton, TRUE);
            numHistTextBox += 1;
            if (numHistTextBox == 5) EnableWindow(addButton, FALSE);
            InvalidateRect(hHistogramPage, NULL, TRUE);
            break;

            // ID вашей кнопки "Убрать"
        case ID_DELETE_BUTTON:
            DestroyWindow(HistTextBox[numHistTextBox - 1][0]);
            DestroyWindow(HistTextBox[numHistTextBox - 1][1]);
            EnableWindow(addButton, TRUE);
            numHistTextBox -= 1;
            if (numHistTextBox == 2) EnableWindow(deleteButton, FALSE);
            InvalidateRect(hHistogramPage, NULL, TRUE);
            break;
        case ID_CREATE_BUTTON:
            //DrawHistogram(hdc, ps.rcPaint, x, NUM, 3); // построение графика
            HistData = new double[numHistTextBox];
            if (getHistogramData(HistData)) {
                // в случае некорректных данных
                delete[] HistData;
            }
            else {
                OutputDebugString(L"HIST_InvalidateRect!\n");
                InvalidateRect(hHistogramPage, NULL, TRUE);
            }
            break;
        default:
            break;
        }
        break;
    }
    break;
    }
    if (g_pGraphPageProc)
        return CallWindowProc(g_pGraphPageProc, hWnd, message, wParam, lParam);
    else
        return DefWindowProc(hWnd, message, wParam, lParam);
}

/*------------------------------ Функция рисования гистограммы ------------------------------*/

void DrawHistogram(HDC hdc, RECT rectClient, double** x, // массив данных
    int n, // количество точек
    int numrow = 1) // количество рядов данных (по умолчанию 1)
{
    OutputDebugString(L"DrawHistogram!\n");
    SetBkMode(hdc, TRANSPARENT); // фон прозрачный
    double OffsetY, OffsetX; double MAX_X, MAX_Y; double ScaleX, ScaleY;
    double min, max; int height, width;
    int X, Y; // координаты в окне (в рх)
    HPEN hpen;
    height = rectClient.bottom - rectClient.top - 50;
    width = rectClient.right - rectClient.left - 270;

    double* arr = new double[numHistTextBox];
    //getHistogramData(arr);

    // Отрисовка осей координат
    hpen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100)); // серое перо толщиной в 3 пикселя
    SelectObject(hdc, hpen);
    line(hdc, 50, rectClient.bottom - 30, width, rectClient.bottom - 30); // рисование горизонтальной оси
    line(hdc, 50, rectClient.bottom - 30, 50, rectClient.top + 30); // рисование горизонтальной оси

    //for (int i = 0; i < 9; i++) line(hdc, i * 0.118 * width, OffsetY, i * 0.118 * width, OffsetY - 3);
    
    //// отметки на горизонтальной оси
    //line(hdc, OffsetX + 10, 0, OffsetX + 10, height - 5); // рисование вертикальной оси line(hdc, OffsetX + 10, 0, OffsetX + 15, 10); // рисование правого вертикального оперения line(hdc, OffsetX + 10, 0, OffsetX + 5, 10); // рисование левого вертикального оперения
    //for (int i = -2; i < 3; i++) line(hdc, OffsetX + 10, OffsetY + height / 6 * i, OffsetX + 15, OffsetY + height / 6 * i);
    //// отметки на вертикальной оси
    DeleteObject(hpen); // удаление черного пера

    //            // Отрисовка графика функции
    //int color = 0xFF0000; // синее перо для первого ряда данных
    ////TextOut(hdc, 21, 7, _T("Y = sin (X)"), 11); // Вывод текста y=sin(x) 8 символов
    //TextOut(hdc, 27, height / 6 - 12, _T("1,0"), 3);
    //TextOut(hdc, 27, height / 3 - 12, _T("0,5"), 3);
    //TextOut(hdc, 27, 2 * height / 3 - 12, _T("-0,5"), 4);
    //TextOut(hdc, 27, 5 * height / 6 - 12, _T("-1,0"), 4);
    //TextOut(hdc, 15, 5 + 0.5 * height, _T("0,0"), 3);
    //TextOut(hdc, 0.92 * width * 0.125, 5 + 0.5 * height, _T("0,78"), 4);
    //TextOut(hdc, 0.92 * width * 0.25, 5 + 0.5 * height, _T("1,57"), 4);
    //TextOut(hdc, 0.92 * width * 0.375, 5 + 0.5 * height, _T("2,35"), 4);
    //TextOut(hdc, 0.92 * width * 0.5, 5 + 0.5 * height, _T("3,14"), 4);
    //TextOut(hdc, 0.92 * width * 0.625, 5 + 0.5 * height, _T("3,92"), 4);
    //TextOut(hdc, 0.92 * width * 0.75, 5 + 0.5 * height, _T("4,71"), 4);
    //TextOut(hdc, 0.92 * width * 0.875, 5 + 0.5 * height, _T("5,49"), 4);
    //TextOut(hdc, 0.92 * width, 5 + 0.5 * height, _T("6,28"), 4);
    //TextOut(hdc, 0.98 * width, 5 + 0.5 * height, _T("x"), 1);
    //for (int j = 1; j <= numrow; j++)
    //{
    //    //hpen = CreatePen(PS_SOLID, 3, color); // формирование пера толщиной 3 пикселя
    //        while (1)
    //        {
    //            hpen = CreatePen(PS_SOLID, 3, color); // формирование пера толщиной 3 пикселя
    //            SelectObject(hdc, hpen);
    //            X = (int)(OffsetX + x[0][0] * ScaleX); // начальная точка графика
    //            Y = (int)(OffsetY - x[j][0] * ScaleY);
    //            MoveToEx(hdc, X + 10, Y, 0); // перемещение в начальную точку
    //            for (int i = 0; i < (n - 3); i++)
    //            {
    //                X = OffsetX + 12 + x[0][i] * ScaleX;
    //                Y = OffsetY - x[j][i] * ScaleY;
    //                LineTo(hdc, X, Y);
    //            }
    //            //color = color >> 8; // изменение цвета пера для следующего ряда
    //            DeleteObject(hpen); // удаление текущего пера
    //            break;
    //        }
    //    color = color >> 8; // изменение цвета пера для следующего ряда
    //    //DeleteObject(hpen); // удаление текущего пера
    //}
}

void DrawTextOnHistogramPage(HWND hWnd, HDC hdc, RECT rectClient) {

    //LPTSTR word = (LPTSTR)"1111111111";
    //OutputDebugString(L"DrawTextOnHistogramPage!\n");
    RECT rectText;
    HFONT hFont;
    BOOL isEnabled = TRUE;

    int length;
    

    //Создание надписи
    hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");

    // серый цвет
    SetTextColor(hdc, RGB(100, 100, 100));
    SelectObject(hdc, hFont); // выбираем шрифт
    SetBkMode(hdc, TRANSPARENT); // фон прозрачный

    rectText = { rectClient.right - 236, 10, rectClient.right, rectClient.bottom };
    DrawText(hdc, L"Значение             Название", -1, &rectText, DT_WORDBREAK);

    //HWND sortExInp = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 233, 40, 60, 20,
    //    hWnd, nullptr, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
    //HWND sortExInp1 = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 160, 40, 160, 20,
    //    hWnd, nullptr, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);

    //
    //int length = GetWindowTextLength(sortExInp); // Получить длину текста в текстовом поле
    //TCHAR* buffer = new TCHAR[length + 1]; // Создать буфер для хранения текста
    //GetWindowText(sortExInp, buffer, length + 1); // Получить текст из текстового поля

    //delete[] buffer;

    //GetWindowText(sortExInp, word, 11);

    for (int i = 0; i < numHistTextBox; i++) {
        TCHAR* buffer;

        length = GetWindowTextLength(HistTextBox[i][0]); // Получить длину текста в текстовом поле
        buffer = new TCHAR[length + 1]; // Создать буфер для хранения текста
        GetWindowText(HistTextBox[i][0], buffer, length + 1); // Получить текст из текстового поля
        DestroyWindow(HistTextBox[i][0]); //Удаление TextBox, чтобы заново нарисовать в другом месте
        HistTextBox[i][0] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 230, 40 + i * 30, 60, 20,
            hWnd, (HMENU)HistTextBoxIDs[i][0], (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
        SetWindowText(HistTextBox[i][0], buffer);

        length = GetWindowTextLength(HistTextBox[i][1]); // Получить длину текста в текстовом поле
        buffer = new TCHAR[length + 1]; // Создать буфер для хранения текста
        GetWindowText(HistTextBox[i][1], buffer, length + 1); // Получить текст из текстового поля
        DestroyWindow(HistTextBox[i][1]); //Удаление TextBox, чтобы заново нарисовать в другом месте
        HistTextBox[i][1] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 160, 40 + i * 30, 160, 20,
            hWnd, (HMENU)HistTextBoxIDs[i][1], (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
        SetWindowText(HistTextBox[i][1], buffer);

        delete[] buffer;
    }


    if (addButton) isEnabled = IsWindowEnabled(addButton);
    if (addButton) DestroyWindow(addButton); //Удаление, чтобы заново нарисовать в другом месте
    addButton = CreateWindow(L"Button", L"Добавить", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 220, 40 + numHistTextBox * 30, 100, 25, hWnd,
        (HMENU)ID_ADD_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);
    EnableWindow(addButton, isEnabled);

    if (deleteButton) isEnabled = IsWindowEnabled(deleteButton);
    if (deleteButton) DestroyWindow(deleteButton); //Удаление, чтобы заново нарисовать в другом месте
    deleteButton = CreateWindow(L"Button", L"Убрать", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 110, 40 + numHistTextBox * 30, 100, 25, hWnd,
        (HMENU)ID_DELETE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);
    EnableWindow(deleteButton, isEnabled);

    if (createButton) DestroyWindow(createButton); //Удаление, чтобы заново нарисовать в другом месте
    createButton = CreateWindow(L"Button", L"Построить", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 220, 40 + (numHistTextBox + 1) * 30, 210, 25, hWnd,
        (HMENU)ID_CREATE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);

    // Освобождение шрифта
    DeleteObject(hFont);
    

}

int getHistogramData(double* arr)
{
    wchar_t message[1024] = L"";

    bool error = false; //флаг ошибки данных
    int length; //длина текста в TextBox

    //OutputDebugString(L"GetHistogramData!\n");
    for (int i = 0; i < numHistTextBox; i++) {
        length = GetWindowTextLength(HistTextBox[i][0]); //получаем длину
        TCHAR* buffer1 = new TCHAR[length + 1]; // создаем буфер для хранения текста
        GetWindowText(HistTextBox[i][0], buffer1, length + 1); // получаем текст из текстового поля

        //проверяем корректность значений
        if ((containsLetters(buffer1) == true) || (wcslen(buffer1) == 0)) {
            error = true;
            const wchar_t* str;
            if (containsLetters(buffer1) == true) {
                str = L"Значение некорректно:\tстрока ";
            }
            else {
                str = L"Значение отсутствует:\tстрока ";
            }
            wchar_t numStr[10]; // буфер для числа
            swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
            wcscat_s(message, 1024, str);
            wcscat_s(message, 1024, numStr);
            wcscat_s(message, 1024, L"\n");
        }
        delete[] buffer1; //освобождаем память

        //проверяем наличие названий
        length = GetWindowTextLength(HistTextBox[i][1]); //получаем длину
        TCHAR* buffer2 = new TCHAR[length + 1]; // создаем буфер для хранения текста
        GetWindowText(HistTextBox[i][1], buffer2, length + 1); // получаем текст из текстового поля
        if (wcslen(buffer2) == 0) { //проверяем на пустоту
            error = true;
            const wchar_t* str;
            // Пример строки для добавления в массив
            str = L"Название отсутствует:\tстрока ";
            wchar_t numStr[10]; // буфер для числа
            swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
            wcscat_s(message, 1024, str);
            wcscat_s(message, 1024, numStr);
            wcscat_s(message, 1024, L"\n");
        }
        delete[] buffer2; //освобождаем память
    }
    if (error) {
        MessageBox(NULL, message, L"Ошибка", MB_ICONEXCLAMATION | MB_OK); // сообщение об ошибке
        return 1;
    }
    else
        return 0;
}

bool containsLetters(TCHAR* str) {
    while (*str) {
        if (_istalpha(*str)) {
            return true; // Если найдена буква, возвращаем true
        }
        str++; // Переходим к следующему символу
    }
    return false; // Если не найдено ни одной буквы, возвращаем false
}