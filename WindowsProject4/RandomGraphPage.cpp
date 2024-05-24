#include "RandomGraphPage.h"

LRESULT CALLBACK RandomGraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT state;
	int length; //переменная для длины текстового поля

	RECT rectClient; //перменная области окна
	GetClientRect(hRandomGraphPage, &rectClient); //получаем область окна

	switch (message)
	{
	case WM_CREATE:
		{
			generateButtonRand = CreateWindow(L"Button", L"Сгенерировать случайный процесс",
			                                  WS_VISIBLE | WS_CHILD | WS_BORDER, 0,
			                                  0, 400, 25, hWnd,
			                                  (HMENU)ID_GENERATE_BUTTON,
			                                  (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                                  nullptr);
		}
		break;
	case WM_SIZE:
		{
			RECT rcClient;
			GetClientRect(hRandomGraphPage, &rcClient);

			SetWindowPos(generateButtonRand, NULL, rcClient.right / 2 - 200, rcClient.bottom - 25, 0, 0, SWP_NOSIZE);
			//SetWindowPos(downButtonFreq, NULL, rcClient.right - 25, rcClient.bottom - 25, 20, 20, SWP_NOZORDER);

			break;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hRandomGraphPage, &ps);

			//if (flagDrawRand) DrawRandom(hdc, ps.rcPaint);
			DrawRandomGraph(hdc, ps.rcPaint);

			EndPaint(hRandomGraphPage, &ps);
			break;
		}
		break;
	case WM_COMMAND:
		{
			HWND hStaticText;
			RECT rcClient;

			switch (LOWORD(wParam))
			{
			case ID_GENERATE_BUTTON: //обработка нажатия на кнопку добавления
				flagDrawRand = true;
				getRandomData();
				InvalidateRect(hRandomGraphPage, NULL, TRUE);
			}
			break;
		}
	}
	if (g_pRandomGraphPageProc)
		return CallWindowProc(g_pRandomGraphPageProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

void DrawRandomGraph(HDC hdc, RECT rectClient)
{
	RECT rectText;
	TCHAR str1[20];
	int color[3] = {255, 128, 0};
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон

	//инициализация переменных
	double min, max;
	int height, width;
	HPEN hpen;
	height = rectClient.bottom - rectClient.top - 50; //высота области рисования графика
	width = rectClient.right - rectClient.left - 50; //ширина области рисования графика

	int interval = (height - 40) / 20; //расстояние между горизонтальными линиями гистограммы

	//рисуем оси
	hpen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100)); // перо толщиной в 2 пикселя
	SelectObject(hdc, hpen);

	int horizontalAxis = (int)((rectClient.bottom - 10) / 2); //координата горизонтальной оси

	line(hdc, 50, horizontalAxis, width, horizontalAxis); // горизонтальная ось гистограммы
	//стрелка на конце
	line(hdc, width - 10, horizontalAxis - 5, width, horizontalAxis);
	line(hdc, width - 10, horizontalAxis + 5, width, horizontalAxis);

	HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                         OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                         DEFAULT_PITCH | FF_SWISS, L"Arial");
	SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет текста
	SelectObject(hdc, hFont); //выбираем шрифт
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон
	rectText = {width, horizontalAxis - 25, width + 50, horizontalAxis - 10};
	DrawText(hdc, L"x", -1, &rectText, DT_WORDBREAK | DT_LEFT); //рисуем текст

	line(hdc, 50, rectClient.bottom - 30, 50, rectClient.top + 10); // вертикальная ось гистограммы
	//стрелка на конце
	line(hdc, 50 - 5, rectClient.top + 10 + 10, 50, rectClient.top + 10);
	line(hdc, 50 + 5, rectClient.top + 10 + 10, 50, rectClient.top + 10);
	DeleteObject(hpen);

	DeleteObject(hFont);
	hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                   DEFAULT_PITCH | FF_SWISS, L"Arial");
	SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет текста
	SelectObject(hdc, hFont); //выбираем шрифт
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон
	rectText = {50 + 10, rectClient.top + 5, 50 + 60, rectClient.top + 10 + 30};
	DrawText(hdc, L"f(x)", -1, &rectText, DT_WORDBREAK | DT_LEFT); //рисуем текст

	//насечки на горизонтальной оси
	for (int i = 1; i < 21; i++)
		line(hdc, 50 + i * (width - 70) / 20, horizontalAxis - 3, 50 + i * (width - 70) / 20, horizontalAxis + 3);

	DeleteObject(hFont);
	hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                   DEFAULT_PITCH | FF_SWISS, L"Arial");
	SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет текста
	SelectObject(hdc, hFont); //выбираем шрифт
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон
	//TCHAR str1[20];
	for (int i = 1; i < 21; i++)
	{
		rectText = {
			50 + i * (width - 70) / 20 - 10, horizontalAxis + 5, 50 + i * (width - 70) / 20 + 10,
			horizontalAxis + 30
		};
		swprintf_s(str1, L"%d", i);
		DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_CENTER); //рисуем текст
	}


	//насечки на вертикальной оси и числовая шкала

	for (int i = 1; i < 11; i++)
	{
		line(hdc, 50 - 3, horizontalAxis - i * interval, 50 + 3, horizontalAxis - i * interval);
		rectText = {0, horizontalAxis - i * interval - 10, 40, horizontalAxis - i * interval + 10};
		swprintf_s(str1, L"%d", i);
		DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст

		line(hdc, 50 - 3, horizontalAxis + i * interval, 50 + 3, horizontalAxis + i * interval);
		rectText = {0, horizontalAxis + i * interval - 10, 40, horizontalAxis + i * interval + 10};
		swprintf_s(str1, L"%d", -i);
		DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст
	}

	//число 0
	rectText = {10, horizontalAxis - 10, 40, horizontalAxis + 10};
	swprintf_s(str1, L"%d", 0); //целое число
	DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст
	DeleteObject(hFont);
	DeleteObject(hpen);

	int top = 0;
	int maxHeight = 0;
	HBRUSH hBrush = CreateSolidBrush(RGB(color[0], color[1], color[2]));
	SelectObject(hdc, hBrush);
	hpen = CreatePen(PS_SOLID, 3, RGB(color[0], color[1], color[2])); // перо толщиной в 2 пикселя
	SelectObject(hdc, hpen);

	if (flagDrawRand)
	{
		/*getRandomData();*/

		for (int i = 0; i < 21; i++)
		{
			if (i != 0)
				line(hdc, 50 + (i - 1) * (width - 70) / 20, top, 50 + i * (width - 70) / 20,
				     horizontalAxis - (maxHeight * randData[i] / 10));
			maxHeight = 10 * interval;
			top = horizontalAxis - (maxHeight * randData[i] / 10);
			//top = horizontalAxis - (maxHeight * 7 / 10);
			Ellipse(hdc, 50 + i * (width - 70) / 20 - 4, top - 4, 50 + i * (width - 70) / 20 + 4, top + 4);
		}
	}


	//for (int i = 0; i < numRandColumns; i++)
	//{
	//	if (randData[i] >= 0)
	//	{
	//		maxHeight = (maxRandLevel > 0 ? 5 : addLevel) * interval;
	//		top = horizontalAxis - (maxHeight * randData[i] / maxPositive);
	//	}
	//	else
	//	{
	//		maxHeight = (maxRandLevel < 0 ? -5 : addLevel) * interval;
	//		top = horizontalAxis - (maxHeight * randData[i] / minNigative);
	//	}

	//	line(hdc, 50 + i * (width - 70) / 20, horizontalAxis - 0, 50 + i * (width - 70) / 20, top);
	//	Ellipse(hdc, 50 + i * (width - 70) / 20 - 4, top - 4, 50 + i * (width - 70) / 20 + 4, top + 4);
	//}
	//DeleteObject(hFont);
	//hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	//	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	//	DEFAULT_PITCH | FF_SWISS, L"Arial");
	//SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет текста
	//SelectObject(hdc, hFont); //выбираем шрифт
	//SetBkMode(hdc, TRANSPARENT); //прозрачный фон
	////числа на горизонтальной шкале
	//for (int i = 1; i < 21; i++)
	//{
	//	rectText = { 50 + i * (width - 70) / 20 - 10, horizontalAxis + ((randData[i] > 0 || i > numRandColumns) ? 5 : -23), 50 + i * (width - 70) / 20 + 10, horizontalAxis + ((randData[i] || i > numRandColumns) > 0 ? 30 : -5) };
	//	swprintf_s(str1, L"%d", i); //дробное число
	//	DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_CENTER); //рисуем текст
	//}
}

void getRandomData()
{
	int min = -10;
	int max = 10;
	for (int i = 0; i < 21; i++)
		randData[i] = min + rand() % (max - min + 1);
}
