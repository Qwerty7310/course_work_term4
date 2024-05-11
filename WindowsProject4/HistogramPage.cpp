#include "HistogramPage.h"

LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT state;
    int length;

    RECT rectClient;
    GetClientRect(hHistogramPage, &rectClient);

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hHistogramPage, &ps);

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW));

        if (flagDrawHist) DrawHistogram(hdc, ps.rcPaint); // рисуем гистограммы
        else { //иначе выводим информационное сообщение
            HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, L"Arial");
            SelectObject(hdc, hFont); //выбираем шрифт
            RECT rectText;
            rectText.left = rect.left + (rect.right - rect.left - 270) / 4;//верхний левый угол
            rectText.top = rect.top + (rect.bottom - rect.top + 50) / 4;
            rectText.right = rect.left + (rect.right - rect.left - 270) * 3 / 4; //нижний правый угол
            rectText.bottom = rect.top + (rect.bottom - rect.top + 50) * 3 / 4;

            DrawText(hdc, L"Введите Ваши данные для построения гистограммы", -1, &rectText, DT_WORDBREAK | DT_CENTER | DT_VCENTER);
        }
        DrawTextOnHistogramPage(hWnd, hdc, ps.rcPaint); // рисуем текст
        EndPaint(hHistogramPage, &ps);
        break;
    }
    break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_ADD_BUTTON:
            EnableWindow(deleteButtonHist, TRUE);
            numHistTextBox += 1;
            if (numHistTextBox == 5) EnableWindow(addButtonHist, FALSE);
            InvalidateRect(hHistogramPage, NULL, TRUE);
            break;
        case ID_DELETE_BUTTON:
            DestroyWindow(HistTextBox[numHistTextBox - 1][0]);
            DestroyWindow(HistTextBox[numHistTextBox - 1][1]);
            EnableWindow(addButtonHist, TRUE);
            numHistTextBox -= 1;
            if (numHistTextBox == 2) EnableWindow(deleteButtonHist, FALSE);
            InvalidateRect(hHistogramPage, NULL, TRUE);
            break;
        case ID_CREATE_BUTTON:
            if (!getHistogramData()) {
                //данные корректны
                for (int i = 0; i < numHistTextBox; i++)
                    for (int j = 0; j < 2; j++) {
                        length = GetWindowTextLength(HistTextBox[i][j]); // получаем длину текста из текстового поля
                        HistText[i][j] = new TCHAR[length + 1];
                        GetWindowText(HistTextBox[i][j], HistText[i][j], length + 1); // записываем текст в отдельный массив
                    }
                numColumns = numHistTextBox;
                flagDrawHist = true;
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

void DrawHistogram(HDC hdc, RECT rectClient)
{
    SetBkMode(hdc, TRANSPARENT); //прозрачный фон
    double min, max; int height, width;
    HPEN hpen;
    height = rectClient.bottom - rectClient.top - 50;
    width = rectClient.right - rectClient.left - 270;

    int interval = (height - 20) / 5; //расстояние между горизонтальными линиями


    //рисуем оси гистограммы
    hpen = CreatePen(PS_SOLID, 3, RGB(100, 100, 100)); // перо толщиной в 3 пикселя
    SelectObject(hdc, hpen);
    line(hdc, 50, rectClient.bottom - 30, width, rectClient.bottom - 30); // горизонтальная ось гистограммы
    line(hdc, 50, rectClient.bottom - 30, 50, rectClient.top + 30); // вертикальная ось гистограммы
    DeleteObject(hpen);

    hpen = CreatePen(PS_SOLID, 2, RGB(170, 170, 170)); // перо толщиной в 2 пикселя
    SelectObject(hdc, hpen);
    for (int i = 1; i <= 5; i++)
        line(hdc, 40, rectClient.bottom - 30 - i * interval, width, rectClient.bottom - 30 - i * interval); //горизонтальная линия уровня гистограммы
    DeleteObject(hpen);

    //добавляем шкалу гистограммы
    for (int i = 0; i <= 5; i++) {
        //создаем шрифт
        HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Arial");

        SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет текста
        SelectObject(hdc, hFont); //выбираем шрифт
        SetBkMode(hdc, TRANSPARENT); //прозрачный фон

        RECT rectText = { 0, rectClient.bottom - 30 - i * interval - 10, 35, rectClient.bottom - 30 - i * interval + 10 };

        TCHAR str1[20];
        if ((i * maxLevel / 5) == (int)(i * maxLevel / 5))
            swprintf_s(str1, L"%d", (int)(i * maxLevel / 5));
        else
            swprintf_s(str1, L"%.0lf", (i * maxLevel / 5));

        DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT);
    }

    //выбираем шрифт
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");

    SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет
    SelectObject(hdc, hFont); //выбираем шрифт
    SetBkMode(hdc, TRANSPARENT); //прозрачный фон

    int length; //длина текста

    //рисуем столбцы
    int leftColumnX = rectClient.left + 75;
    int rightColumnX = rectClient.left + width - 20;
    int widthColumn = (rightColumnX - leftColumnX) / numColumns;
    for (int i = 0; i < numColumns; i++) {
        RECT r;
        r.left = leftColumnX + i * widthColumn;
        r.bottom = rectClient.bottom - 31;
        r.top = r.bottom - (5. * interval * histData[i] / maxLevel);
        r.right = leftColumnX + (i + 1) * widthColumn;
        OutputDebugString(L"DrawColumns!\n");
        //рисуем столбец
        FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(colors[i]));

        RECT rectText;
        rectText.left = r.left;
        rectText.right = r.right;

        rectText.top = r.top - 20;
        rectText.bottom = r.top - 5;

        wchar_t numStr[10]; //буфер для числового значения
        //добавляем числовое значение
        if (histData[i] == (int)histData[i])
            swprintf_s(numStr, L"%d", (int)histData[i]);
        else
            swprintf_s(numStr, L"%.2lf", histData[i]);
        DrawText(hdc, numStr, -1, &rectText, DT_WORDBREAK | DT_CENTER);
    }

    //добавляем подписи к стобцам
    hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");
    SelectObject(hdc, hFont); //выбираем новый шрифт
    for (int i = 0; i < numColumns; i++) {
        RECT rectText;
        rectText.left = leftColumnX + i * widthColumn; //верхний левый угол
        rectText.top = rectClient.bottom - 20;
        rectText.right = leftColumnX + (i + 1) * widthColumn; //нижний правый угол
        rectText.bottom = rectText.top + 20;

        DrawText(hdc, HistText[i][1], -1, &rectText, DT_WORDBREAK | DT_CENTER);
    }
}

