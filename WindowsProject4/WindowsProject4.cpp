#include "GraphPage.h"
#include "HistogramPage.h"
#include "DiagramPage.h"
#include "PieChartPage.h"
#include "FrequencyPage.h"

// Глобальные переменные
HINSTANCE hInst;

HWND hTabControl;
HWND hGraphPage;
//HWND hChildPage;
HWND hRandomGraphPage;
HWND hHistogramPage;
HWND hPieChartPage;
HWND hDiagramPage;
HWND hHodographPage;
HWND hFrequencyPage;

WNDPROC g_pTabCtrlProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pGraphPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pRandomGraphPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pHistogramPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pPieChartPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pDiagramPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pHodographPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pFrequencyPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pChildDiaPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pChildHistPageProc = nullptr; // Объявление и инициализация указателя
WNDPROC g_pChildFreqPageProc = nullptr; // Объявление и инициализация указателя

//Для построения графиков
LPCWSTR CheckBoxNames[3] = {L"y = cos(x)", L"y = πx / 10 - 1", L"y = sin(x)"};
int CheckBoxIDs[3] = {2001, 2002, 2003};
HWND CheckBox[3];
const int NUM = 70; // Примерное количество точек для графика
double** x; // массив данных для построения графиков

//Для построения гистограммы
int numHistTextBox = 3;
//int HistTextBoxIDs[5][2] = {
//	{3001, 3002},
//	{3011, 3012},
//	{3021, 3022},
//	{3031, 3032},
//	{3041, 3042}
//};
int numHistColumns = 3;
HWND histTextBox[20][2];
TCHAR* histText[20][2];

HWND addButtonHist;
HWND deleteButtonHist;
HWND createButtonHist;
HWND upButtonHist;
HWND downButtonHist;
HWND hChildHistPage;

int histChildScroll = 0;
int curHistChildWidth;

bool flagDrawHist = false;
double maxHistLevel;
double* histData; // массив данных для построения гистограммы

//для построения диаграммы
int numDiaTextBox = 3;
//int diaTextBoxIDs[15][2] = {
//	{3001, 3002},
//	{3011, 3012},
//	{3021, 3022},
//	{3031, 3032},
//	{3041, 3042}
//};
int numDiaColumns = 3;
HWND diaTextBox[20][2];
TCHAR* diaText[20][2];

HWND addButtonDia;
HWND deleteButtonDia;
HWND createButtonDia;
HWND upButtonDia;
HWND downButtonDia;
HWND hChildDiaPage;

int diaChildScroll = 0;
int scrollStep = 30;
int curDiaChildWidth;


bool flagDrawDia = false;
double maxDiaLevel;
double* diaData; // массив данных для построения гистограммы

//Для построения круговой диаграммы
int numPieTextBox = 3;
int pieTextBoxIDs[5][2] = {
	{3001, 3002},
	{3011, 3012},
	{3021, 3022},
	{3031, 3032},
	{3041, 3042}
};
int numSectors = 3;
HWND pieTextBox[5][2];
TCHAR* pieText[5][2];

HWND addButtonPie;
HWND deleteButtonPie;
HWND createButtonPie;

bool flagDrawPie = false;
double* pieData; // массив данных для построения гистограммы

//Для построения частотной характеристики
 int freqChildScroll = 0;
 int numFreqTextBox = 3;
int numFreqColumns = 3;
HWND freqTextBox[20][2];
TCHAR* freqText[20][2];

 HWND addButtonFreq;
 HWND deleteButtonFreq;
 HWND createButtonFreq;
 HWND upButtonFreq;
 HWND downButtonFreq;

 HWND hChildFreqPage;
 int curFreqChildWidth;

 bool flagDrawFreq;
 double maxFreqLevel;
 double* freqData; // массив данных для построения гистограммы
 double maxPos;
 double minNig;


// Прототипы функций
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK TabControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RandomGraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HistogramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PieChartPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DiagramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HodographPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrequencyPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void line(HDC hdc, int Xs, int Ys, int Xf, int Yf); //отрезок прямой
bool containsLetters(TCHAR* str); //проверка на буквы

