#include "HistogramPage.h"

/*----------------------------------- ��������� ���� -----------------------------------*/

LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //OutputDebugString(L"GraphPageProc 1\n");
    //PAINTSTRUCT ps;
    UINT state;
    int length;

    RECT rectClient;
    GetClientRect(hHistogramPage, &rectClient);

    switch (message)
    {
    case WM_PAINT:
    {
        
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hHistogramPage, &ps); // ����������� hdc, ����������� ��������;
        //OutputDebugString(L"WM_HISTOGRAM_PAINT!\n");

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW));

        //SetTextColor(hdc, 0x00FF0000); // ����� ���� ����
        if (flagDrawHist) DrawHistogram(hdc, ps.rcPaint); // ���������� �������
        DrawTextOnHistogramPage(hWnd, hdc, ps.rcPaint); // �����
        EndPaint(hHistogramPage, &ps);
        //flagDrawHist = false;
        break;
    }
    break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            // ID ����� ������ "��������"
        case ID_ADD_BUTTON:
            EnableWindow(deleteButton, TRUE);
            numHistTextBox += 1;
            if (numHistTextBox == 5) EnableWindow(addButton, FALSE);
            //numColumns = numHistTextBox;
            InvalidateRect(hHistogramPage, NULL, TRUE);
            break;

            // ID ����� ������ "������"
        case ID_DELETE_BUTTON:
            DestroyWindow(HistTextBox[numHistTextBox - 1][0]);
            DestroyWindow(HistTextBox[numHistTextBox - 1][1]);
            EnableWindow(addButton, TRUE);
            numHistTextBox -= 1;
            //numColumns = numHistTextBox;
            if (numHistTextBox == 2) EnableWindow(deleteButton, FALSE);
            InvalidateRect(hHistogramPage, NULL, TRUE);
            break;
        case ID_CREATE_BUTTON:
            if (!getHistogramData()) {
                
                OutputDebugString(L"HIST_InvalidateRect!\n");
                //������ ���������

                for (int i = 0; i < numHistTextBox; i++)
                    for (int j = 0; j < 2; j++) {
                        length = GetWindowTextLength(HistTextBox[i][j]); // �������� ����� ������ � ��������� ����
                        HistText[i][j] = new TCHAR[length + 1];
                        GetWindowText(HistTextBox[i][j], HistText[i][j], length + 1); // �������� ����� �� ���������� ����
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

/*------------------------------ ������� ��������� ����������� ------------------------------*/

void DrawHistogram(HDC hdc, RECT rectClient)
{
    SetBkMode(hdc, TRANSPARENT); // ��� ����������
    double min, max; int height, width;
    HPEN hpen;
    height = rectClient.bottom - rectClient.top - 50;
    width = rectClient.right - rectClient.left - 270;

    int interval = (height - 20) / 5;

    //double sum = 0;
    //max = -1; //�������������� ���������� ��� ������ ���������
    //for (int i = 0; i < numColumns; i++) {
    //    sum += histData[i];
    //    if (histData[i] > max) max = histData[i];
    //}

    ////�������� ������� �����
    //int order = 0;
    //double tempDouble;
    //double maxLevel = 0;
    //int tempInt;
    //int multiplier = 0;

    //tempDouble = max;
    //if (tempDouble >= 1)
    //    while ((int)tempDouble > 0) {
    //        order += 1;
    //        tempDouble /= 10;
    //    }
    //else
    //    while ((int)tempDouble < 1) {
    //        order -= 1;
    //        tempDouble *= 10;
    //    }
    //if (order > 0)
    //    tempInt = (int)(max / pow(10, order - 2)) * pow(10, order - 2);
    //else
    //    tempInt = (int)(max / pow(10, order));

    //if (order == 1) {
    //    order = 2;
    //    tempInt *= 10;
    //    multiplier = 1;
    //}

    ////������� ������������ ������ �����������
    //for (int i = 0; i <= 10; i++) {
    //    int a = tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0);
    //    int b = 5 * (int)pow(10, (order > 1) ? (order - 3) : 0);
    //    int c = tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0);
    //    int d = (int)pow(10, (order > 1) ? (order) : (log10(5)));
    //    if (
    //        ((tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0)) % (5 * (int)pow(10, (order > 2) ? (order - 3) : 0)) == 0) &&
    //        ((tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0)) % ((order > 1) ? 1 : 5) == 0) && //(int)pow(10, (order > 1) ? (1) : (log10(5)))
    //        (tempInt + i * pow(10, (order > 1) ? (order - 2) : 0) >= max * pow(10, multiplier))
    //        ) {
    //        maxLevel = tempInt + i * pow(10, (order > 1) ? (order - 2) : 0);
    //        break;
    //    }
    //}
    //if (order < 0) maxLevel *= pow(10, order);

    //if (multiplier) maxLevel /= 10;

    // ��������� ���� ���������
    hpen = CreatePen(PS_SOLID, 3, RGB(100, 100, 100)); // ����� ���� �������� � 3 �������
    SelectObject(hdc, hpen);
    line(hdc, 50, rectClient.bottom - 30, width, rectClient.bottom - 30); // ��������� �������������� ���
    line(hdc, 50, rectClient.bottom - 30, 50, rectClient.top + 30); // ��������� �������������� ���
    DeleteObject(hpen); // �������� ������� ����

    hpen = CreatePen(PS_SOLID, 2, RGB(170, 170, 170)); // ������-����� ���� �������� � 2 �������
    SelectObject(hdc, hpen);
    for (int i = 1; i <= 5; i++)
        line(hdc, 40, rectClient.bottom - 30 - i * interval, width, rectClient.bottom - 30 - i * interval);
    DeleteObject(hpen); // �������� ������� ����

    //�������� �������� �����
    for (int i = 0; i <= 5; i++) {
        //����� ������
        HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Arial");

        // ������ ����
        SetTextColor(hdc, RGB(0, 0, 0));
        SelectObject(hdc, hFont); // �������� �����
        SetBkMode(hdc, TRANSPARENT); // ��� ����������

        RECT rectText = { 0, rectClient.bottom - 30 - i * interval - 10, 35, rectClient.bottom - 30 - i * interval + 10 };

        TCHAR str1[20];
        if ((i * maxLevel / 5) == (int)(i * maxLevel / 5))
            swprintf_s(str1, L"%d", (int)(i * maxLevel / 5));
        else
            swprintf_s(str1, L"%.2lf", (i * maxLevel / 5));

        DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT);
    }

    //����� ������
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");

    // ������ ����
    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, hFont); // �������� �����
    SetBkMode(hdc, TRANSPARENT); // ��� ����������

    int length; //����� ��������� ������

    //������ �������
    int leftColumnX = rectClient.left + 75;
    int rightColumnX = rectClient.left + width - 20;
    int widthColumn = (rightColumnX - leftColumnX) / numColumns;
    for (int i = 0; i < numColumns; i++) {
        RECT r; //��������� ��������� ��������� RECT - ���������� ��������������.
        r.left = leftColumnX + i * widthColumn; //����� ������� ����
        r.bottom = rectClient.bottom - 31;
        r.top = r.bottom - (5. * interval * histData[i] / maxLevel);
        r.right = leftColumnX + (i + 1) * widthColumn; //������ ������
        OutputDebugString(L"DrawColumns!\n");
        //��������� �������������
        FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(colors[i]));

        //TCHAR* buffer;
        //length = GetWindowTextLength(HistTextBox[i][0]); // �������� ����� ������ � ��������� ����
        //buffer = new TCHAR[length + 1]; // ������� ����� ��� �������� ������
        //GetWindowText(HistTextBox[i][0], buffer, length + 1); // �������� ����� �� ���������� ����

        RECT rectText;
        rectText.left = r.left;
        rectText.right = r.right;

        rectText.top = r.top - 20;
        rectText.bottom = r.top - 5;

        DrawText(hdc, HistText[i][0], -1, &rectText, DT_WORDBREAK | DT_CENTER);
    }

    //��������� ������� � �������
    hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");
    SelectObject(hdc, hFont); // �������� �����
    for (int i = 0; i < numColumns; i++) {

        //TCHAR* buffer;
        //length = GetWindowTextLength(HistTextBox[i][1]); // �������� ����� ������ � ��������� ����
        //buffer = new TCHAR[length + 1]; // ������� ����� ��� �������� ������
        //GetWindowText(HistTextBox[i][1], buffer, length + 1); // �������� ����� �� ���������� ����

        RECT rectText;
        rectText.left = leftColumnX + i * widthColumn; //����� ������� ����
        rectText.top = rectClient.bottom - 20;
        rectText.right = leftColumnX + (i + 1) * widthColumn; //������ ������
        rectText.bottom = rectText.top + 20;

        DrawText(hdc, HistText[i][1], -1, &rectText, DT_WORDBREAK | DT_CENTER);

        //delete[] buffer;
    }
}

