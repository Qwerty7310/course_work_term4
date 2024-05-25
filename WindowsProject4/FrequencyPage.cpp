#include "FrequencyPage.h"

LRESULT CALLBACK FrequencyPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT state;
	int length; //переменная для длины текстового поля

	RECT rectClient; //перменная области окна
	GetClientRect(hFrequencyPage, &rectClient); //получаем область окна

	switch (message)
	{
	case WM_CREATE:
		{
			// Создаем дочерние окна для каждой вкладки
			hChildFreqPage = CreateWindow(L"ChildFreqPage", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 0,
			                              0, hWnd, NULL, hInst, NULL);
		}
		break;
	case WM_SIZE:
		{
			OutputDebugString(L"QWERTY\n");
			ShowWindow(hChildFreqPage, SW_SHOW);
			// Получаем новые размеры окна
			int newWidth = LOWORD(lParam);
			int newHeight = HIWORD(lParam);
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			SetWindowPos(hChildFreqPage, NULL, rcClient.right - 220, 20, 220,
			             min(rcClient.bottom - 20, 40 + (numFreqTextBox + 1) * 30 + 40), SWP_NOZORDER);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hFrequencyPage, &ps);

			FillRect(hdc, &rectClient, (HBRUSH)(COLOR_WINDOW));

			if (flagDrawFreq) DrawFrequency(hdc, ps.rcPaint);
			// рисуем гистограммы, если были введены корректные значения
			else
			{
				//иначе выводим информационное сообщение
				HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
				                         OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				                         DEFAULT_PITCH | FF_SWISS, L"Arial");
				SelectObject(hdc, hFont); //выбираем шрифт
				RECT rectText;
				rectText.left = rectClient.left; //верхний левый угол
				rectText.top = rectClient.top + (rectClient.bottom - rectClient.top + 50) / 6;
				rectText.right = rectClient.right - rectClient.left - 270; //нижний правый угол
				rectText.bottom = rectClient.top + (rectClient.bottom - rectClient.top + 50) * 5 / 6;

				DrawText(hdc, L"Введите Ваши данные для\n построения частотной характеристики.", -1, &rectText,
				         DT_WORDBREAK | DT_CENTER);
				DeleteObject(hFont);
			}
			EndPaint(hFrequencyPage, &ps);
			break;
		}
		break;
	}
	if (g_pFrequencyPageProc)
		return CallWindowProc(g_pFrequencyPageProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

void DrawFrequency(HDC hdc, RECT rectClient)
{
	RECT rectText;
	int color[3] = {255, 128, 0};
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон

	//инициализация переменных
	double min, max;
	int height, width;
	HPEN hpen;
	height = rectClient.bottom - rectClient.top - 50; //высота области рисования графика
	width = rectClient.right - rectClient.left - 250; //ширина области рисования графика

	int addLevel = 0;
	if (maxFreqLevel > 0)
	{
		addLevel = (int)(abs(minNig) / (maxFreqLevel / 5));
		if ((int)(abs(minNig) / (maxFreqLevel / 5)) * 1. != abs(minNig) / (maxFreqLevel / 5))
			addLevel += 1;
	}
	else
	{
		addLevel = (int)(maxPos / (-maxFreqLevel / 5));
		if ((int)(maxPos / (-maxFreqLevel / 5)) * 1. != maxPos / (-maxFreqLevel / 5))
			addLevel += 1;
	}

	int interval = (height - 40) / (5 + addLevel); //расстояние между горизонтальными линиями гистограммы

	//рисуем оси
	hpen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100)); // перо толщиной в 2 пикселя
	SelectObject(hdc, hpen);

	int horizontalAxis = 0; //координата горизонтальной оси

	if (maxFreqLevel > 0)
		horizontalAxis = rectClient.bottom - 40 - addLevel * interval;
	else
		horizontalAxis = rectClient.bottom - 30 - 5 * interval;

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
	rectText = { width, horizontalAxis - 25, width + 50, horizontalAxis - 10 };
	DrawText(hdc, L"k", -1, &rectText, DT_WORDBREAK | DT_LEFT); //рисуем текст

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
	rectText = { 50 + 10, rectClient.top + 5, 50 + 60, rectClient.top + 10 + 30 };
	DrawText(hdc, L"F(k)", -1, &rectText, DT_WORDBREAK | DT_LEFT); //рисуем текст

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
	
	TCHAR str1[20];

	//насечки на вертикальной оси и числовая шкала
	if (maxFreqLevel > 0)
	{
		for (int i = 1; i < 6; i++)
		{
			line(hdc, 50 - 3, horizontalAxis - i * interval, 50 + 3, horizontalAxis - i * interval);
			rectText = {0, horizontalAxis - i * interval - 10, 40, horizontalAxis - i * interval + 10};
			if ((i * maxFreqLevel / 5) == (int)(i * maxFreqLevel / 5))
				swprintf_s(str1, L"%d", (int)(i * maxFreqLevel / 5)); //целое число
			else
				swprintf_s(str1, L"%.2lf", (i * maxFreqLevel / 5)); //дробное число
			DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст
		}
		for (int i = 1; i < addLevel + 1; i++)
		{
			line(hdc, 50 - 3, horizontalAxis + i * interval, 50 + 3, horizontalAxis + i * interval);
			TCHAR str1[20];
			rectText = {0, horizontalAxis + i * interval - 10, 40, horizontalAxis + i * interval + 10};
			if ((i * maxFreqLevel / 5) == (int)(i * maxFreqLevel / 5))
				swprintf_s(str1, L"%d", -(int)(i * maxFreqLevel / 5)); //целое число
			else
				swprintf_s(str1, L"%.2lf", -(i * maxFreqLevel / 5)); //дробное число
			DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст
		}
	}
	else
	{
		for (int i = 1; i < 6; i++)
		{
			line(hdc, 50 - 3, horizontalAxis + i * interval, 50 + 3, horizontalAxis + i * interval);

			rectText = {0, horizontalAxis + i * interval - 10, 45, horizontalAxis + i * interval + 10};
			if ((i * maxFreqLevel / 5) == (int)(i * maxFreqLevel / 5))
				swprintf_s(str1, L"%d", (int)(i * maxFreqLevel / 5)); //целое число
			else
				swprintf_s(str1, L"%.2lf", (i * maxFreqLevel / 5)); //дробное число
			DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст
		}
		for (int i = 1; i < addLevel + 1; i++)
		{
			line(hdc, 50 - 3, horizontalAxis - i * interval, 50 + 3, horizontalAxis - i * interval);
			TCHAR str1[20];
			rectText = {0, horizontalAxis - i * interval - 10, 45, horizontalAxis - i * interval + 10};
			if ((i * maxFreqLevel / 5) == (int)(i * maxFreqLevel / 5))
				swprintf_s(str1, L"%d", -(int)(i * maxFreqLevel / 5)); //целое число
			else
				swprintf_s(str1, L"%.2lf", -(i * maxFreqLevel / 5)); //дробное число
			DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст
		}
	}
	//число 0
	rectText = {10, horizontalAxis - 10, 40, horizontalAxis + 10};
	swprintf_s(str1, L"%d", 0); //целое число
	DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст
	DeleteObject(hFont);
	DeleteObject(hpen);


	int maxPositive;
	int minNigative;
	if (maxFreqLevel > 0)
	{
		maxPositive = maxFreqLevel;
		minNigative = (int)(maxFreqLevel / 5 * addLevel);
	}
	else
	{
		maxPositive = (int)(-maxFreqLevel / 5 * addLevel);
		minNigative = maxFreqLevel;
	}

	int top = 0;
	int maxHeight = 0;
	HBRUSH hBrush = CreateSolidBrush(RGB(color[0], color[1], color[2]));
	SelectObject(hdc, hBrush);
	hpen = CreatePen(PS_SOLID, 3, RGB(color[0], color[1], color[2])); // перо толщиной в 2 пикселя
	SelectObject(hdc, hpen);
	for (int i = 0; i < numFreqColumns; i++)
	{
		if (freqData[i] >= 0)
		{
			maxHeight = (maxFreqLevel > 0 ? 5 : addLevel) * interval;
			top = horizontalAxis - (maxHeight * freqData[i] / maxPositive);
		}
		else
		{
			maxHeight = (maxFreqLevel < 0 ? -5 : addLevel) * interval;
			top = horizontalAxis - (maxHeight * freqData[i] / minNigative);
		}

		line(hdc, 50 + i * (width - 70) / 20, horizontalAxis - 0, 50 + i * (width - 70) / 20, top);
		Ellipse(hdc, 50 + i * (width - 70) / 20 - 3, top - 3, 50 + i * (width - 70) / 20 + 3, top + 3);
	}
	DeleteObject(hFont);
	hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"Arial");
	SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет текста
	SelectObject(hdc, hFont); //выбираем шрифт
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон
	//числа на горизонтальной шкале
	for (int i = 1; i < 21; i++)
	{
		rectText = { 50 + i * (width - 70) / 20 - 10, horizontalAxis + ((freqData[i] >= 0 || i >= numFreqColumns) ? 5 : -23), 50 + i * (width - 70) / 20 + 10, horizontalAxis + ((freqData[i] >= 0 || i >= numFreqColumns) ? 30 : -5) };
		swprintf_s(str1, L"%d", i); //дробное число
		DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_CENTER); //рисуем текст
	}
}

