#include <windows.h>
#include <commctrl.h>
#include <cstdio>
#include <cstring>

#include "versuse-main.hpp"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void UpdateWidthDisplay(HWND hWnd);
int ReadSettings();
int SaveSettings();
void LoadDefaults(HWND hWnd);
int WriteText(HWND hWnd);
int MaxNum(int a, int b) { return (a > b ? a : b); }
int MinNum(int a, int b) { return (a < b ? a : b); }

//enum Align { LEFT = 1, CENTER = 2, RIGHT = 3 };

// GLOBALS EVERYWHERE GLOBALS
char outfile[26] = "versuse-out.txt";
int outw = 69;
int alignL = 2;
int alignR = 2;
int mono = 1;
char leftname[41] = "";
char leftscore[4] = "0";
char rightname[41] = "";
char rightscore[4] = "0";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{	
    WNDCLASSEX wc = {0};
		
	wc.cbSize        = sizeof(wc);
	wc.lpszClassName = "versuseMain";
	wc.lpszMenuName  = NULL;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance     = hInstance;
	wc.cbClsExtra    = NULL;
	wc.cbWndExtra    = NULL;
	wc.hIcon         = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(VERSUSE_ICON));
	wc.hIconSm       = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(VERSUSE_ICON), IMAGE_ICON, 16, 16, 0);
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error! aaaaaaa!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

	HWND hWnd = CreateWindowEx(NULL, 
	        wc.lpszClassName, "Versuse",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 200, 312, 144,
			NULL, NULL,
			hInstance, NULL);

	if (hWnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error! aaaaaaa!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nShowCmd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool acted = true;
	switch(msg)	{
		case WM_CREATE:
		{
			FILE *conf = fopen(VERSUSE_STRING_CONFIG, "r");
			if (conf == NULL) {
				//try write defaults
				if (SaveSettings()) MessageBox(hWnd, "Failed to save default config.", "Err", MB_ICONEXCLAMATION | MB_OK);
			} else {
				//read in saved values
				fclose(conf);
				if (ReadSettings()) MessageBox(hWnd, "Something went wrong reading existing config.", "Err", MB_ICONEXCLAMATION | MB_OK);
			}
			HWND init;
			CreateWindowEx(WS_EX_CLIENTEDGE, 
							"EDIT", leftname, 
							WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
							4, 4, 168, 24,
							hWnd, (HMENU)VERSUSE_EDIT_LEFTNAME,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(WS_EX_CLIENTEDGE, 
							"EDIT", leftscore, 
							WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
							184, 4, 36, 24,
							hWnd, (HMENU)VERSUSE_EDIT_LEFTSCORE,
							GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON|WS_GROUP,
							232, 4, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_LEFT_L,
							GetModuleHandle(NULL), NULL);
			if (alignL == 1) SendMessage(init, BM_SETCHECK, BST_CHECKED, 0);
			init = CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
							254, 4, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_LEFT_C,
							GetModuleHandle(NULL), NULL);
			if (alignL == 2) SendMessage(init, BM_SETCHECK, BST_CHECKED, 0);
			init = CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
							276, 4, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_LEFT_R,
							GetModuleHandle(NULL), NULL);
			if (alignL == 3) SendMessage(init, BM_SETCHECK, BST_CHECKED, 0);
								
			CreateWindowEx(WS_EX_CLIENTEDGE, 
							"EDIT", rightname, 
							WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
							4, 32, 168, 24,
							hWnd, (HMENU)VERSUSE_EDIT_RIGHTNAME,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(WS_EX_CLIENTEDGE, 
							"EDIT", rightscore, 
							WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
							184, 32, 36, 24,
							hWnd, (HMENU)VERSUSE_EDIT_RIGHTSCORE,
							GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON|WS_GROUP,
							232, 32, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_RIGHT_L,
							GetModuleHandle(NULL), NULL);
			if (alignR == 1) SendMessage(init, BM_SETCHECK, BST_CHECKED, 0);
			init = CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
							254, 32, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_RIGHT_C,
							GetModuleHandle(NULL), NULL);
			if (alignR == 2) SendMessage(init, BM_SETCHECK, BST_CHECKED, 0);
			init = CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
							276, 32, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_RIGHT_R,
							GetModuleHandle(NULL), NULL);
			if (alignR == 3) SendMessage(init, BM_SETCHECK, BST_CHECKED, 0);
								
			CreateWindowEx(NULL, 
							"BUTTON", "Write", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
							4, 60, 100, 24,
							hWnd, (HMENU)VERSUSE_BUTTON_WRITE,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"STATIC", VERSUSE_STRING_VERSION, 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD,
							188, 64, 36, 24,
							hWnd, (HMENU)VERSUSE_STATIC_VERSION,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"STATIC", VERSUSE_STRING_L, 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD,
							234, 64, 24, 24,
							hWnd, (HMENU)VERSUSE_STATIC_L,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"STATIC", VERSUSE_STRING_C, 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD,
							256, 64, 24, 24,
							hWnd, (HMENU)VERSUSE_STATIC_C,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"STATIC", VERSUSE_STRING_R, 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD,
							278, 64, 24, 24,
							hWnd, (HMENU)VERSUSE_STATIC_R,
							GetModuleHandle(NULL), NULL);
								
								
			CreateWindowEx(NULL, 
							"STATIC", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD,
							130, 64, 24, 24,
							hWnd, (HMENU)VERSUSE_STATIC_SLIDER,
							GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
							"msctls_trackbar32", "Output Width", 
							WS_VISIBLE|WS_CHILD|TBS_AUTOTICKS|TBS_ENABLESELRANGE,
							2, 88, 292, 12,
							hWnd, (HMENU)VERSUSE_TRACKBAR_WIDTH,
							GetModuleHandle(NULL), NULL);
			SendMessage(init, TBM_SETRANGE, true, MAKELONG(51,161));
			SendMessage(init, TBM_SETSEL, true, MAKELONG(68,100));
			SendMessage(init, TBM_SETPAGESIZE, 0,  2); 
			SendMessage(init, TBM_SETTICFREQ, 10, 0); 
			SendMessage(init, TBM_SETPOS, true, outw); 
			UpdateWidthDisplay(hWnd);
		}
		break;
		case WM_HSCROLL:
			UpdateWidthDisplay(hWnd);
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case VERSUSE_BUTTON_WRITE:
				{
					char buf[4];
					GetWindowText(GetDlgItem(hWnd, VERSUSE_STATIC_SLIDER), buf, 4);
					sprintf(buf, "%03d", outw);
				}
					if (IsDlgButtonChecked(hWnd, VERSUSE_OPTION_LEFT_L)) alignL = 1;
					else if (IsDlgButtonChecked(hWnd, VERSUSE_OPTION_LEFT_R)) alignL = 3;
					else alignL = 2;
					if (IsDlgButtonChecked(hWnd, VERSUSE_OPTION_RIGHT_L)) alignR = 1;
					else if (IsDlgButtonChecked(hWnd, VERSUSE_OPTION_RIGHT_R)) alignR = 3;
					else alignR = 2;
					
					GetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTNAME), leftname, 41);
					GetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTSCORE), leftscore, 4);
					GetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTNAME), rightname, 41);
					GetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTSCORE), rightscore, 4);
					if (SaveSettings()) MessageBox(hWnd, "Failed to save current config.", "Err", MB_ICONEXCLAMATION | MB_OK);
					if (WriteText(hWnd)) MessageBox(hWnd, "Failure. FAILURE. NOOOOOO", "Err", MB_ICONEXCLAMATION | MB_OK);
				break;
				default:
					acted = false;				
			}
		break;
        case WM_CLOSE:
            DestroyWindow(hWnd);
        break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_KEYUP:
			switch (wParam) {
				case VK_RETURN:
					SendMessage(GetDlgItem(hWnd, VERSUSE_VUTTON_WRITE), BM_CLICK, 0, 0);
				break;
				default:
					acted = false;
			}
		break;
		default:
			acted = false;
	}

	if (acted) return 0;
	else return DefWindowProc(hWnd,msg,wParam,lParam);
}

