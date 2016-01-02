#include <windows.h>
#include <cstdio>
#include <cstring>

#include "versuse-main.hpp"

BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void LoadDefaults();
int SaveSettings();
int ReadSettings();
void DisplaySettings(HWND);
int WriteText(HWND);

int MaxNum(int a, int b) { return (a > b ? a : b); }
int MinNum(int a, int b) { return (a < b ? a : b); }

void SaveSettingsOrComplain(HWND hWnd) { if (SaveSettings()) MessageBox(hWnd, "Failed to save default config.", "Err", MB_ICONEXCLAMATION|MB_OK); }
void ReadSettingsOrComplain(HWND hWnd) { if (ReadSettings()) MessageBox(hWnd, "Problem reading existing config.", "Err", MB_ICONEXCLAMATION|MB_OK); }

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

HINSTANCE hInst = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{	
	hInst = hInstance;
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
			HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(TEST_ICON));
			SendMessage(hWnd, WM_SETICON, WPARAM(TRUE), LPARAM(hIcon));
			hIcon = LoadIcon(hInst, MAKEINTRESOURCE(TEST_ICO2));
			SendMessage(hWnd, WM_SETICON, WPARAM(FALSE), LPARAM(hIcon));
			
			LoadDefaults();
			
			FILE *conf = fopen(VERSUSE_STRING_CONFIG, "r");
			if (conf == NULL) SaveSettingsOrComplain(hWnd);
			else ReadSettingsOrComplain(hWnd);
			fclose(conf);
			
			DisplaySettings(hWnd);
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
					SaveSettingsOrComplain(hWnd);
					WriteTextOrComplain(hWnd);
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

void LoadDefaults() {
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

void DisplaySettings(HWND hWnd) {
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