int getFrequencyData()
{
	wchar_t message[1024] = L""; //инициализируем переменную для записи сообщения об ошибке

	bool flagError = false; //флаг наличия ошибки в введенных данных
	int length; //длина текста в TextBox
	double* tempArr = new double[numFreqTextBox]; //временный массив для хранения данных

	for (int i = 0; i < numFreqTextBox; i++)
	{
		length = GetWindowTextLength(freqTextBox[i][1]); //получаем длину
		TCHAR* buffer1 = new TCHAR[length + 1]; //создаем буфер для хранения строки
		GetWindowText(freqTextBox[i][1], buffer1, length + 1); // получаем текст из текстового поля

		//проверяем корректность введенных данных
		if ((containsLetters(buffer1) == true) || (wcslen(buffer1) == 0))
		//если есть неразрешенные символы или длина текста равно 0
		{
			flagError = true; //флаг ошибки

			//выбор соответствующего сообщения
			const wchar_t* str;
			if (containsLetters(buffer1) == true)
				str = L"Значение некорректно:\tстрока ";
			else
				str = L"Значение отсутствует:\tстрока ";

			wchar_t numStr[10]; //буфер для числа
			//добавления текста в сообщение для пользователя
			swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
			wcscat_s(message, 1024, str);
			wcscat_s(message, 1024, numStr);
			wcscat_s(message, 1024, L"\n");
		}
		else
		{
			tempArr[i] = _wtof(buffer1);
		}
		delete[] buffer1; //удаяем буфер
	}
	if (flagError)
	{
		//если данные некорректны
		MessageBox(NULL, message, L"Ошибка", MB_ICONEXCLAMATION | MB_OK); //выводим сообщение об ошибке
		delete[] tempArr; //удаляем временный массив
		return 1; //возвращаем 1 (значение наличия ошибки)
	}
	else
	{
		//данные корректны
		//создаем новый массив данных
		delete[] freqData; //удаляем старый
		freqData = new double[numFreqTextBox]; //создаем новый
		for (int i = 0; i < numFreqTextBox; i++)
		{
			freqData[i] = tempArr[i]; //записываем данные в массив
		}
		delete[] tempArr; //очищаем память под временный массив

		//находим максимальное значение
		maxPos = 0; //инициализируем переменную для поиска максимума
		minNig = 0; //инициализируем переменную для поиска максимума
		for (int i = 0; i < numFreqTextBox; i++)
		{
			if (freqData[i] > maxPos) maxPos = freqData[i];
			if (freqData[i] < minNig) minNig = freqData[i];
		}
		int max = max(maxPos, -minNig);
		if (max == 0)
		{
			//если все числа равны 0
			MessageBox(NULL, L"Должно быть хотя бы 1 число, отличное от нуля!", L"Ошибка", MB_ICONEXCLAMATION | MB_OK);
			//сообщение об ошибке
			return 1; //возвращаем 1 (значение наличия ошибки)
		}
		else
		{
			//находим максимальное значение шкалы гистограммы
			maxFreqLevel = 0; //максимальное значение шкалы гистограммы

			int order = 0;
			//порядок числа ((количество цифр для чисел > 1) или (количество 0 после запятой со знаком минус + 1))
			double tempDouble = max; //переменная для поиска порядка числа
			int tempInt;
			int multiplier = 0; //множитель (для случая, если max == 1)

			if (tempDouble >= 1)
				while ((int)tempDouble > 0)
				{
					order += 1;
					tempDouble /= 10;
				}
			else
				while ((int)tempDouble < 1)
				{
					order -= 1;
					tempDouble *= 10;
				}
			if (order > 0)
				tempInt = (int)(max / pow(10, order - 2)) * pow(10, order - 2); //очищаем (
			else
				tempInt = (int)(max * pow(10, -order)); //делаем число больше 1

			//если max == 1, увеличиваем на 10
			if (order == 1)
			{
				order = 2;
				tempInt *= 10;
				multiplier = 1;
			}

			//находим максимальное значение шкалы гистограммы
			for (int i = 0; i <= 10; i++)
			{
				if (
					((tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0)) % (5 * (int)pow(
						10, (order > 2) ? (order - 3) : 0)) == 0) &&
					//проверяем, делится ли наше число на 5 * 10 в соответствующей степени для 2-х случаев
					((tempInt + i * (int)pow(10, (order > 1) ? (order - 2) : 0)) % ((order > 1) ? 1 : 5) == 0) &&
					(tempInt + i * pow(10, (order > 1) ? (order - 2) : 0) >= max * pow(10, multiplier)) &&
					//проверяем, что число больше max для 2-х случаев
					((order > 0) || (order < 0) && (tempInt + i * pow(10, (order > 1) ? (order - 2) : 0) >= max * pow(
						10, -order)))
				)
				{
					//учитываем изменения для tempInt перед началом поиска
					maxFreqLevel = tempInt + i * pow(10, (order > 1) ? (order - 2) : 0);
					break;
				}
			}
			//учитываем изменения для tempInt перед началом поиска
			if (order < 0) maxFreqLevel *= pow(10, order);
			if (multiplier) maxFreqLevel /= 10;

			if (abs(minNig) > maxPos) maxFreqLevel *= -1;

			return 0;
		}
	}
	return 0;
}