//массив цветов для диаграмм
int colors[15] = {
	RGB(204, 0, 0),
	RGB(255, 255, 0),
	RGB(51, 102, 0),
	RGB(0, 0, 255),
	RGB(153, 0, 153),
	RGB(102, 0, 204),
	RGB(0, 102, 0),
	RGB(96, 0, 96),
	RGB(51, 255, 255),
	RGB(255, 153, 255),
	RGB(0, 139, 139),
	RGB(255, 228, 181),
	RGB(218, 112, 214),
	RGB(47, 79, 79),
	RGB(255, 255, 255)
};

// Функция регистрации класса дочернего окна
ATOM RegisterChildClass(HINSTANCE hInstance, LPCWSTR className, WNDPROC wndProc) {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

// Функция WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Регистрация класса окна
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.hbrBackground = CreateSolidBrush(0x00FFFFFF);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MainWindow";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	x = getGraphData(NUM); // задание исходных данных

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Регистрация классов дочерних окон
	RegisterChildClass(hInstance, L"GraphPage", GraphPageProc);
	RegisterChildClass(hInstance, L"RandomGraphPage", RandomGraphPageProc);
	RegisterChildClass(hInstance, L"HistogramPage", HistogramPageProc);
	RegisterChildClass(hInstance, L"PieChartPage", PieChartPageProc);
	RegisterChildClass(hInstance, L"DiagramPage", DiagramPageProc);
	RegisterChildClass(hInstance, L"HodographPage", HodographPageProc);
	RegisterChildClass(hInstance, L"FrequencyPage", FrequencyPageProc);
	RegisterChildClass(hInstance, L"ChildDiaPage", ChildDiaPageProc);
	RegisterChildClass(hInstance, L"ChildHistPage", ChildHistPageProc);
	RegisterChildClass(hInstance, L"ChildFreqPage", ChildFreqPageProc);

	// Создание окна
	if (!InitInstance(hInstance, nCmdShow))
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Цикл обработки сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// Функция инициализации экземпляра
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindow(L"MainWindow", L"2D графики", WS_OVERLAPPEDWINDOW,
	                         CW_USEDEFAULT, 0, 1000, 600, NULL, NULL, hInstance, NULL); // 0, 800, 600,

	if (!hWnd)
		return FALSE;

	// Показать окно и обновить его
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// Функция обработки сообщений окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
			case TCN_SELCHANGING:
				{
					// Return FALSE to allow the selection to change.
					return FALSE;
				}

			case TCN_SELCHANGE:
				{
					OutputDebugString(L"It is OK! \n");
					int selectedIndex = TabCtrl_GetCurSel(hTabControl);
					switch (selectedIndex)
					{
					case 0:
						ShowWindow(hGraphPage, SW_SHOW);
						ShowWindow(hRandomGraphPage, SW_HIDE);
						ShowWindow(hHistogramPage, SW_HIDE);
						ShowWindow(hDiagramPage, SW_HIDE);
						ShowWindow(hPieChartPage, SW_HIDE);
						ShowWindow(hHodographPage, SW_HIDE);
						ShowWindow(hFrequencyPage, SW_HIDE);
						break;
					case 1:
						ShowWindow(hGraphPage, SW_HIDE);
						ShowWindow(hRandomGraphPage, SW_SHOW);
						ShowWindow(hHistogramPage, SW_HIDE);
						ShowWindow(hDiagramPage, SW_HIDE);
						ShowWindow(hPieChartPage, SW_HIDE);
						ShowWindow(hHodographPage, SW_HIDE);
						ShowWindow(hFrequencyPage, SW_HIDE);
						break;
					case 2:
						ShowWindow(hGraphPage, SW_HIDE);
						ShowWindow(hRandomGraphPage, SW_HIDE);
						ShowWindow(hHistogramPage, SW_SHOW);
						ShowWindow(hDiagramPage, SW_HIDE);
						ShowWindow(hPieChartPage, SW_HIDE);
						ShowWindow(hHodographPage, SW_HIDE);
						ShowWindow(hFrequencyPage, SW_HIDE);
						break;
					case 3:
						ShowWindow(hGraphPage, SW_HIDE);
						ShowWindow(hRandomGraphPage, SW_HIDE);
						ShowWindow(hHistogramPage, SW_HIDE);
						ShowWindow(hDiagramPage, SW_SHOW);
						ShowWindow(hPieChartPage, SW_HIDE);
						ShowWindow(hHodographPage, SW_HIDE);
						ShowWindow(hFrequencyPage, SW_HIDE);
						break;
					case 4:
						ShowWindow(hGraphPage, SW_HIDE);
						ShowWindow(hRandomGraphPage, SW_HIDE);
						ShowWindow(hHistogramPage, SW_HIDE);
						ShowWindow(hDiagramPage, SW_HIDE);
						ShowWindow(hPieChartPage, SW_SHOW);
						ShowWindow(hHodographPage, SW_HIDE);
						ShowWindow(hFrequencyPage, SW_HIDE);
						break;
					case 5:
						ShowWindow(hGraphPage, SW_HIDE);
						ShowWindow(hRandomGraphPage, SW_HIDE);
						ShowWindow(hHistogramPage, SW_HIDE);
						ShowWindow(hDiagramPage, SW_HIDE);
						ShowWindow(hPieChartPage, SW_HIDE);
						ShowWindow(hHodographPage, SW_SHOW);
						ShowWindow(hFrequencyPage, SW_HIDE);
						break;
					case 6:
						ShowWindow(hGraphPage, SW_HIDE);
						ShowWindow(hRandomGraphPage, SW_HIDE);
						ShowWindow(hHistogramPage, SW_HIDE);
						ShowWindow(hDiagramPage, SW_HIDE);
						ShowWindow(hPieChartPage, SW_HIDE);
						ShowWindow(hHodographPage, SW_HIDE);
						ShowWindow(hFrequencyPage, SW_SHOW);
						break;
					}

					break;
				}
			}
			return TRUE;
		}
	case WM_CREATE:
		{
			OutputDebugString(L"main_WM_CREATE\n");
			// Создаем элемент управления TabControl
			hTabControl = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			                           10, 30, 760, 540, hWnd, NULL, hInst, NULL);
				// Создаем дочерние окна для каждой вкладки
			hGraphPage = CreateWindow(L"GraphPage", L"", WS_CHILD | WS_VISIBLE, 0, 20, 760, 540, hWnd, NULL, hInst, NULL);
			hRandomGraphPage = CreateWindow(L"RandomGraphPage", L"", WS_CHILD | WS_VISIBLE, 0, 20, 760, 540, hWnd, NULL, hInst, NULL);
			hHistogramPage = CreateWindow(L"HistogramPage", L"", WS_CHILD | WS_VISIBLE, 0, 20, 760, 540, hWnd, NULL, hInst, NULL);
			hPieChartPage = CreateWindow(L"PieChartPage", L"", WS_CHILD | WS_VISIBLE, 0, 20, 760, 540, hWnd, NULL, hInst, NULL);
			hDiagramPage = CreateWindow(L"DiagramPage", L"", WS_CHILD | WS_VISIBLE, 0, 20, 760, 540, hWnd, NULL, hInst, NULL);
			hHodographPage = CreateWindow(L"HodographPage", L"", WS_CHILD | WS_VISIBLE, 0, 20, 760, 540, hWnd, NULL, hInst, NULL);
			if (!hHodographPage)
				MessageBox(NULL, L"Hodograph", L"Ошибка", MB_ICONEXCLAMATION | MB_OK); //выводим сообщение об ошибке
			hFrequencyPage = CreateWindow(L"FrequencyPage", L"", WS_CHILD | WS_VISIBLE, 0, 20, 760, 540, hWnd, NULL, hInst, NULL);
			if (!hFrequencyPage)
				MessageBox(NULL, L"Frequency", L"Ошибка", MB_ICONEXCLAMATION | MB_OK); //выводим сообщение об ошибке

			// Добавляем вкладки на TabControl и связываем их с соответствующими окнами
			TCITEM tie;
			tie.mask = TCIF_TEXT | TCIF_PARAM;

			tie.pszText = const_cast<LPTSTR>(L"График");
			tie.lParam = (LPARAM)hGraphPage;
			TabCtrl_InsertItem(hTabControl, 0, &tie);

			tie.pszText = const_cast<LPTSTR>(L"Случайный процесс");
			tie.lParam = (LPARAM)hGraphPage;
			TabCtrl_InsertItem(hTabControl, 1, &tie);

			tie.pszText = const_cast<LPTSTR>(L"Гистограмма");
			tie.lParam = (LPARAM)hDiagramPage;
			TabCtrl_InsertItem(hTabControl, 2, &tie);

			tie.pszText = const_cast<LPTSTR>(L"Диаграмма");
			tie.lParam = (LPARAM)hHistogramPage;
			TabCtrl_InsertItem(hTabControl, 3, &tie);

			tie.pszText = const_cast<LPTSTR>(L"Круговая диаграмма");
			tie.lParam = (LPARAM)hPieChartPage;
			TabCtrl_InsertItem(hTabControl, 4, &tie);

			tie.pszText = const_cast<LPTSTR>(L"Годограф");
			tie.lParam = (LPARAM)hHodographPage;
			TabCtrl_InsertItem(hTabControl, 5, &tie);

			tie.pszText = const_cast<LPTSTR>(L"Частотная характеристика");
			tie.lParam = (LPARAM)hFrequencyPage;
			TabCtrl_InsertItem(hTabControl, 6, &tie);

			ShowWindow(hGraphPage, SW_SHOW);
			ShowWindow(hRandomGraphPage, SW_HIDE);
			ShowWindow(hHistogramPage, SW_HIDE);
			ShowWindow(hDiagramPage, SW_HIDE);
			ShowWindow(hPieChartPage, SW_HIDE);
			ShowWindow(hHodographPage, SW_HIDE);
			ShowWindow(hFrequencyPage, SW_HIDE);

			break;
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.

			//Создание надписи
			HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			                         OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			                         DEFAULT_PITCH | FF_SWISS, L"Arial");

			// серый цвет
			SetTextColor(hdc, RGB(0, 0, 0));
			SelectObject(hdc, hFont); // выбираем шрифт
			SetBkMode(hdc, TRANSPARENT); // фон прозрачный

			RECT rectText = {ps.rcPaint.left, 5, ps.rcPaint.right, ps.rcPaint.bottom};
			DrawText(hdc, L"2D ГРАФИКИ", -1, &rectText, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		}
		break;
	case WM_SIZE:
		{
			// Изменяем размеры элемента управления TabControl при изменении размеров окна
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			SetWindowPos(hTabControl, NULL, 10, 30, rcClient.right - 20, rcClient.bottom - 40, SWP_NOZORDER);
			SetWindowPos(hGraphPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
			SetWindowPos(hRandomGraphPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
			SetWindowPos(hHistogramPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
			SetWindowPos(hPieChartPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
			SetWindowPos(hDiagramPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
			SetWindowPos(hHodographPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
			SetWindowPos(hFrequencyPage, NULL, 15, 60, rcClient.right - 30, rcClient.bottom - 85, SWP_NOZORDER);
			break;
		}
	case WM_GETMINMAXINFO:
		{
			//Устанавливаем минимальные и максимальные размеры
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 700;
			lpMMI->ptMinTrackSize.y = 370;
			break;
		}
	case WM_DESTROY:
		// Отменяем присвоение обработчика сообщений TabControl
		delete[] histData;
		SetWindowLongPtr(hTabControl, GWLP_WNDPROC, (LONG_PTR)g_pTabCtrlProc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void line(HDC hdc, int Xs, int Ys, int Xf, int Yf) // рисование отрезка прямой линии
{
	MoveToEx(hdc, Xs, Ys, 0); // перемещение из точки (Xs, Ys)
	LineTo(hdc, Xf, Yf); // в точку (Xt, Yf)
}

bool containsLetters(TCHAR* str)
{
	while (*str)
	{
		if ((*str < '0' || *str > '9') && (*str != '.') && (*str != '-'))
			return true; // Если найдена буква, возвращаем true
		str++; // Переходим к следующему символу
	}
	return false; // Если не найдено ни одной буквы, возвращаем false
}


//LRESULT CALLBACK DiagramPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (g_pDiagramPageProc)
//		return CallWindowProc(g_pDiagramPageProc, hWnd, message, wParam, lParam);
//	else
//		return DefWindowProc(hWnd, message, wParam, lParam);
//}

LRESULT CALLBACK RandomGraphPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_pRandomGraphPageProc)
		return CallWindowProc(g_pRandomGraphPageProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK HodographPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_pRandomGraphPageProc)
		return CallWindowProc(g_pRandomGraphPageProc, hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}
