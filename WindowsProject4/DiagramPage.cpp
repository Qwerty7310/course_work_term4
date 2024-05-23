#include "DiagramPage.h"

LRESULT CALLBACK DiagramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT state;
	int length; //переменная для длины текстового поля

	RECT rectClient; //перменная области окна
	GetClientRect(hDiagramPage, &rectClient); //получаем область окна

	switch (message)
	{
	case WM_CREATE:
		{
			// Создаем дочерние окна для каждой вкладки
			hChildDiaPage = CreateWindow(L"ChildDiaPage", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 0,
			                             0, hWnd, NULL, hInst, NULL);
		}
		break;
	case WM_SIZE:
		{
			ShowWindow(hChildDiaPage, SW_SHOW);
			// Получаем новые размеры окна
			int newWidth = LOWORD(lParam);
			int newHeight = HIWORD(lParam);
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			SetWindowPos(hChildDiaPage, NULL, rcClient.right - 290, 20, 290,
			             min(rcClient.bottom - 20, 40 + (numDiaTextBox + 1) * 30 + 40), SWP_NOZORDER);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDiagramPage, &ps);

			FillRect(hdc, &rectClient, (HBRUSH)(COLOR_WINDOW));

			if (flagDrawDia) DrawDiagram(hdc, ps.rcPaint); // рисуем гистограммы, если были введены корректные значения
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

				DrawText(hdc, L"Введите Ваши данные для\n построения диаграммы.", -1, &rectText,
				         DT_WORDBREAK | DT_CENTER);
				DeleteObject(hFont);
			}
			EndPaint(hDiagramPage, &ps);
			break;
		}
		break;
	}
	if (g_pDiagramPageProc)
		return CallWindowProc(g_pDiagramPageProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

void DrawDiagram(HDC hdc, RECT rectClient)
{
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон

	//инициализация переменных
	double min, max;
	int height, width;
	HPEN hpen;
	height = rectClient.bottom - rectClient.top - 50; //высота области рисования графика
	width = rectClient.right - rectClient.left - 300; //ширина области рисования графика

	int interval = (height - 20) / 5; //расстояние между горизонтальными линиями гистограммы

	//рисуем оси гистограммы
	hpen = CreatePen(PS_SOLID, 3, RGB(100, 100, 100)); // перо толщиной в 3 пикселя
	SelectObject(hdc, hpen);
	line(hdc, 50, rectClient.bottom - 30, width, rectClient.bottom - 30); // горизонтальная ось гистограммы
	line(hdc, 50, rectClient.bottom - 30, 50, rectClient.top + 30); // вертикальная ось гистограммы
	DeleteObject(hpen);

	hpen = CreatePen(PS_SOLID, 2, RGB(170, 170, 170)); // перо толщиной в 2 пикселя
	SelectObject(hdc, hpen);
	for (int i = 1; i <= 5; i++)
		line(hdc, 40, rectClient.bottom - 30 - i * interval, width, rectClient.bottom - 30 - i * interval);
	//горизонтальная линия уровня гистограммы
	DeleteObject(hpen);

	//создаем шрифт
	HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                         OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                         DEFAULT_PITCH | FF_SWISS, L"Arial");

	SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет текста
	SelectObject(hdc, hFont); //выбираем шрифт
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон
	//добавляем шкалу гистограммы
	for (int i = 0; i <= 5; i++)
	{
		RECT rectText = {0, rectClient.bottom - 30 - i * interval - 10, 35, rectClient.bottom - 30 - i * interval + 10};

		TCHAR str1[20];
		if ((i * maxDiaLevel / 5) == (int)(i * maxDiaLevel / 5))
			swprintf_s(str1, L"%d", (int)(i * maxDiaLevel / 5)); //целое число
		else
			swprintf_s(str1, L"%.2lf", (i * maxDiaLevel / 5)); //дробное число

		DrawText(hdc, str1, -1, &rectText, DT_WORDBREAK | DT_RIGHT); //рисуем текст
	}
	DeleteObject(hFont);

	//выбираем шрифт
	hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                   DEFAULT_PITCH | FF_SWISS, L"Arial");
	SetTextColor(hdc, RGB(0, 0, 0)); //выбираем цвет
	SelectObject(hdc, hFont); //выбираем шрифт
	SetBkMode(hdc, TRANSPARENT); //прозрачный фон

	int length; //длина текста

	//рисуем столбцы
	int leftColumnX = rectClient.left + 75;
	int rightColumnX = rectClient.left + width - 20;
	int widthColumn = (rightColumnX - leftColumnX) / numDiaColumns;
	for (int i = 0; i < numDiaColumns; i++)
	{
		RECT r;
		r.left = leftColumnX + i * widthColumn;
		r.bottom = rectClient.bottom - 31;
		r.top = r.bottom - (5. * interval * diaData[i] / maxDiaLevel);
		r.right = leftColumnX + (i + 1) * widthColumn;
		OutputDebugString(L"DrawColumns!\n");
		//рисуем столбец
		FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(colors[i]));

		RECT rectText; //область текста
		rectText.left = r.left;
		rectText.right = r.right;
		rectText.top = r.top - 20;
		rectText.bottom = r.top - 5;

		wchar_t numStr[10]; //буфер для числового значения
		//добавляем числовое значение
		if (diaData[i] == (int)diaData[i])
			swprintf_s(numStr, L"%d", (int)diaData[i]); //целое число
		else
			swprintf_s(numStr, L"%.2lf", diaData[i]); //дробное число
		DrawText(hdc, numStr, -1, &rectText, DT_WORDBREAK | DT_CENTER); //рисуем текст
	}
	DeleteObject(hFont);

	//добавляем подписи к стобцам
	hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                   DEFAULT_PITCH | FF_SWISS, L"Arial"); //создаем шрифт
	SelectObject(hdc, hFont); //выбираем новый шрифт
	for (int i = 0; i < numDiaColumns; i++)
	{
		RECT rectText; //получаем область текста
		rectText.left = leftColumnX + i * widthColumn; //верхний левый угол
		rectText.top = rectClient.bottom - 20;
		rectText.right = leftColumnX + (i + 1) * widthColumn; //нижний правый угол
		rectText.bottom = rectText.top + 20;

		DrawText(hdc, diaText[i][1], -1, &rectText, DT_WORDBREAK | DT_CENTER); //добалвяем подпись к столбцу
	}
}

