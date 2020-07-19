//
// �������� ������: MAIN.CPP
//
// �������� ���������:
//   ���������� ��������� �� ����� ���� ������ ������� LinksPlatform.
//   See: https://github.com/linksplatform/Data.Doublets
//
// ������� ���������: WINDOWS x86/64
//

#include "include.h"
#include "resource.h"
#include "engine.h"
#include "main.h"

//
// ���������� ���������
//
const TCHAR *szNull     = TEXT("");
const TCHAR *szFileMask = TEXT("*.links");
const TCHAR *szWildCard = TEXT("LinksPlatfrom Database File\0*.links\0\0");


//
// ���������� ����������
//
LinksPlatformDB db;                    // �������� ��������� ���� ������
HINSTANCE hInst;                       // ��������� ����������
HACCEL hAccelTable;                    // ������� �������������
HWND hListView;                        // ������� ���������� ListView
HWND hStatusBar;                       // ������� ���������� StatusBar
TCHAR szWindowClass[MAX_LOADSTRING];   // ��� ������ �������� ����
TCHAR szAbout[MAX_LOADSTRING];         // ������ �� ������� "� ���������..."
TCHAR szAboutTitle[MAX_LOADSTRING];    // ��������� ������� "� ���������..."
TCHAR szError[MAX_LOADSTRING];         // ��������� �� ������
TCHAR szOpenDlgTitle[MAX_LOADSTRING];  // ��������� ������� �������� �����
TCHAR szFilePath[MAX_PATH];            // ���� �� �������� ����� ���� ������


//
//   �������: int _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
//
//   ����������: ����� ����� ��� ����������
//
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine, int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	// ���������� �� ����� ������������ �������� ������
	UNREFERENCED_PARAMETER(lpCmdLine);
	HWND hWnd;

	// ������������� ���������� ����������, �������� ��������� ��������
	LoadString(hInstance, IDS_WNDCLASS, szWindowClass, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ABOUTTITLE, szAboutTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ABOUT, szAbout, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ERROR, szError, MAX_LOADSTRING);
	LoadString(hInstance, IDDS_OPENTITLE, szOpenDlgTitle, MAX_LOADSTRING);
	hAccelTable = LoadAccelerators(hInstance, TEXT("APP_ACCELERATORS"));

	// ����������� ������ ����
	if (!RegisterWndClass())
		return false;

	// �������� ���� � ��������� ����������
	hWnd = InitWnd();
	if (!hWnd)
		return false;

	MSG msg;
	// ���� ��������� ��������� ����������
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}


//
// �������: ATOM RegisterWndClass()
//
// ����������: ������������ ����� ����
//
// ����������: ���� ������ ���������� ��������
//
ATOM RegisterWndClass() {
	// ������������ ����� ����
	WNDCLASSEX  wcex;
	ATOM        aReturn;

	wcex.cbSize          = sizeof(WNDCLASSEX);
	wcex.style           = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc     = (WNDPROC)WndProc;
	wcex.cbClsExtra      = 0;
	wcex.cbWndExtra      = 0;
	wcex.hInstance       = hInst;
	wcex.hCursor         = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground   = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName    = MAKEINTRESOURCE(IDM_MAINMENU);
	wcex.lpszClassName   = szWindowClass;
	wcex.hIcon           = LoadIcon(hInst, IDI_APPLICATION);
	wcex.hIconSm         = (HICON) LoadImage(hInst,
	                       IDI_APPLICATION, IMAGE_ICON,
	                       16, 16, 0);
	aReturn = RegisterClassEx(&wcex);
	// ��������� ��� ���������� ������ Windows(NT 4.5? =)
	if (!aReturn) {
		WNDCLASS wc;
		wc.style          = 0;
		wc.lpfnWndProc    = (WNDPROC)WndProc;
		wc.cbClsExtra     = 0;
		wc.cbWndExtra     = 0;
		wc.hInstance      = hInst;
		wc.hIcon          = LoadIcon(hInst, IDI_APPLICATION);
		wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground  = (HBRUSH) (COLOR_WINDOW + 1);
		wc.lpszMenuName   = MAKEINTRESOURCE(IDM_MAINMENU);
		wc.lpszClassName  = szWindowClass;
		aReturn = RegisterClass(&wc);
	}
	return aReturn;
}


//
// �������: WHND InitWnd()
//
// ����������: ������ ���� � �������� ����������, ���������� ����
//
// ����������: ������������ ���� / NULL
//
HWND InitWnd() {
	HWND hWnd = CreateWindow(szWindowClass, szFilePath,
	                         WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
	                         600, 450, NULL, NULL, hInst, NULL);
	if (hWnd) {
		if (!InitListView(hWnd))
			return NULL;
		if (!InitStatusBar(hWnd))
			return NULL;
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		RefreshWindow(hWnd);
		return hWnd;
	} else
		return NULL;
}


//
// �������: bool InitListView(HWND)
//
// ����������: ������ ������� ���������� ListView
//
// ����������: ���� ������ ���������� ��������
//
bool InitListView(HWND hWnd) {
	INT dwStyle = WS_TABSTOP |
	              WS_CHILD |
	              WS_VISIBLE |
	              LVS_SHOWSELALWAYS |
	              LVS_SINGLESEL |
	              LVS_REPORT |
	              LVS_OWNERDATA;
	hListView = CreateWindowEx(WS_EX_CLIENTEDGE,
	                           WC_LISTVIEW,
	                           (PCTSTR) NULL,
	                           dwStyle,
	                           0, 0, 0, 0,
	                           hWnd,
	                           (HMENU) IDC_LISTVIEW,
	                           hInst,
	                           NULL);
	if (hListView) {
		ListView_SetExtendedListViewStyle (
		    hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES
		);
		LV_COLUMN lvColumn;
		TCHAR buf[MAX_LOADSTRING];
		lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 100;
		lvColumn.pszText = buf;
		for (UINT i = 0; i < IDS_COL_num; i++) {
			LoadString(hInst, IDS_COL+i, buf, MAX_LOADSTRING);
			ListView_InsertColumn(hListView, i, &lvColumn);
		}
	}
	return (hListView != NULL);
}


