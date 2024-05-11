#include "GraphPage.h"

LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT state;
    RECT rectClient;
    GetClientRect(hGraphPage, &rectClient);

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hGraphPage, &ps);
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
            InvalidateRect(hGraphPage, NULL, TRUE);
            break;
        case ID_CHECKBOX_2:
            // ��������� ������� �� ������� 2
            OutputDebugString(L"ID_CHECKBOX_2!\n");
            InvalidateRect(hGraphPage, NULL, TRUE);
            break;
        case ID_CHECKBOX_3:
            // ��������� ������� �� ������� 3
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
    hpen = CreatePen(PS_SOLID, 2, 0); // ������ ���� �������� � 3 �������
    SelectObject(hdc, hpen);
    line(hdc, 10, OffsetY, width - 10, OffsetY); // ��������� �������������� ��� line(hdc, width - 10, OffsetY, width - 15, OffsetY - 5); // ������� �������������� �������� line(hdc, width - 10, OffsetY, width - 15, OffsetY + 5); // ������ �������������� ��������
    
    //��������� ���������
    line(hdc, width - 20, OffsetY - 5, width - 10, OffsetY);
    line(hdc, width - 20, OffsetY + 5, width - 10, OffsetY);
    
    for (int i = 1; i < 9; i++) line(hdc, i * 0.118 * width, OffsetY, i * 0.118 * width, OffsetY - 3);
    // ������� �� �������������� ���
    line(hdc, OffsetX + 10, 10, OffsetX + 10, height - 5); // ��������� ������������ ��� line(hdc, OffsetX + 10, 0, OffsetX + 15, 10); // ��������� ������� ������������� �������� line(hdc, OffsetX + 10, 0, OffsetX + 5, 10); // ��������� ������ ������������� ��������
  
    //��������� ���������
    line(hdc, OffsetX + 5, 20, OffsetX + 10, 10);
    line(hdc, OffsetX + 15, 20, OffsetX + 10, 10);
    
    for (int i = -2; i < 3; i++) line(hdc, OffsetX + 10, OffsetY + height / 6 * i, OffsetX + 15, OffsetY + height / 6 * i);
    // ������� �� ������������ ���
    DeleteObject(hpen); // �������� ������� ����

                // ��������� ������� �������
    int color = 0xFF0000; // ����� ���� ��� ������� ���� ������
    //TextOut(hdc, 21, 7, _T("Y = sin (X)"), 11); // ����� ������ y=sin(x) 8 ��������
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
    RECT rectText = { rectClient.right - 180, 20, rectClient.right, rectClient.bottom };
    DrawText(hdc, L"�������� �������,\n������� �� ������ �������:", -1, &rectText, DT_WORDBREAK);

    for (int i = 0; i < 3; i++) {
        state = SendMessage(CheckBox[i], BM_GETCHECK, 0, 0);
        DestroyWindow(CheckBox[i]); //�������� CheckBox, ����� ������ ���������� � ������ �����
        CheckBox[i] = CreateWindowEx(NULL, L"BUTTON", CheckBoxNames[i], WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            rectClient.right - 170, 100 + i * 30, 110, 30, hGraphPage, (HMENU)CheckBoxIDs[i], NULL, NULL);
        if (state == BST_CHECKED) SendMessage(CheckBox[i], BM_SETCHECK, BST_CHECKED, 0);
    }

    DeleteObject(hFont); // ������������ ������
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
        // 3 ������� (4-1)
        double x = (double)i * 0.099;
        f[0][i] = x;
        f[3][i] = sin(x);
        f[1][i] = cos(x);
        f[2][i] = 3.14159265358979 * x / 10 - 1.0;
    }
    return f;
}