int getDiagramData()
{
	wchar_t message[1024] = L""; //инициализируем переменную для записи сообщения об ошибке

	bool flagError = false; //флаг наличия ошибки в введенных данных
	int length; //длина текста в TextBox
	double* tempArr = new double[numDiaTextBox]; //временный массив для хранения данных

	for (int i = 0; i < numDiaTextBox; i++)
	{
		length = GetWindowTextLength(diaTextBox[i][0]); //получаем длину
		TCHAR* buffer1 = new TCHAR[length + 1]; //создаем буфер для хранения строки
		GetWindowText(diaTextBox[i][0], buffer1, length + 1); // получаем текст из текстового поля

		//проверяем корректность введенных данных
		if ((containsLetters(buffer1) == true) || (wcslen(buffer1) == 0))
		//если есть неразрешенные символы или длина текста равно 0
		{
			flagError = true; //флаг ошибки

			//выбор соответствующего сообщения
			const wchar_t* str;
			if (containsLetters(buffer1) == true)
			{
				str = L"Значение некорректно:\tстрока ";
			}
			else
			{
				str = L"Значение отсутствует:\tстрока ";
			}
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
			//проверка на отрицательные значения
			if (tempArr[i] < 0)
			{
				flagError = true; //флаг ошибки
				//добавление сообщения пользователю
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
		length = GetWindowTextLength(diaTextBox[i][1]); //получаем длину
		TCHAR* buffer2 = new TCHAR[length + 1]; //создаем буфер для хранения строки
		GetWindowText(diaTextBox[i][1], buffer2, length + 1); //получаем данные из текстового поля
		if (wcslen(buffer2) == 0)
		{
			//если название отсутствует
			flagError = true;
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
		delete[] diaData; //удаляем старый
		diaData = new double[numDiaTextBox]; //создаем новый
		for (int i = 0; i < numDiaTextBox; i++)
		{
			diaData[i] = tempArr[i]; //записываем данные в массив
		}
		delete[] tempArr; //очищаем память под временный массив

		//находим максимальное значение
		double max = -1; //инициализируем переменную для поиска максимума
		for (int i = 0; i < numDiaTextBox; i++)
		{
			if (diaData[i] > max) max = diaData[i];
		}
		if (max == 0)
		{
			//если все числа равны 0
			MessageBox(NULL, L"Должно быть хотя бы 1 положительное число!", L"Ошибка", MB_ICONEXCLAMATION | MB_OK);
			//сообщение об ошибке
			return 1; //возвращаем 1 (значение наличия ошибки)
		}
		else
		{
			//находим максимальное значение шкалы гистограммы
			maxDiaLevel = 0; //максимальное значение шкалы гистограммы

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
					maxDiaLevel = tempInt + i * pow(10, (order > 1) ? (order - 2) : 0);
					break;
				}
			}
			//учитываем изменения для tempInt перед началом поиска
			if (order < 0) maxDiaLevel *= pow(10, order);
			if (multiplier) maxDiaLevel /= 10;

			return 0;
		}
	}
	return 0;
}


LRESULT CALLBACK ChildDiaPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT state;
	int length; // переменная для длины текстового поля


	RECT rectClient; // переменная области окна
	GetClientRect(hChildDiaPage, &rectClient); // получаем область окна

	int xStart = 25;
	switch (message)
	{
	case WM_CREATE:
		{
			HDC hdc = GetDC(hChildDiaPage);
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
			HWND hStaticText = CreateWindowEx(0, L"STATIC", L"Значение         Название",
			                                  WS_CHILD | WS_VISIBLE | SS_LEFT, xStart, 10, 240, 20,
			                                  hWnd, NULL, NULL, NULL);
			SendMessage(hStaticText, WM_SETFONT, (WPARAM)hFont, TRUE);
			// Сохранение дескриптора текста в пользовательских данных окна
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)hStaticText);
			for (int i = 0; i < MAX_DIAGRAM_TEXTBOX; i++)
			{
				wchar_t numStr[10]; //буфер для числа
				//добавления текста в сообщение для пользователя
				swprintf(numStr, sizeof(numStr) / sizeof(numStr[0]), L"%d", i + 1);
				diaTextBox[i][0] = CreateWindowEx(NULL, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
				                                  xStart + 6, 40 + i * 30, 60, 20,
				                                  hWnd, nullptr,
				                                  (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
				diaTextBox[i][1] = CreateWindowEx(NULL, L"Edit", numStr, WS_CHILD | WS_VISIBLE | WS_BORDER,
				                                  xStart + 76, 40 + i * 30, 160, 20,
				                                  hWnd, nullptr,
				                                  (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
				if (i < numDiaTextBox)
				{
					ShowWindow(diaTextBox[i][0], SW_SHOW);
					ShowWindow(diaTextBox[i][1], SW_SHOW);
				}
				else
				{
					ShowWindow(diaTextBox[i][0], SW_HIDE);
					ShowWindow(diaTextBox[i][1], SW_HIDE);
				}
			}
			addButtonDia = CreateWindow(L"Button", L"Добавить", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart + 16,
			                            40 + numDiaTextBox * 30, 100, 25, hWnd,
			                            (HMENU)ID_ADD_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                            nullptr);
			deleteButtonDia = CreateWindow(L"Button", L"Убрать", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart + 126,
			                               40 + numDiaTextBox * 30, 100, 25, hWnd,
			                               (HMENU)ID_DELETE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                               nullptr);
			createButtonDia = CreateWindow(L"Button", L"Построить", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart + 16,
			                               40 + (numDiaTextBox + 1) * 30, 210, 25, hWnd,
			                               (HMENU)ID_CREATE_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                               nullptr);
			upButtonDia = CreateWindow(L"Button", L"∧", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart,
			                           0, 0, 0, hWnd,
			                           (HMENU)ID_UP_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                           nullptr);
			downButtonDia = CreateWindow(L"Button", L"∨", WS_VISIBLE | WS_CHILD | WS_BORDER, xStart,
			                             0, 0, 0, hWnd,
			                             (HMENU)ID_DOWN_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			                             nullptr);

			EnableWindow(upButtonDia, FALSE);
			EnableWindow(downButtonDia, FALSE);
		}
		break;
	case WM_SIZE:
		{
			//ShowWindow(hChildDiaPage, SW_SHOW);
			// Получаем новые размеры окна

			RECT rcClient;
			GetClientRect(hChildDiaPage, &rcClient);
			curDiaChildWidth = rcClient.bottom;

			//SetWindowPos(hTabControl, NULL, 10, 30, rcClient.right - 20, rcClient.bottom - 40, SWP_NOZORDER);

			SetWindowPos(upButtonDia, NULL, rcClient.right - 25, rcClient.bottom - 55, 20, 20, SWP_NOZORDER);
			SetWindowPos(downButtonDia, NULL, rcClient.right - 25, rcClient.bottom - 25, 20, 20, SWP_NOZORDER);

			if (curDiaChildWidth < 40 + (numDiaTextBox + 1) * 30 + 25)
				EnableWindow(downButtonDia, TRUE);
			else
				EnableWindow(downButtonDia, FALSE);

			if (rcClient.bottom >= 40 + (numDiaTextBox + 1) * 30 + 25 + (diaChildScroll + 1) * scrollStep)
				DiaChildScrollUp(hWnd);

			if (!IsWindowEnabled(upButtonDia) && !IsWindowEnabled(downButtonDia))
			{
				ShowWindow(upButtonDia, SW_HIDE);
				ShowWindow(downButtonDia, SW_HIDE);
			}
			else
			{
				ShowWindow(upButtonDia, SW_SHOW);
				ShowWindow(downButtonDia, SW_SHOW);
			}

			break;
		}
	case WM_PAINT:
		{
			OutputDebugString(L"PAINT CHILD\n");
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hChildDiaPage, &ps);
			FillRect(hdc, &rectClient, (HBRUSH)(COLOR_WINDOW));
			//DrawTextOnDiagramPage(hWnd, hdc, ps.rcPaint); // рисуем текст
			EndPaint(hChildDiaPage, &ps);
		}
		break;
	case WM_COMMAND:
		{
			HWND hStaticText;
			RECT rcClient;

			switch (LOWORD(wParam))
			{
			case ID_ADD_BUTTON: //обработка нажатия на кнопку добавления
				numDiaTextBox += 1;
				GetClientRect(hDiagramPage, &rcClient);

			//SetWindowPos(hTabControl, NULL, 10, 30, rcClient.right - 20, rcClient.bottom - 40, SWP_NOZORDER);

				SetWindowPos(hChildDiaPage, NULL, rcClient.right - 290, 20, 290,
				             min(rcClient.bottom - 20, 40 + (numDiaTextBox + 1) * 30 + 40), SWP_NOZORDER);

				EnableWindow(deleteButtonDia, TRUE);

				SetWindowPos(createButtonDia, NULL, xStart + 16,
				             40 + (numDiaTextBox + 1) * 30 + diaChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				SetWindowPos(addButtonDia, NULL, xStart + 16,
				             40 + numDiaTextBox * 30 + diaChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				SetWindowPos(deleteButtonDia, NULL, xStart + 126,
				             40 + numDiaTextBox * 30 + diaChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				ShowWindow(diaTextBox[numDiaTextBox - 1][0], SW_SHOW);
				ShowWindow(diaTextBox[numDiaTextBox - 1][1], SW_SHOW);

				if (numDiaTextBox == 15)
					EnableWindow(addButtonDia, FALSE);
				if (curDiaChildWidth < 40 + (numDiaTextBox + 1) * 30 + 25)
				{
					DiaChildScrollDown(hWnd);
					EnableWindow(upButtonDia, TRUE);
				}
				if (!IsWindowEnabled(upButtonDia) && !IsWindowEnabled(downButtonDia))
				{
					ShowWindow(upButtonDia, SW_HIDE);
					ShowWindow(downButtonDia, SW_HIDE);
				}
				else
				{
					ShowWindow(upButtonDia, SW_SHOW);
					ShowWindow(downButtonDia, SW_SHOW);
				}

				break;
			case ID_DELETE_BUTTON: //обработка нажатия на кнопку удаления
				EnableWindow(addButtonDia, TRUE);
				numDiaTextBox -= 1;
				GetClientRect(hDiagramPage, &rcClient);

			//SetWindowPos(hTabControl, NULL, 10, 30, rcClient.right - 20, rcClient.bottom - 40, SWP_NOZORDER);

				SetWindowPos(hChildDiaPage, NULL, rcClient.right - 290, 20, 290,
				             min(rcClient.bottom - 20, 40 + (numDiaTextBox + 1) * 30 + 40), SWP_NOZORDER);

				ShowWindow(diaTextBox[numDiaTextBox][0], SW_HIDE);
				ShowWindow(diaTextBox[numDiaTextBox][1], SW_HIDE);
				SetWindowPos(addButtonDia, NULL, xStart + 16,
				             40 + numDiaTextBox * 30 + diaChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				SetWindowPos(deleteButtonDia, NULL, xStart + 126,
				             40 + numDiaTextBox * 30 + diaChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				SetWindowPos(createButtonDia, NULL, xStart + 16,
				             40 + (numDiaTextBox + 1) * 30 + diaChildScroll * scrollStep, 0, 0,
				             SWP_NOSIZE);
				if (numDiaTextBox == 2)
					EnableWindow(deleteButtonDia, FALSE);

				if (curDiaChildWidth >= 40 + (numDiaTextBox + 1) * 30 + 25)
				{
					EnableWindow(upButtonDia, FALSE);
					EnableWindow(downButtonDia, FALSE);
				}
				if (10 + diaChildScroll * scrollStep < 0)
					DiaChildScrollUp(hWnd);
				if (!IsWindowEnabled(upButtonDia) && !IsWindowEnabled(downButtonDia))
				{
					ShowWindow(upButtonDia, SW_HIDE);
					ShowWindow(downButtonDia, SW_HIDE);
				}
				else
				{
					ShowWindow(upButtonDia, SW_SHOW);
					ShowWindow(downButtonDia, SW_SHOW);
				}

				break;
			case ID_CREATE_BUTTON: //обработка нажатия на кнопку построения гистограммы
				if (!getDiagramData()) //проверка на корректность данных и получени данных для построения
				{
					//данные корректны
					for (int i = 0; i < numDiaTextBox; i++)
						for (int j = 0; j < 2; j++)
						{
							length = GetWindowTextLength(diaTextBox[i][j]); // получаем длину текста из текстового поля
							diaText[i][j] = new TCHAR[length + 1]; //создаем строку символов для записи
							GetWindowText(diaTextBox[i][j], diaText[i][j], length + 1); //записываем текст
						}
					numDiaColumns = numDiaTextBox; //изменяем число столбцов диаграммы
					flagDrawDia = true; //разрешаем рисовать гистограмму
					InvalidateRect(hDiagramPage, NULL, TRUE); //команда на перерисовку окна
				}
				break;
			case ID_DOWN_BUTTON: //обработка нажатия на кнопку построения гистограммы
				DiaChildScrollDown(hWnd);
				break;
			case ID_UP_BUTTON: //обработка нажатия на кнопку построения гистограммы
				DiaChildScrollUp(hWnd);
				break;
			}
		}
		break;
	}

	//return DefWindowProc(hWnd, message, wParam, lParam);
	if (g_pChildDiaPageProc)
		return CallWindowProc(g_pChildDiaPageProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

void DiaChildScrollUp(HWND hWnd)
{
	HWND hStaticText;
	RECT rcClient;
	int xStart = 25;
	diaChildScroll += 1;

	SetWindowPos(createButtonDia, NULL, xStart + 16,
	             40 + (numDiaTextBox + 1) * 30 + diaChildScroll * scrollStep,
	             210, 25, SWP_NOSIZE);
	SetWindowPos(addButtonDia, NULL, xStart + 16, 40 + numDiaTextBox * 30 + diaChildScroll * scrollStep,
	             100, 25,
	             SWP_NOSIZE);
	SetWindowPos(deleteButtonDia, NULL, xStart + 126, 40 + numDiaTextBox * 30 + diaChildScroll * scrollStep,
	             100, 25,
	             SWP_NOSIZE);
	for (int i = MAX_DIAGRAM_TEXTBOX; i >= 0; i--)
	{
		SetWindowPos(diaTextBox[i][0], NULL, xStart + 6, 40 + i * 30 + diaChildScroll * scrollStep, 60, 20,
		             SWP_NOSIZE);
		SetWindowPos(diaTextBox[i][1], NULL, xStart + 76, 40 + i * 30 + diaChildScroll * scrollStep, 160,
		             20,
		             SWP_NOSIZE);
	}
	GetClientRect(hChildDiaPage, &rcClient);
	curDiaChildWidth = rcClient.bottom;
	hStaticText = (HWND)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	SetWindowPos(hStaticText, NULL, xStart, 10 + diaChildScroll * scrollStep, 0, 0, SWP_NOSIZE);

	if (10 + diaChildScroll * scrollStep > 0)
	{
		EnableWindow(upButtonDia, FALSE);
	}
	if (curDiaChildWidth < 40 + (numDiaTextBox + 1) * 30 + 25 + diaChildScroll * scrollStep)
	{
		EnableWindow(downButtonDia, TRUE);
	}
}

void DiaChildScrollDown(HWND hWnd)
{
	int xStart = 25;
	RECT rcClient;
	HWND hStaticText;

	diaChildScroll -= 1;

	GetClientRect(hChildDiaPage, &rcClient);
	curDiaChildWidth = rcClient.bottom;
	hStaticText = (HWND)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	SetWindowPos(hStaticText, NULL, 0, 10 + diaChildScroll * scrollStep, 0, 0, SWP_NOSIZE);

	for (int i = 0; i < MAX_DIAGRAM_TEXTBOX; i++)
	{
		SetWindowPos(diaTextBox[i][0], NULL, xStart + 6, 40 + i * 30 + diaChildScroll * scrollStep, 60, 20,
		             SWP_NOSIZE);
		SetWindowPos(diaTextBox[i][1], NULL, xStart + 76, 40 + i * 30 + diaChildScroll * scrollStep, 160,
		             20,
		             SWP_NOSIZE);
	}
	SetWindowPos(addButtonDia, NULL, xStart + 16, 40 + numDiaTextBox * 30 + diaChildScroll * scrollStep,
	             100, 25,
	             SWP_NOSIZE);
	SetWindowPos(deleteButtonDia, NULL, xStart + 126, 40 + numDiaTextBox * 30 + diaChildScroll * scrollStep,
	             100, 25,
	             SWP_NOSIZE);
	SetWindowPos(createButtonDia, NULL, xStart + 16,
	             40 + (numDiaTextBox + 1) * 30 + diaChildScroll * scrollStep,
	             210, 25, SWP_NOSIZE);

	if (curDiaChildWidth > 40 + (numDiaTextBox + 1) * 30 + 25 + diaChildScroll * scrollStep)
	{
		EnableWindow(upButtonDia, TRUE);
		EnableWindow(downButtonDia, FALSE);
	}
	if (10 + diaChildScroll * scrollStep < 0)
		EnableWindow(upButtonDia, TRUE);
}