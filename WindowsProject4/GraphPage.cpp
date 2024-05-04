//#pragma once

#include "GraphPage.h"

/*----------------------------------- ��������� ���� -----------------------------------*/

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
        HDC hdc = BeginPaint(hGraphPage, &ps); // ����������� hdc, ����������� ��������;
        OutputDebugString(L"WM_PAINT_123!\n");

        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW));

        SetTextColor(hdc, 0x00FF0000); // ����� ���� ����
        DrawGraph(hdc, ps.rcPaint, x, NUM, 3); // ���������� �������
        DrawTextOnGraphPage(hdc, ps.rcPaint); // �����
        EndPaint(hGraphPage, &ps);
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        int color = 0xFF0000; // ����� ���� ��� ������� �������
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
            color = color >> 8; // ��������� ����� ���� ��� ���������� ����
        }
    }
    break;
    case WM_COMMAND:
    {
        // �������� ID ��������, ������� ��� �����
        int wmId = LOWORD(wParam);
        // �������� ���������� (HWND) ��������
        HWND hwndCtl = (HWND)lParam;
        HBRUSH hBrush;
        // ������������ ������� �� ������� � ����������� �� ��� ID
        switch (wmId)
        {
        case ID_CHECKBOX_1:
            // ��������� ������� �� ������� 1
            OutputDebugString(L"ID_CHECKBOX_1!\n");

            //hBrush = CreateSolidBrush(RGB(100, 100, 100)); // ������� ����� � ����� ������
            //FillRect(hdc, &rectClient, hBrush); // �������� ��� ���������� ������� ����� ������
            //DeleteObject(hBrush); // ����������� ������� �����


            //RedrawWindow(hGraphPage, &rectClient, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);

            //DestroyWindow(hGraphPage);
            //hGraphPage = CreateWindow(WC_STATIC, L"", WS_CHILD | WS_VISIBLE,
                //0, 20, 760, 540, hWnd, nullptr, hInst, nullptr);

            InvalidateRect(hGraphPage, NULL, TRUE);
            //RedrawWindow(hGraphPage, &rectClient, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
            //UpdateWindow(hGraphPage);

            //DrawGraph(hdc, rectClient, x, NUM, 3); // ���������� �������

            /*
            // �������� ������� ������ ��������
            state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);

            // ��������� ������ ��������
            switch (state)
            {
            case BST_CHECKED: // ������� �����
                // ���������� ��������� ��������
                SendMessage(hCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
                break;
            case BST_UNCHECKED: // ������� �� �����
                // ������������� ��������� �������� � "�������"
                SendMessage(hCheckBox1, BM_SETCHECK, BST_CHECKED, 0);
                break;
            default:
                // �������������� ������
                break;
            }*/
            break;
        case ID_CHECKBOX_2:
            // ��������� ������� �� ������� 2
            OutputDebugString(L"ID_CHECKBOX_2!\n");

            //hBrush = CreateSolidBrush(RGB(255, 255, 255)); // ������� ����� � ����� ������
            //FillRect(hdc, &rectClient, hBrush); // �������� ��� ���������� ������� ����� ������
            //DeleteObject(hBrush); // ����������� ������� �����

            InvalidateRect(hGraphPage, NULL, TRUE);

            /*
            // �������� ������� ������ ��������
            state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
            // ��������� ������ ��������
            switch (state)
            {
            case BST_CHECKED: // ������� �����
                // ���������� ��������� ��������
                SendMessage(hCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
                break;
            case BST_UNCHECKED: // ������� �� �����
                // ������������� ��������� �������� � "�������"
                SendMessage(hCheckBox2, BM_SETCHECK, BST_CHECKED, 0);
                break;
            default:
                // �������������� ������
                break;
            }*/
            break;
        case ID_CHECKBOX_3:
            // ��������� ������� �� ������� 3
            OutputDebugString(L"ID_CHECKBOX_3!\n");
            
            //hBrush = CreateSolidBrush(RGB(255, 255, 255)); // ������� ����� � ����� ������
            //FillRect(hdc, &rectClient, hBrush); // �������� ��� ���������� ������� ����� ������
            //DeleteObject(hBrush); // ����������� ������� �����

            InvalidateRect(hGraphPage, NULL, TRUE);

            /*
            // �������� ������� ������ ��������
            state = SendMessage(hCheckBox3, BM_GETCHECK, 0, 0);
            // ��������� ������ ��������
            switch (state)
            {
            case BST_CHECKED: // ������� �����
                // ���������� ��������� ��������
                SendMessage(hCheckBox3, BM_SETCHECK, BST_UNCHECKED, 0);
                break;
            case BST_UNCHECKED: // ������� �� �����
                // ������������� ��������� �������� � "�������"
                SendMessage(hCheckBox3, BM_SETCHECK, BST_CHECKED, 0);
                break;
            default:
                // �������������� ������
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

/*------------------------------ ������� ��������� ������� ------------------------------*/

void DrawGraph(HDC hdc, RECT rectClient, double** x, // ������ ������
    int n, // ���������� �����
    int numrow = 1) // ���������� ����� ������ (�� ��������� 1)
{
    SetBkMode(hdc, TRANSPARENT); // ��� ����������
    double OffsetY, OffsetX; double MAX_X, MAX_Y; double ScaleX, ScaleY;
    double min, max; int height, width;
    int X, Y; // ���������� � ���� (� ��)
    HPEN hpen;
    height = rectClient.bottom - rectClient.top;
    width = rectClient.right - rectClient.left - 200;
    // ������� ���������� �������� �
    min = x[0][0];
    max = x[0][0];
    for (int i = 0; i < n; i++)
    {
        if (x[0][i] < min) min = x[0][i];
        if (x[0][i] > max) max = x[0][i];
    }

    double temp = max - min;
    MAX_X = max - min; // ������� �� ��� �
    OffsetX = min * width / MAX_X; // �������� X
    ScaleX = (double)width / MAX_X; // ���������� ����������� �
    // ������� ���������� �������� Y
    max = 1.5; /////////!!!!!!!!!!!
    min = -1.5; ///

    MAX_Y = max - min; // ������� �� ��� Y
    OffsetY = max * height / (MAX_Y); // �������� Y
    ScaleY = (double)height / MAX_Y; // ���������� ����������� Y
    // ��������� ���� ���������
    hpen = CreatePen(PS_SOLID, 3, 0); // ������ ���� �������� � 3 �������
    SelectObject(hdc, hpen);
    line(hdc, 10, OffsetY, width - 10, OffsetY); // ��������� �������������� ��� line(hdc, width - 10, OffsetY, width - 15, OffsetY - 5); // ������� �������������� �������� line(hdc, width - 10, OffsetY, width - 15, OffsetY + 5); // ������ �������������� ��������
    for (int i = 0; i < 9; i++) line(hdc, i * 0.118 * width, OffsetY, i * 0.118 * width, OffsetY - 3);
    // ������� �� �������������� ���
    line(hdc, OffsetX + 10, 0, OffsetX + 10, height - 5); // ��������� ������������ ��� line(hdc, OffsetX + 10, 0, OffsetX + 15, 10); // ��������� ������� ������������� �������� line(hdc, OffsetX + 10, 0, OffsetX + 5, 10); // ��������� ������ ������������� ��������
    for (int i = -2; i < 3; i++) line(hdc, OffsetX + 10, OffsetY + height / 6 * i, OffsetX + 15, OffsetY + height / 6 * i);
    // ������� �� ������������ ���
    DeleteObject(hpen); // �������� ������� ����

                // ��������� ������� �������
    int color = 0xFF0000; // ����� ���� ��� ������� ���� ������
    //TextOut(hdc, 21, 7, _T("Y = sin (X)"), 13); // ����� ������ y=sin(x) 8 ��������
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
        //hpen = CreatePen(PS_SOLID, 3, color); // ������������ ���� �������� 3 �������
        if (SendMessage(CheckBox[j - 1], BM_GETCHECK, 0, 0) == BST_CHECKED)
        {
            while (1)
            {
                hpen = CreatePen(PS_SOLID, 3, color); // ������������ ���� �������� 3 �������
                SelectObject(hdc, hpen);
                X = (int)(OffsetX + x[0][0] * ScaleX); // ��������� ����� �������
                Y = (int)(OffsetY - x[j][0] * ScaleY);
                MoveToEx(hdc, X + 10, Y, 0); // ����������� � ��������� �����
                for (int i = 0; i < (n - 3); i++)
                {
                    X = OffsetX + 12 + x[0][i] * ScaleX;
                    Y = OffsetY - x[j][i] * ScaleY;
                    LineTo(hdc, X, Y);
                }
                //color = color >> 8; // ��������� ����� ���� ��� ���������� ����
                DeleteObject(hpen); // �������� �������� ����
                break;
            }
        }
        color = color >> 8; // ��������� ����� ���� ��� ���������� ����
        //DeleteObject(hpen); // �������� �������� ����
    }
}

void DrawTextOnGraphPage(HDC hdc, RECT rectClient) {
    UINT state;

    //�������� �������
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");

    // ����� ����
    SetTextColor(hdc, RGB(100, 100, 100));
    SelectObject(hdc, hFont); // �������� �����
    SetBkMode(hdc, TRANSPARENT); // ��� ����������

    // ������ ����� � ���������� �����
    //TextOut(hdc, rectClient.right - 200, 20, L"�������� �������,\n������� �� ������ �������:", wcslen(L"�������� �������,\n������� �� ������ �������:"));

    RECT rectText = { rectClient.right - 180, 20, rectClient.right, rectClient.bottom };
    DrawText(hdc, L"�������� �������,\n������� �� ������ �������:", -1, &rectText, DT_WORDBREAK);

    //state = SendMessage(hCheckBox1, BM_GETCHECK, 0, 0);
    //DestroyWindow(hCheckBox1); //�������� CheckBox, ����� ������ ���������� � ������ �����
    //hCheckBox1 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_1", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    //    rectClient.right - 190, 90, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_1, NULL, NULL);
    //if (state == BST_CHECKED) SendMessage(hCheckBox1, BM_SETCHECK, BST_CHECKED, 0);

    //state = SendMessage(hCheckBox2, BM_GETCHECK, 0, 0);
    //DestroyWindow(hCheckBox2); //�������� CheckBox, ����� ������ ���������� � ������ �����
    //hCheckBox2 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_2", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    //    rectClient.right - 190, 120, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_2, NULL, NULL);
    //if (state == BST_CHECKED) SendMessage(hCheckBox2, BM_SETCHECK, BST_CHECKED, 0);

    //state = SendMessage(hCheckBox3, BM_GETCHECK, 0, 0);
    //DestroyWindow(hCheckBox3); //�������� CheckBox, ����� ������ ���������� � ������ �����
    //hCheckBox3 = CreateWindowEx(NULL, L"BUTTON", L"Checkbox_3", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    //    rectClient.right - 190, 150, 110, 30, hGraphPage, (HMENU)ID_CHECKBOX_3, NULL, NULL);
    //if (state == BST_CHECKED) SendMessage(hCheckBox3, BM_SETCHECK, BST_CHECKED, 0);

    for (int i = 0; i < 3; i++) {
        state = SendMessage(CheckBox[i], BM_GETCHECK, 0, 0);
        DestroyWindow(CheckBox[i]); //�������� CheckBox, ����� ������ ���������� � ������ �����
        CheckBox[i] = CreateWindowEx(NULL, L"BUTTON", CheckBoxNames[i], WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            rectClient.right - 170, 100 + i * 30, 110, 30, hGraphPage, (HMENU)CheckBoxIDs[i], NULL, NULL);
        if (state == BST_CHECKED) SendMessage(CheckBox[i], BM_SETCHECK, BST_CHECKED, 0);
    }

    // ������������ ������
    DeleteObject(hFont);

}