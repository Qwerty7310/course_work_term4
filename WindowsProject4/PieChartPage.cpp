#include "PieChartPage.h"

LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT state;
    int length;

    RECT rectClient;
    GetClientRect(hPieChartPage, &rectClient);

    switch (message)
    {
    case WM_PAINT:
    {

        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hPieChartPage, &ps); // Используйте hdc, объявленный локально;

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW));

        if (flagDrawPie) DrawPieChart(hdc, ps.rcPaint); // построение графика
        DrawTextOnPieChartPage(hWnd, hdc, ps.rcPaint); // текст
        EndPaint(hPieChartPage, &ps);
        break;
    }
    break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            // ID вашей кнопки "Добавить"
        case ID_ADD_BUTTON_2:
            EnableWindow(deleteButtonPie, TRUE);
            numPieTextBox += 1;
            if (numPieTextBox == 5) EnableWindow(addButtonPie, FALSE);
            InvalidateRect(hPieChartPage, NULL, TRUE);
            break;
        case ID_DELETE_BUTTON_2:
            DestroyWindow(pieTextBox[numPieTextBox - 1][0]);
            DestroyWindow(pieTextBox[numPieTextBox - 1][1]);
            EnableWindow(addButtonPie, TRUE);
            numPieTextBox -= 1;
            if (numPieTextBox == 2) EnableWindow(deleteButtonPie, FALSE);
            InvalidateRect(hPieChartPage, NULL, TRUE);
            break;
        case ID_CREATE_BUTTON_2:
            if (!getPieChartData()) {
                //данные корректны
                for (int i = 0; i < numPieTextBox; i++)
                    for (int j = 0; j < 2; j++) {
                        length = GetWindowTextLength(pieTextBox[i][j]); // Получить длину текста в текстовом поле
                        pieText[i][j] = new TCHAR[length + 1];
                        GetWindowText(pieTextBox[i][j], pieText[i][j], length + 1); // Получить текст из текстового поля
                    }
                numSectors = numPieTextBox;
                flagDrawPie = true;
                InvalidateRect(hPieChartPage, NULL, TRUE);
            }
            break;
        default:
            break;
        }
        break;
    }
    break;
    }
    if (g_pPieChartPageProc)
        return CallWindowProc(g_pPieChartPageProc, hWnd, message, wParam, lParam);
    else
        return DefWindowProc(hWnd, message, wParam, lParam);
}