void DrawTextOnHistogramPage(HWND hWnd, HDC hdc, RECT rectClient) {

    //LPTSTR word = (LPTSTR)"1111111111";
    //OutputDebugString(L"DrawTextOnHistogramPage!\n");
    RECT rectText;
    HFONT hFont;
    BOOL isEnabled = TRUE;

    int length;
    

    //�������� �������
    hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");

    // ����� ����
    SetTextColor(hdc, RGB(100, 100, 100));
    SelectObject(hdc, hFont); // �������� �����
    SetBkMode(hdc, TRANSPARENT); // ��� ����������

    rectText = { rectClient.right - 236, 10, rectClient.right, rectClient.bottom };
    DrawText(hdc, L"��������             ��������", -1, &rectText, DT_WORDBREAK);

    //HWND sortExInp = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 233, 40, 60, 20,
    //    hWnd, nullptr, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
    //HWND sortExInp1 = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 160, 40, 160, 20,
    //    hWnd, nullptr, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);

    //
    //int length = GetWindowTextLength(sortExInp); // �������� ����� ������ � ��������� ����
    //TCHAR* buffer = new TCHAR[length + 1]; // ������� ����� ��� �������� ������
    //GetWindowText(sortExInp, buffer, length + 1); // �������� ����� �� ���������� ����

    //delete[] buffer;

    //GetWindowText(sortExInp, word, 11);

    for (int i = 0; i < numHistTextBox; i++) {
        TCHAR* buffer;

        length = GetWindowTextLength(HistTextBox[i][0]); // �������� ����� ������ � ��������� ����
        buffer = new TCHAR[length + 1]; // ������� ����� ��� �������� ������
        GetWindowText(HistTextBox[i][0], buffer, length + 1); // �������� ����� �� ���������� ����
        DestroyWindow(HistTextBox[i][0]); //�������� TextBox, ����� ������ ���������� � ������ �����
        HistTextBox[i][0] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 230, 40 + i * 30, 60, 20,
            hWnd, (HMENU)HistTextBoxIDs[i][0], (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
        SetWindowText(HistTextBox[i][0], buffer);

        length = GetWindowTextLength(HistTextBox[i][1]); // �������� ����� ������ � ��������� ����
        buffer = new TCHAR[length + 1]; // ������� ����� ��� �������� ������
        GetWindowText(HistTextBox[i][1], buffer, length + 1); // �������� ����� �� ���������� ����
        DestroyWindow(HistTextBox[i][1]); //�������� TextBox, ����� ������ ���������� � ������ �����
        HistTextBox[i][1] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, rectClient.right - 160, 40 + i * 30, 160, 20,
            hWnd, (HMENU)HistTextBoxIDs[i][1], (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
        SetWindowText(HistTextBox[i][1], buffer);

        delete[] buffer;
    }


    if (addButton) isEnabled = IsWindowEnabled(addButton);
    if (addButton) DestroyWindow(addButton); //��������, ����� ������ ���������� � ������ �����
    addButton = CreateWindow(L"Button", L"��������", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 220, 40 + numHistTextBox * 30, 100, 25, hWnd,
        (HMENU)ID_ADD_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);
    EnableWindow(addButton, isEnabled);

    if (deleteButton) isEnabled = IsWindowEnabled(deleteButton);
    if (deleteButton) DestroyWindow(deleteButton); //��������, ����� ������ ���������� � ������ �����
    deleteButton = CreateWindow(L"Button", L"������", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 110, 40 + numHistTextBox * 30, 100, 25, hWnd,
        (HMENU)ID_DELETE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);
    EnableWindow(deleteButton, isEnabled);

    if (createButton) DestroyWindow(createButton); //��������, ����� ������ ���������� � ������ �����
    createButton = CreateWindow(L"Button", L"���������", WS_VISIBLE | WS_CHILD | WS_BORDER, rectClient.right - 220, 40 + (numHistTextBox + 1) * 30, 210, 25, hWnd,
        (HMENU)ID_CREATE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        nullptr);

    // ������������ ������
    DeleteObject(hFont);
    

}

int getHistogramData()
{
    wchar_t message[1024] = L"";

    bool error = false; //���� ������ ������
    int length; //����� ������ � TextBox
    
    double* tempArr = new double[numHistTextBox];

    //OutputDebugString(L"GetHistogramData!\n");
    for (int i = 0; i < numHistTextBox; i++) {
        length = GetWindowTextLength(HistTextBox[i][0]); //�������� �����
        TCHAR* buffer1 = new TCHAR[length + 1]; // ������� ����� ��� �������� ������
        GetWindowText(HistTextBox[i][0], buffer1, length + 1); // �������� ����� �� ���������� ����

        //��������� ������������ ��������
        if ((containsLetters(buffer1) == true) || (wcslen(buffer1) == 0)) {
            error = true;
            const wchar_t* str;
            if (containsLetters(buffer1) == true) {
                str = L"�������� �����������:\t������ ";
            }
            else {
                str = L"�������� �����������:\t������ ";
            }
            wchar_t numStr[10]; // ����� ��� �����
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
                str = L"�������� ������ ����:\t������ ";
                wchar_t numStr[10]; // ����� ��� �����
                swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
                wcscat_s(message, 1024, str);
                wcscat_s(message, 1024, numStr);
                wcscat_s(message, 1024, L"\n");
            }
            //TCHAR str1[20];
            //swprintf_s(str1, L"%lf", tempArr[i]);
            //MessageBox(NULL, str1, L"", MB_OK);

        }
        delete[] buffer1; //����������� ������

        //��������� ������� ��������
        length = GetWindowTextLength(HistTextBox[i][1]); //�������� �����
        TCHAR* buffer2 = new TCHAR[length + 1]; // ������� ����� ��� �������� ������
        GetWindowText(HistTextBox[i][1], buffer2, length + 1); // �������� ����� �� ���������� ����
        if (wcslen(buffer2) == 0) { //��������� �� �������
            error = true;
            const wchar_t* str;
            // ������ ������ ��� ���������� � ������
            str = L"�������� �����������:\t������ ";
            wchar_t numStr[10]; // ����� ��� �����
            swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
            wcscat_s(message, 1024, str);
            wcscat_s(message, 1024, numStr);
            wcscat_s(message, 1024, L"\n");
        }
        delete[] buffer2; //����������� ������
    }
    if (error) { //���� ������ �����������
        MessageBox(NULL, message, L"������", MB_ICONEXCLAMATION | MB_OK); // ��������� �� ������
        delete[] tempArr;
        return 1;
    }
    else { // ���� ������ ���������
        //������ ������
        delete[] histData; //������� ������
        histData = new double[numHistTextBox]; //������� �����
        for (int i = 0; i < numHistTextBox; i++) {
            histData[i] = tempArr[i];

            //TCHAR str1[20];
            //swprintf_s(str1, L"N: %lf", array[i]);
            //MessageBox(NULL, str1, L"", MB_OK);
        }
        delete[] tempArr;

        //������� ������ �����������
        double sum = 0;
        double max = -1; //�������������� ���������� ��� ������ ���������
        for (int i = 0; i < numHistTextBox; i++) {
            sum += histData[i];
            if (histData[i] > max) max = histData[i];
        }

        //�������� ������� �����
        maxLevel = 0;

        int order = 0;
        double tempDouble;
        int tempInt;
        int multiplier = 0;

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

        //������� ������������ ������ �����������
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

bool containsLetters(TCHAR* str) {
    while (*str) {
        if (_istalpha(*str) || *str == ',') {
            return true; // ���� ������� �����, ���������� true
        }
        str++; // ��������� � ���������� �������
    }
    return false; // ���� �� ������� �� ����� �����, ���������� false
}