void UpdateWidthDisplay(HWND hWnd) {
	LRESULT pos = SendMessage(GetDlgItem(hWnd, VERSUSE_TRACKBAR_WIDTH), TBM_GETPOS, 0, 0);
	char buf[4];
	outw = pos;
	sprintf(buf, "%03d", (short)pos);
	SetWindowText(GetDlgItem(hWnd, VERSUSE_STATIC_SLIDER), buf);
}

int ReadSettings() {
	char buf[41] = {0};
	FILE *fp = fopen(VERSUSE_STRING_CONFIG, "r");
	if (fp == NULL) return -1;
	if (fgets(buf, 41, fp) == NULL) return 1;
	if (sscanf(buf, "%[^\n]\n", outfile) != 1) return 2;
	if (fgets(buf, 41, fp) == NULL) return 3;
	if (sscanf(buf, "%d %d %d %d\n", &outw, &alignL, &alignR, &mono) != 4) return 4;
	if (fgets(buf, 41, fp) == NULL) return 5;
	if (sscanf(buf, "%[^\n]\n", leftname) != 1) leftname[0] = '\0';
	if (fgets(buf, 41, fp) == NULL) return 7;
	if (sscanf(buf, "%[^\n]\n", leftscore) != 1) {
		leftscore[0] = '0';
		leftscore[1] = '\0';
	}
	if (fgets(buf, 41, fp) == NULL) return 9;
	if (sscanf(buf, "%[^\n]\n", rightname) != 1) rightname[0] = '\0';
	if (fgets(buf, 41, fp) == NULL) return 11;
	if (sscanf(buf, "%[^\n]\n", rightscore) != 1) {
		rightscore[0] = '0';
		rightscore[1] = '\0';
	}
	
	return 0;
}