LRESULT CALLBACK ChildFreqPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT state;
	int length; // переменная для длины текстового поля


	RECT rectClient; // переменная области окна
	GetClientRect(hChildFreqPage, &rectClient); // получаем область окна

	int xStart = 30;
	switch (message)
	{
	case WM_CREATE:
		{
			HDC hdc = GetDC(hChildFreqPage);
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
			HWND hStaticText = CreateWindowEx(0, L"STATIC", L"    k              F(k)",
			                                  WS_CHILD | WS_VISIBLE | SS_LEFT, xStart, 10, 240, 20,
			                                  hWnd, NULL, NULL, NULL);
			SendMessage(hStaticText, WM_SETFONT, (WPARAM)hFont, TRUE);
			// Сохранение дескриптора текста в пользовательских данных окна
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)hStaticText);
			for (int i = 0; i <= MAX_FREQUENCY_TEXTBOX; i++)
			{
				wchar_t numStr[10]; //буфер для числа
				swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i);
				freqTextBox[i][0] = CreateWindowEx(NULL, L"Edit", numStr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
				                                   xStart, 40 + i * 30, 50, 20,
				                                   hWnd, nullptr,
				                                   (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
				EnableWindow(freqTextBox[i][0], FALSE);
				freqTextBox[i][1] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
				                                   xStart + 60, 40 + i * 30, 100, 20,
				                                   hWnd, nullptr,
				                                   (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
				if (i < numFreqTextBox)
				{
					ShowWindow(freqTextBox[i][0], SW_SHOW);
					ShowWindow(freqTextBox[i][1], SW_SHOW);
				}
				else
				{
					ShowWindow(freqTextBox[i][0], SW_HIDE);
					ShowWindow(freqTextBox[i][1], SW_HIDE);
				}
			}
			addButtonFreq = CreateWindow(L"Button", L"Добавить", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart,
			                             40 + numFreqTextBox * 30, 75, 25, hWnd,
			                             (HMENU)ID_ADD_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                             nullptr);
			deleteButtonFreq = CreateWindow(L"Button", L"Убрать", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart + 85,
			                                40 + numFreqTextBox * 30, 75, 25, hWnd,
			                                (HMENU)ID_DELETE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                                nullptr);
			createButtonFreq = CreateWindow(L"Button", L"Построить", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart,
			                                40 + (numFreqTextBox + 1) * 30, 160, 25, hWnd,
			                                (HMENU)ID_CREATE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                                nullptr);
			upButtonFreq = CreateWindow(L"Button", L"∧", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart,
			                            0, 0, 0, hWnd,
			                            (HMENU)ID_UP_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                            nullptr);
			downButtonFreq = CreateWindow(L"Button", L"∨", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart,
			                              0, 0, 0, hWnd,
			                              (HMENU)ID_DOWN_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                              nullptr);

			EnableWindow(upButtonFreq, FALSE);
			EnableWindow(downButtonFreq, FALSE);
		}
		break;
	case WM_SIZE:
		{
			// Получаем новые размеры окна
			RECT rcClient;
			GetClientRect(hChildFreqPage, &rcClient);
			curFreqChildWidth = rcClient.bottom;
			SetWindowPos(upButtonFreq, NULL, rcClient.right - 25, rcClient.bottom - 55, 20, 20, SWP_NOZORDER);
			SetWindowPos(downButtonFreq, NULL, rcClient.right - 25, rcClient.bottom - 25, 20, 20, SWP_NOZORDER);

			if (curFreqChildWidth < 40 + (numFreqTextBox + 1) * 30 + 25)
				EnableWindow(downButtonFreq, TRUE);
			else
				EnableWindow(downButtonFreq, FALSE);

			if (rcClient.bottom >= 40 + (numFreqTextBox + 1) * 30 + 25 + (freqChildScroll + 1) * scrollStep)
				FreqChildScrollUp(hWnd);

			if (!IsWindowEnabled(upButtonFreq) && !IsWindowEnabled(downButtonFreq))
			{
				ShowWindow(upButtonFreq, SW_HIDE);
				ShowWindow(downButtonFreq, SW_HIDE);
			}
			else
			{
				ShowWindow(upButtonFreq, SW_SHOW);
				ShowWindow(downButtonFreq, SW_SHOW);
			}

			break;
		}
	case WM_PAINT:
		{
			OutputDebugString(L"PAINT CHILD\n");
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hChildFreqPage, &ps);
			FillRect(hdc, &rectClient, (HBRUSH)(COLOR_WINDOW));
			EndPaint(hChildFreqPage, &ps);
		}
		break;
	case WM_COMMAND:
		{
			HWND hStaticText;
			RECT rcClient;

			switch (LOWORD(wParam))
			{
			case ID_ADD_BUTTON: //обработка нажатия на кнопку добавления
				numFreqTextBox += 1;
				GetClientRect(hFrequencyPage, &rcClient);
				SetWindowPos(hChildFreqPage, NULL, rcClient.right - 220, 20, 220,
				             min(rcClient.bottom - 20, 40 + (numFreqTextBox + 1) * 30 + 40), SWP_NOZORDER);

				EnableWindow(deleteButtonFreq, TRUE);

				SetWindowPos(createButtonFreq, NULL, xStart,
				             40 + (numFreqTextBox + 1) * 30 + freqChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				SetWindowPos(addButtonFreq, NULL, xStart,
				             40 + numFreqTextBox * 30 + freqChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				SetWindowPos(deleteButtonFreq, NULL, xStart + 85,
				             40 + numFreqTextBox * 30 + freqChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				ShowWindow(freqTextBox[numFreqTextBox - 1][0], SW_SHOW);
				ShowWindow(freqTextBox[numFreqTextBox - 1][1], SW_SHOW);

				if (numFreqTextBox == 21)
					EnableWindow(addButtonFreq, FALSE);
				if (curFreqChildWidth < 40 + (numFreqTextBox + 1) * 30 + 25)
				{
					FreqChildScrollDown(hWnd);
					EnableWindow(upButtonFreq, TRUE);
				}
				if (!IsWindowEnabled(upButtonFreq) && !IsWindowEnabled(downButtonFreq))
				{
					ShowWindow(upButtonFreq, SW_HIDE);
					ShowWindow(downButtonFreq, SW_HIDE);
				}
				else
				{
					ShowWindow(upButtonFreq, SW_SHOW);
					ShowWindow(downButtonFreq, SW_SHOW);
				}

				break;
			case ID_DELETE_BUTTON: //обработка нажатия на кнопку удаления
				EnableWindow(addButtonFreq, TRUE);
				numFreqTextBox -= 1;
				GetClientRect(hFrequencyPage, &rcClient);
				SetWindowPos(hChildFreqPage, NULL, rcClient.right - 220, 20, 220,
				             min(rcClient.bottom - 20, 40 + (numFreqTextBox + 1) * 30 + 40), SWP_NOZORDER);

				ShowWindow(freqTextBox[numFreqTextBox][0], SW_HIDE);
				ShowWindow(freqTextBox[numFreqTextBox][1], SW_HIDE);
				SetWindowPos(addButtonFreq, NULL, xStart,
				             40 + numFreqTextBox * 30 + freqChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				SetWindowPos(deleteButtonFreq, NULL, xStart + 85,
				             40 + numFreqTextBox * 30 + freqChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				SetWindowPos(createButtonFreq, NULL, xStart,
				             40 + (numFreqTextBox + 1) * 30 + freqChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				if (numFreqTextBox == 2)
					EnableWindow(deleteButtonFreq, FALSE);

				if (curFreqChildWidth >= 40 + (numFreqTextBox + 1) * 30 + 25)
				{
					EnableWindow(upButtonFreq, FALSE);
					EnableWindow(downButtonFreq, FALSE);
				}
				if (10 + freqChildScroll * scrollStep < 0)
					FreqChildScrollUp(hWnd);
				if (!IsWindowEnabled(upButtonFreq) && !IsWindowEnabled(downButtonFreq))
				{
					ShowWindow(upButtonFreq, SW_HIDE);
					ShowWindow(downButtonFreq, SW_HIDE);
				}
				else
				{
					ShowWindow(upButtonFreq, SW_SHOW);
					ShowWindow(downButtonFreq, SW_SHOW);
				}

				break;
			case ID_CREATE_BUTTON: //обработка нажатия на кнопку построения гистограммы
				if (!getFrequencyData()) //проверка на корректность данных и получени данных для построения
				{
					//данные корректны
					for (int i = 0; i < numFreqTextBox; i++)
						for (int j = 0; j < 2; j++)
						{
							length = GetWindowTextLength(freqTextBox[i][j]); // получаем длину текста из текстового поля
							freqText[i][j] = new TCHAR[length + 1]; //создаем строку символов для записи
							GetWindowText(freqTextBox[i][j], freqText[i][j], length + 1); //записываем текст
						}
					numFreqColumns = numFreqTextBox; //изменяем число столбцов диаграммы
					flagDrawFreq = true; //разрешаем рисовать гистограмму
					InvalidateRect(hFrequencyPage, NULL, TRUE); //команда на перерисовку окна
				}
				break;
			case ID_DOWN_BUTTON: //обработка нажатия на кнопку построения гистограммы
				FreqChildScrollDown(hWnd);
				break;
			case ID_UP_BUTTON: //обработка нажатия на кнопку построения гистограммы
				FreqChildScrollUp(hWnd);
				break;
			}
		}
		break;
	}

	if (g_pChildFreqPageProc)
		return CallWindowProc(g_pChildFreqPageProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

void FreqChildScrollUp(HWND hWnd)
{
	HWND hStaticText;
	RECT rcClient;
	int xStart = 30;
	freqChildScroll += 1;

	SetWindowPos(createButtonFreq, NULL, xStart,
	             40 + (numFreqTextBox + 1) * 30 + freqChildScroll * scrollStep,
	             210, 25, SWP_NOSIZE);
	SetWindowPos(addButtonFreq, NULL, xStart, 40 + numFreqTextBox * 30 + freqChildScroll * scrollStep,
	             100, 25,
	             SWP_NOSIZE);
	SetWindowPos(deleteButtonFreq, NULL, xStart + 85, 40 + numFreqTextBox * 30 + freqChildScroll * scrollStep,
	             100, 25,
	             SWP_NOSIZE);
	for (int i = MAX_FREQUENCY_TEXTBOX; i >= 0; i--)
	{
		SetWindowPos(freqTextBox[i][0], NULL, xStart, 40 + i * 30 + freqChildScroll * scrollStep, 60, 20,
		             SWP_NOSIZE);
		SetWindowPos(freqTextBox[i][1], NULL, xStart + 60, 40 + i * 30 + freqChildScroll * scrollStep, 160,
		             20,
		             SWP_NOSIZE);
	}
	GetClientRect(hChildFreqPage, &rcClient);
	curFreqChildWidth = rcClient.bottom;
	hStaticText = (HWND)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	SetWindowPos(hStaticText, NULL, xStart, 10 + freqChildScroll * scrollStep, 0, 0, SWP_NOSIZE);

	if (10 + freqChildScroll * scrollStep > 0)
	{
		EnableWindow(upButtonFreq, FALSE);
	}
	if (curFreqChildWidth < 40 + (numFreqTextBox + 1) * 30 + 25 + freqChildScroll * scrollStep)
	{
		EnableWindow(downButtonFreq, TRUE);
	}
}

void FreqChildScrollDown(HWND hWnd)
{
	int xStart = 30;
	RECT rcClient;
	HWND hStaticText;

	freqChildScroll -= 1;

	GetClientRect(hChildFreqPage, &rcClient);
	curFreqChildWidth = rcClient.bottom;
	hStaticText = (HWND)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	SetWindowPos(hStaticText, NULL, 0, 10 + freqChildScroll * scrollStep, 0, 0, SWP_NOSIZE);

	for (int i = 0; i < MAX_FREQUENCY_TEXTBOX; i++)
	{
		SetWindowPos(freqTextBox[i][0], NULL, xStart, 40 + i * 30 + freqChildScroll * scrollStep, 60, 20,
		             SWP_NOSIZE);
		SetWindowPos(freqTextBox[i][1], NULL, xStart + 60, 40 + i * 30 + freqChildScroll * scrollStep, 160,
		             20,
		             SWP_NOSIZE);
	}
	SetWindowPos(addButtonFreq, NULL, xStart, 40 + numFreqTextBox * 30 + freqChildScroll * scrollStep,
	             100, 25,
	             SWP_NOSIZE);
	SetWindowPos(deleteButtonFreq, NULL, xStart + 85, 40 + numFreqTextBox * 30 + freqChildScroll * scrollStep,
	             100, 25,
	             SWP_NOSIZE);
	SetWindowPos(createButtonFreq, NULL, xStart,
	             40 + (numFreqTextBox + 1) * 30 + freqChildScroll * scrollStep,
	             210, 25, SWP_NOSIZE);

	if (curFreqChildWidth > 40 + (numFreqTextBox + 1) * 30 + 25 + freqChildScroll * scrollStep)
	{
		EnableWindow(upButtonFreq, TRUE);
		EnableWindow(downButtonFreq, FALSE);
	}
	if (10 + freqChildScroll * scrollStep < 0)
		EnableWindow(upButtonFreq, TRUE);
}