//
// �������: bool InitStatusBar(HWND)
//
// ����������: ������ ������� ���������� StatusBar
//
// ����������: ���� ������ ���������� ��������
//
bool InitStatusBar(HWND hWnd) {
	INT dwStyle = WS_CHILD | WS_VISIBLE;
	hStatusBar = CreateWindowEx(0,
	                            STATUSCLASSNAME,
	                            (PCTSTR) NULL,
	                            dwStyle, 0, 0, 0, 0,
	                            hWnd,
	                            (HMENU) IDC_STATUSBAR,
	                            hInst, NULL);
	if (hStatusBar) {

	}
	return (hStatusBar != NULL);
}


//
// �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
// ����������: ������������ ��������� � ������� ����
//
// WM_COMMAND	- ��������� ���� ����������
// WM_NOTIFY	- ��������� �������� ����������� ���� ������ � ListView
// WM_SIZE      - �������� ����������� ����
// WM_DESTROY	- ������ ��������� � ������ � ���������
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
	switch (message) {
		case WM_COMMAND:
			int wmId;
			wmId = LOWORD(wParam);
			LPTSTR tname;
			switch (wmId) {
				case IDM_OPEN:
					tname = OpenFileDialog(szOpenDlgTitle, szFileMask, hWnd);
					if (tname && _tcslen(tname)>0) {
						LinksPlatformDB tdb;
						if (tdb.load(tname)) {
							db.load(tdb);
							_tcscpy(szFilePath, tname);
						}
					}
					RefreshWindow(hWnd);
					break;

				case IDM_ABOUT:
					MessageBox(hWnd, szAbout, szAboutTitle, MB_OK);
					RefreshWindow(hWnd);
					break;
			}
			break; /// case WM_COMMAND

		case WM_NOTIFY:
			LV_DISPINFO *lpdi;
			lpdi = (LV_DISPINFO*) lParam;
			UINT itemid;
			itemid = lpdi->item.iItem;
			switch (lpdi->hdr.code) {
				case LVN_GETDISPINFO:
					// ����� ���������� � ���������?
					if (lpdi->item.mask & LVIF_TEXT) {
						TCHAR buf[25];
						switch(lpdi->item.iSubItem) {
							case 0:
								_stprintf(buf, TEXT("%u"), itemid);
								lpdi->item.pszText = buf;
								break;
							case 1:
								_stprintf(buf, TEXT("%lld"), db.item[itemid].Source);
								lpdi->item.pszText = buf;
								break;
							case 2:
								_stprintf(buf, TEXT("%lld"), db.item[itemid].Target);
								lpdi->item.pszText = buf;
								break;
							case 3:
								_stprintf(buf, TEXT("%lld"), db.item[itemid].LeftAsSource);
								lpdi->item.pszText = buf;
								break;
							case 4:
								_stprintf(buf, TEXT("%lld"), db.item[itemid].LeftAsTarget);
								lpdi->item.pszText = buf;
								break;
							case 5:
								_stprintf(buf, TEXT("%lld"), db.item[itemid].RightAsSource);
								lpdi->item.pszText = buf;
								break;
							case 6:
								_stprintf(buf, TEXT("%lld"), db.item[itemid].RightAsTarget);
								lpdi->item.pszText = buf;
								break;
							case 7:
								_stprintf(buf, TEXT("%lld"), db.item[itemid].SizeAsSource);
								lpdi->item.pszText = buf;
								break;
							case 8:
								_stprintf(buf, TEXT("%lld"), db.item[itemid].SizeAsTarget);
								lpdi->item.pszText = buf;
								break;
						} /// switch(lpdi->item.iSubItem)
					}
					break;
			} /// switch (lpdi->hdr.code)
			break; /// case WM_NOTIFY

		case WM_SIZE:
			RefreshWindow(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	} /// switch(message)
	return DefWindowProc(hWnd, message, wParam, lParam);
} /// WndProc()


//
// �������: RefreshWindow(HWND)
//
// ����������: ��������� �������� �������� ����
//
void RefreshWindow(HWND hWnd) {
	ListView_SetItemCount(hListView, db.length());

	RECT rc, sbrc;
	GetClientRect(hWnd, &rc);
	GetClientRect(hStatusBar, &sbrc);
	LONG sbheight = (sbrc.bottom - sbrc.top);
	MoveWindow(hListView,
	           rc.left,
	           rc.top,
	           rc.right - rc.left,
	           rc.bottom - rc.top - sbheight,
	           true);
	SendMessage(hStatusBar, WM_SIZE, 0, 0);

	for (int i=0; i<IDS_COL_num; i++)
		ListView_SetColumnWidth(hListView, i, LVSCW_AUTOSIZE_USEHEADER);

	SetWindowText(hWnd, szFilePath);
}



//
// �������: LPTSTR OpenFileDialog(LPTSTR, LPTSTR, HWND)
//
// ����������: ���������� ������ �������� �����
//
LPTSTR OpenFileDialog(LPTSTR Title, LPCTSTR Filter, HWND hWnd) {
	OPENFILENAME ofn;
	LPTSTR buffer = (LPTSTR) operator new(MAX_PATH);
	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(buffer, MAX_PATH);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = szWildCard;
	ofn.nFilterIndex = 1;
	ofn.hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
	    ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	GetOpenFileName(&ofn);
	return buffer;
}