int SaveSettings() {
	FILE *fp = fopen(VERSUSE_STRING_CONFIG, "w");
	if (fp == NULL) return -1;
	fprintf(fp, VERSUSE_STRING_CONFIG_FORMAT, VERSUSE_LIST_CONFIG_VARS_SET);
	fclose(fp);
	return 0;
}

void LoadDefaults(HWND hWnd) {
	outw = 69;
	alignL = 2;
	alignR = 2;
	leftname[0] = '\0';
	leftscore[0] = '0';
	leftscore[1] = '\0';
	rightname[0] = '\0';
	rightscore[0] = '0';
	rightscore[1] = '\0';
	mono = 0;
	SendMessage(GetDlgItem(hWnd, VERSUSE_OPTION_LEFT_C), BM_SETCHECK, BST_CHECKED, 0);
	SendMessage(GetDlgItem(hWnd, VERSUSE_OPTION_RIGHT_C), BM_SETCHECK, BST_CHECKED, 0);
	SendMessage(GetDlgItem(hWnd, VERSUSE_TRACKBAR_WIDTH), TBM_SETPOS, true, outw);
	UpdateWidthDisplay(hWnd);
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTNAME), leftname);
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTSCORE), leftscore);
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTNAME), rightname);
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTSCORE), rightscore);
}

//this function is what everything else is wrapping, and it is a sloppy mess
int WriteText(HWND hWnd) {
	int n = outw;
	int leftA = alignL;
	int rightA = alignR;
	int leftW = strlen(leftname);
	int rightW = strlen(rightname);
	char *out = (char*)malloc(sizeof(char) * (n + 1));
	int loc = 0;
	int center = n / 2;
	if (n % 2) ++center;
	
	int i = 0;
	while (i < n) {
		out[i] = ' ';
		++i;
	}
	
	int j = (i = 0);
	switch (leftA) {
		case 1:
			loc = 0;			
		break;
		case 2:
			loc = MaxNum(0, (center - 6 - leftW) / 2);
		break;
		case 3:
			loc = MaxNum(0, center - 6 - leftW);
		break;
	}
	while (leftname[i] != '\0') out[loc + j++] = leftname[i++];
	
	j = (i = 0);
	switch (rightA) {
		case 1:
			loc = MinNum(n - rightW, center + 7);
		break;
		case 2:
			loc = MinNum(n - rightW, (center + 7 + n - rightW) / 2);
		break;
		case 3:
			loc = n - rightW;
		break;
	}
	while (rightname[i] != '\0') out[loc + j++] = rightname[i++];
	
	loc = center - (mono?0:1) - strlen(leftscore);
	j = (i = 0);
	while (leftscore[i] != '\0') out[loc + j++] = leftscore[i++];
	
	loc = center + (mono?1:2) + MaxNum(strlen(leftscore), strlen(rightscore)) - strlen(rightscore);
	j = (i = 0);
	while (rightscore[i] != '\0') out[loc + j++] = rightscore[i++];
	
	out[center] = '-';
	out[n] = '\0';
	
	FILE *fp = fopen(outfile, "w");
	if (fp == NULL) MessageBox(hWnd, "Failed to save display text.", "Welp", MB_OK|MB_ICONERROR);
	else {
		fprintf(fp, "%s", out);
		fclose(fp);
	}
	free(out);
	return 0;
}