#include <windows.h>
#include <cstdio>
#include <cstring>

#include "versuse-main.hpp"

BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

int ReadSettings();
int SaveSettings();
void LoadDefaults(HWND hWnd);
int WriteText(HWND hWnd);
int MaxNum(int a, int b) { return (a > b ? a : b); }
int MinNum(int a, int b) { return (a < b ? a : b); }

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

HINSTANCE augh = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{	
	augh = hInst;
	HWND hDiag = 0;
	
	hDiag = CreateDialog(hInst, MAKEINTRESOURCE(TEST_MAIN), 0, (DLGPROC)DialogProc);
	
	if (hDiag == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error! aaaaaaa!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
        if (!IsDialogMessage(hDiag, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
	}
	return msg.wParam;
}

BOOL CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool acted = true;
	switch (msg) {
		case WM_INITDIALOG:
		{
			HICON hIcon = LoadIcon(augh, MAKEINTRESOURCE(TEST_ICON));
			SendMessage(hWnd, WM_SETICON, WPARAM(TRUE), LPARAM(hIcon));
			hIcon = LoadIcon(augh, MAKEINTRESOURCE(TEST_ICO2));
			SendMessage(hWnd, WM_SETICON, WPARAM(FALSE), LPARAM(hIcon));
			
			FILE *conf = fopen(VERSUSE_STRING_CONFIG, "r");
			if (conf == NULL) {
				//try write defaults
				if (SaveSettings()) MessageBox(hWnd, "Failed to save default config.", "Err", MB_ICONEXCLAMATION | MB_OK);
			} else {
				//read in saved values
				fclose(conf);
				if (ReadSettings()) MessageBox(hWnd, "Something went wrong reading existing config.", "Err", MB_ICONEXCLAMATION | MB_OK);
			}
			if (alignL == 1) SendMessage(GetDlgItem(hWnd, VERSUSE_EDIT_LEFT_L), BM_SETCHECK, BST_CHECKED, 0);
			if (alignL == 2) SendMessage(GetDlgItem(hWnd, VERSUSE_EDIT_LEFT_C), BM_SETCHECK, BST_CHECKED, 0);
			if (alignL == 3) SendMessage(GetDlgItem(hWnd, VERSUSE_EDIT_LEFT_R), BM_SETCHECK, BST_CHECKED, 0);
			if (alignR == 1) SendMessage(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHT_L), BM_SETCHECK, BST_CHECKED, 0);
			if (alignR == 2) SendMessage(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHT_C), BM_SETCHECK, BST_CHECKED, 0);
			if (alignR == 3) SendMessage(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHT_R), BM_SETCHECK, BST_CHECKED, 0);
			SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTNAME), leftname.c_str());
			SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTSCORE), leftscore.c_str());
			SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTNAME), rightname.c_str());
			SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTSCORE), rightscore.c_str());
		}
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
		default:
			acted = false;
	}
	return acted;
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