void DrawPieChart(HDC hdc, RECT rectClient)
{
    double min, max; int height, width;
    HPEN hpen;
    height = rectClient.bottom - rectClient.top - 50;
    width = rectClient.right - rectClient.left - 270;

    //шрифт для рисования текста
    HFONT hFont;
    // черный цвет
    SetTextColor(hdc, RGB(0, 0, 0));

    //Положение центра диаграммы и ее радиус
    //int xCenter = (rectClient.bottom + rectClient.top) / 2;
    int xCenter = rectClient.left + width / 1.9;
    int yCenter = rectClient.bottom - height / 1.8;
    double radius = min(height, width * 0.65) / 2.5;

    double startAngle = 90; //стартовый угол сектора
    double sweepAngl = 0; //угол поворота
    for (int i = 0; i < numSectors; i++) {
        startAngle += sweepAngl;
        sweepAngl = pieData[i + 1] / pieData[0] * 360;

        BeginPath(hdc);
        SelectObject(hdc, (HBRUSH)CreateSolidBrush(colors[i]));
        MoveToEx(hdc, xCenter, yCenter, (LPPOINT)NULL);
        AngleArc(hdc, xCenter, yCenter, radius, startAngle, sweepAngl);
        LineTo(hdc, xCenter, yCenter);
        EndPath(hdc);
        StrokeAndFillPath(hdc);

        //Добавляем значения на диграмму
        int xText = xCenter + radius * 0.7 * cos((startAngle + 0.5 * sweepAngl) / 180. * 3.14159265358979);
        int yText = yCenter - radius * 0.7 * sin((startAngle + 0.5 * sweepAngl) / 180. * 3.14159265358979);
        RECT rectText;
        rectText.left = xText - max(20, min(0.76 * radius, 50));
        rectText.right = xText + max(20, min(0.76 * radius, 50));
        rectText.top = yText - max(10, min(0.76 * radius, 20));
        rectText.bottom = yText + max(10, min(0.76 * radius, 20));

        wchar_t numStr[10]; // буфер для числа
        if (pieData[i + 1] == (int)pieData[i + 1])
            swprintf_s(numStr, L"%d", (int)pieData[i + 1]);
        else
            swprintf_s(numStr, L"%.2lf", pieData[i + 1]);

        hFont = CreateFont(max(15, min(0.16 * radius, 35)), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Arial");
        SelectObject(hdc, hFont); // выбираем шрифт
        SetBkMode(hdc, OPAQUE);
        DrawText(hdc, numStr, -1, &rectText, DT_WORDBREAK | DT_CENTER);

        //Добавляем подписи на диаграмму
        int xStLine = xCenter + radius * cos((startAngle + 0.5 * sweepAngl) / 180. * 3.14159265358979);
        int yStLine = yCenter - radius * sin((startAngle + 0.5 * sweepAngl) / 180. * 3.14159265358979);
        int xFinLine = xCenter + radius * 1.15 * cos((startAngle + 0.5 * sweepAngl) / 180. * 3.14159265358979);
        int yFinLine = yCenter - radius * 1.15 * sin((startAngle + 0.5 * sweepAngl) / 180. * 3.14159265358979);
        line(hdc, xStLine, yStLine, xFinLine, yFinLine);
        line(hdc, xFinLine, yFinLine, (xFinLine < xCenter) ? (xFinLine - max(50, min(0.7 * radius, 150))) : (xFinLine + max(50, min(0.7 * radius, 150))), yFinLine);

        hFont = CreateFont(max(13, min(0.12 * radius, 24)), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Arial");
        SelectObject(hdc, hFont); // выбираем шрифт
        SetBkMode(hdc, TRANSPARENT); // фон прозрачный
        if (xFinLine < xCenter) {
            rectText.left = xFinLine - max(100, min(0.75 * radius, 200));
            rectText.right = xFinLine - max(10, min(0.06 * radius, 10));
        }
        else {
            rectText.left = xFinLine + max(10, min(0.06 * radius, 10));
            rectText.right = xFinLine + max(100, min(0.75 * radius, 200));
        }
        rectText.top = yFinLine - max(15, min(0.12 * radius, 30));
        rectText.bottom = yFinLine;
        DrawText(hdc, pieText[i][1], -1, &rectText, DT_WORDBREAK | ((xFinLine < xCenter) ? DT_RIGHT : DT_LEFT));
    }
}

void DrawTextOnPieChartPage(HWND hWnd, HDC hdc, RECT rectClient) {
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

    for (int i = 0; i < numPieTextBox; i++) {
        TCHAR* buffer;

        length = GetWindowTextLength(pieTextBox[i][0]); // Получить длину текста в текстовом поле
        buffer = new TCHAR[length + 1]; // Создать буфер для хранения текста
        GetWindowText(pieTextBox[i][0], buffer, length + 1); // Получить текст из текстового поля
        DestroyWindow(pieTextBox[i][0]); //Удаление TextBox, чтобы заново нарисовать в другом месте
        pieTextBox[i][0] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 230, 40 + i * 30, 60, 20,
            hWnd, (HMENU)pieTextBoxIDs[i][0], (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
        SetWindowText(pieTextBox[i][0], buffer);

        length = GetWindowTextLength(pieTextBox[i][1]); // Получить длину текста в текстовом поле
        buffer = new TCHAR[length + 1]; // Создать буфер для хранения текста
        GetWindowText(pieTextBox[i][1], buffer, length + 1); // Получить текст из текстового поля
        DestroyWindow(pieTextBox[i][1]); //Удаление TextBox, чтобы заново нарисовать в другом месте
        pieTextBox[i][1] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 160, 40 + i * 30, 160, 20,
            hWnd, (HMENU)pieTextBoxIDs[i][1], (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
        SetWindowText(pieTextBox[i][1], buffer);

        delete[] buffer;
    }


    if (addButtonPie) isEnabled = IsWindowEnabled(addButtonPie);
    if (addButtonPie) DestroyWindow(addButtonPie); //Удаление, чтобы заново нарисовать в другом месте
    addButtonPie = CreateWindow(L"Button", L"Добавить", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 220, 40 + numPieTextBox * 30, 100, 25, hWnd,
        (HMENU)ID_ADD_BUTTON_2, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);
    EnableWindow(addButtonPie, isEnabled);

    if (deleteButtonPie) isEnabled = IsWindowEnabled(deleteButtonPie);
    if (deleteButtonPie) DestroyWindow(deleteButtonPie); //Удаление, чтобы заново нарисовать в другом месте
    deleteButtonPie = CreateWindow(L"Button", L"Убрать", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 110, 40 + numPieTextBox * 30, 100, 25, hWnd,
        (HMENU)ID_DELETE_BUTTON_2, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);
    EnableWindow(deleteButtonPie, isEnabled);

    if (createButtonPie) DestroyWindow(createButtonPie); //Удаление, чтобы заново нарисовать в другом месте
    createButtonPie = CreateWindow(L"Button", L"Построить", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 220, 40 + (numPieTextBox + 1) * 30, 210, 25, hWnd,
        (HMENU)ID_CREATE_BUTTON_2, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);

    DeleteObject(hFont); // Освобождение шрифта
}

int getPieChartData()
{
    wchar_t message[1024] = L"";

    bool error = false; //флаг ошибки данных
    int length; //длина текста в TextBox

    double* tempArr = new double[numPieTextBox];

    for (int i = 0; i < numPieTextBox; i++) {
        length = GetWindowTextLength(pieTextBox[i][0]); //получаем длину
        TCHAR* buffer1 = new TCHAR[length + 1]; // создаем буфер для хранения текста
        GetWindowText(pieTextBox[i][0], buffer1, length + 1); // получаем текст из текстового поля

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
        else {
            tempArr[i] = _wtof(buffer1);
            if (tempArr[i] <= 0) {
                error = true;
                const wchar_t* str;
                str = L"Значение меньше/равно 0:\tстрока ";
                wchar_t numStr[10]; // буфер для числа
                swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
                wcscat_s(message, 1024, str);
                wcscat_s(message, 1024, numStr);
                wcscat_s(message, 1024, L"\n");
            }
        }
        delete[] buffer1; //освобождаем память

        //проверяем наличие названий
        length = GetWindowTextLength(pieTextBox[i][1]); //получаем длину
        TCHAR* buffer2 = new TCHAR[length + 1]; // создаем буфер для хранения текста
        GetWindowText(pieTextBox[i][1], buffer2, length + 1); // получаем текст из текстового поля
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
    if (error) { //если данные некорректны
        MessageBox(NULL, message, L"Ошибка", MB_ICONEXCLAMATION | MB_OK); // сообщение об ошибке
        delete[] tempArr;
        return 1;
    }
    else { // если данные корректны
        //меняем массив
        delete[] pieData; //удаляем старый
        pieData = new double[numPieTextBox + 1]; //создаем новый
        for (int i = 0; i < numPieTextBox; i++) {
            pieData[i + 1] = tempArr[i];
        }
        delete[] tempArr;

        //находим вмаксимал
        double sum = 0;
        //double max = -1; //инициализируем переменную для поиска максимума
        for (int i = 0; i < numPieTextBox; i++) sum += pieData[i + 1]; //находим сумму
        pieData[0] = sum;
        
        return 0;
    }
}