void DrawTextOnHistogramPage(HWND hWnd, HDC hdc, RECT rectClient) {
    RECT rectText;
    HFONT hFont;
    BOOL isEnabled = TRUE;
    int length;
    
    //создаем шрифт
    hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");

    SetTextColor(hdc, RGB(100, 100, 100)); //выбираем цвет
    SelectObject(hdc, hFont); //выбираем шрифт
    SetBkMode(hdc, TRANSPARENT); //прозрачный фон

    rectText = { rectClient.right - 236, 10, rectClient.right, rectClient.bottom };
    DrawText(hdc, L"Значение             Название", -1, &rectText, DT_WORDBREAK);

    for (int i = 0; i < numHistTextBox; i++) {
        TCHAR* buffer;

        length = GetWindowTextLength(HistTextBox[i][0]); //получаем длину строки в текстовом поле
        buffer = new TCHAR[length + 1]; //создаем буффер для хранения строки
        GetWindowText(HistTextBox[i][0], buffer, length + 1); //получаем текст из текстового поля
        DestroyWindow(HistTextBox[i][0]); //удаляем TextBox, чтобы заново нарисовать в другом месте
        HistTextBox[i][0] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 230, 40 + i * 30, 60, 20,
            hWnd, (HMENU)HistTextBoxIDs[i][0], (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
        SetWindowText(HistTextBox[i][0], buffer);

        length = GetWindowTextLength(HistTextBox[i][1]); //получаем длину строки в текстовом поле
        buffer = new TCHAR[length + 1]; //создаем буффер для хранения строки
        GetWindowText(HistTextBox[i][1], buffer, length + 1); //получаем текст из текстового поля
        DestroyWindow(HistTextBox[i][1]); //удаляем TextBox, чтобы заново нарисовать в другом месте
        HistTextBox[i][1] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 160, 40 + i * 30, 160, 20,
            hWnd, (HMENU)HistTextBoxIDs[i][1], (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
        SetWindowText(HistTextBox[i][1], buffer);

        delete[] buffer;
    }

    if (addButtonHist) isEnabled = IsWindowEnabled(addButtonHist);
    if (addButtonHist) DestroyWindow(addButtonHist); //удаляем кнопку, чтобы заново нарисовать в другом месте
    addButtonHist = CreateWindow(L"Button", L"Добавить", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 220, 40 + numHistTextBox * 30, 100, 25, hWnd,
        (HMENU)ID_ADD_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);
    EnableWindow(addButtonHist, isEnabled);

    if (deleteButtonHist) isEnabled = IsWindowEnabled(deleteButtonHist);
    if (deleteButtonHist) DestroyWindow(deleteButtonHist); //удаляем кнопку, чтобы заново нарисовать в другом месте
    deleteButtonHist = CreateWindow(L"Button", L"Убрать", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 110, 40 + numHistTextBox * 30, 100, 25, hWnd,
        (HMENU)ID_DELETE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);
    EnableWindow(deleteButtonHist, isEnabled);

    if (createButtonHist) DestroyWindow(createButtonHist); //удаляем кнопку, чтобы заново нарисовать в другом месте
    createButtonHist = CreateWindow(L"Button", L"Построить", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 220, 40 + (numHistTextBox + 1) * 30, 210, 25, hWnd,
        (HMENU)ID_CREATE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);

    DeleteObject(hFont); //удаляем шрифт
}

int getHistogramData()
{
    wchar_t message[1024] = L"";

    bool error = false; //флаг наличия ошибки в введенных данных
    int length; //длина текста в TextBox
    double* tempArr = new double[numHistTextBox]; //временный массив для хранения данных

    for (int i = 0; i < numHistTextBox; i++) {
        length = GetWindowTextLength(HistTextBox[i][0]); //получаем длину
        TCHAR* buffer1 = new TCHAR[length + 1]; //создаем буфер для хранения строки
        GetWindowText(HistTextBox[i][0], buffer1, length + 1); // получаем текст из текстового поля

        //проверяем корректность введенных данных
        if ((containsLetters(buffer1) == true) || (wcslen(buffer1) == 0)) {
            error = true;
            const wchar_t* str;
            if (containsLetters(buffer1) == true) {
                str = L"Значение некорректно:\tстрока ";
            }
            else {
                str = L"Значение отсутствует:\tстрока ";
            }
            wchar_t numStr[10]; //буфер для числа
            swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
            wcscat_s(message, 1024, str);
            wcscat_s(message, 1024, numStr);
            wcscat_s(message, 1024, L"\n");
        }
        else {
            tempArr[i] = _wtof(buffer1);
            if (tempArr[i] < 0) {
                error = true;
                const wchar_t* str;
                str = L"Значение меньше нуля:\tстрока ";
                wchar_t numStr[10]; //буфер для числа
                swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
                wcscat_s(message, 1024, str);
                wcscat_s(message, 1024, numStr);
                wcscat_s(message, 1024, L"\n");
            }
        }
        delete[] buffer1; //удаяем буфер

        //проверяем названия на пустоту
        length = GetWindowTextLength(HistTextBox[i][1]); //получаем длину
        TCHAR* buffer2 = new TCHAR[length + 1]; //создаем буфер для хранения строки
        GetWindowText(HistTextBox[i][1], buffer2, length + 1); //получаем данные из текстового поля
        if (wcslen(buffer2) == 0) { //если название отсутствует
            error = true;
            const wchar_t* str;
            //добавляем название ошибки к сообщению
            str = L"Название отсутствует:\tстрока ";
            wchar_t numStr[10]; //буфер для числа
            swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
            wcscat_s(message, 1024, str);
            wcscat_s(message, 1024, numStr);
            wcscat_s(message, 1024, L"\n");
        }
        delete[] buffer2; //удаляем буфер
    }
    if (error) { //если данные некорректны
        MessageBox(NULL, message, L"Ошибка", MB_ICONEXCLAMATION | MB_OK); //сообщение об ошибке
        delete[] tempArr;
        return 1;
    }
    else { //данные корректны
        //создаем новый массив данных
        delete[] histData; //удаляем старый
        histData = new double[numHistTextBox]; //создаем новый
        for (int i = 0; i < numHistTextBox; i++) {
            histData[i] = tempArr[i];
        }
        delete[] tempArr;

        //������� ������ �����������
        double sum = 0;
        double max = -1; //�������������� ���������� ��� ������ ���������
        for (int i = 0; i < numHistTextBox; i++) {
            sum += histData[i];
            if (histData[i] > max) max = histData[i];
        }
        if (max == 0) { //если все числа равны 0
            MessageBox(NULL, L"Должно быть хотя бы 1 положительное число!", L"Ошибка", MB_ICONEXCLAMATION | MB_OK); //сообщение об ошибке
            return 1;
        }
        else { //находим максимальное значение уровня гистограммы
            maxLevel = 0; //максимальное значение уровня гистограммы

            int order = 0; //порядок числа
            double tempDouble;
            int tempInt;
            int multiplier = 0; //множитель

            tempDouble = max;
            if (tempDouble >= 1)
                while ((int)tempDouble > 0) {
                    order += 1;
                    tempDouble /= 10;
                }
            else
                while ((int)tempDouble < 1) {
                    order -= 1;
                    tempDouble *= 10;
                }
            if (order > 0)
                tempInt = (int)(max / pow(10, order - 2)) * pow(10, order - 2);
            else
                tempInt = (int)(max * pow(10, -order));

            if (order == 1) {
                order = 2;
                tempInt *= 10;
                multiplier = 1;
            }

            //находим максимальный уровень гистограммы
            for (int i = 0; i <= 10; i++) {
                int a = tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0);
                int b = 5 * (int)pow(10, (order > 1) ? (order - 3) : 0);
                int c = tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0);
                int d = (int)pow(10, (order > 1) ? (order) : (log10(5)));
                if (
                    ((tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0)) % (5 * (int)pow(10, (order > 2) ? (order - 3) : 0)) == 0) &&
                    ((tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0)) % ((order > 1) ? 1 : 5) == 0) && //(int)pow(10, (order > 1) ? (1) : (log10(5)))
                    (tempInt + i * pow(10, (order > 1) ? (order - 2) : 0) >= max * pow(10, multiplier)) &&
                    ((order > 0) || (order < 0) && (tempInt + i * pow(10, (order > 1) ? (order - 2) : 0) >= max * pow(10, -order)))
                    ) {
                    maxLevel = tempInt + i * pow(10, (order > 1) ? (order - 2) : 0);
                    break;
                }
            }
            if (order < 0) maxLevel *= pow(10, order);

            if (multiplier) maxLevel /= 10;

            return 0;
        }
    }
}