#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>

#include "versuse.hpp"
#include "versuse-main.hpp"

int MaxNum(int a, int b) { return (a > b ? a : b); }
int MinNum(int a, int b) { return (a < b ? a : b); }

void WriteSaveOrComplain(HWND hWnd) { if (WriteSave()) MessageBox(hWnd, "Failed to save default config.", "Err", MB_ICONEXCLAMATION|MB_OK); }
void ReadSaveOrComplain(HWND hWnd) {
	if (int i = ReadSave()) MessageBox(hWnd, (i<0?"Problem reading existing config.":"Problem after partially reading config."), "Err", MB_ICONEXCLAMATION|MB_OK);
}
void WriteTextOrComplain(HWND hWnd) { if (WriteText()) MessageBox(hWnd, "Failed to save display text.", "Welp", MB_OK|MB_ICONERROR); }

// GLOBALS EVERYWHERE GLOBALS
std::string outfile, leftname, leftscore, rightname, rightscore;
int outw, alignL, alignR;

HINSTANCE hInst = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{	
	hInst = hInstance;
	HWND hDiag = 0;
	
	hDiag = CreateDialog(hInst, MAKEINTRESOURCE(VERSUSE_MAIN), 0, (DLGPROC)DialogProc);
	
	if (hDiag == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error! aaaaaaa!", MB_ICONEXCLAMATION|MB_OK);
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
			HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(VERSUSE_ICON));
			SendMessage(hWnd, WM_SETICON, WPARAM(TRUE), LPARAM(hIcon));
			//hIcon = LoadIcon(hInst, MAKEINTRESOURCE(VERSUSE_ICO2));
			hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(VERSUSE_ICON), IMAGE_ICON, 16, 16, 0);
			SendMessage(hWnd, WM_SETICON, WPARAM(FALSE), LPARAM(hIcon));
			SendMessage(hWnd, DM_SETDEFID, VERSUSE_BUTTON_WRITE, 0);
			
			LoadDefaults();
			
			std::fstream fs (VERSUSE_STRING_CONFIG, std::ifstream::in);
			if (fs.good()) ReadSaveOrComplain(hWnd);
			else WriteSaveOrComplain(hWnd);
			fs.close();
			
			WriteDisplay(hWnd);
		}
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case VERSUSE_BUTTON_WRITE:
					ReadDisplay(hWnd);
					WriteSaveOrComplain(hWnd);
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

void LoadDefaults() {
	outfile = "versuse-out.txt";
	outw = 69;
	alignL = 2;
	alignR = 2;
	leftname = "";
	leftscore = "0";
	rightname = "";
	rightscore = "0";
}

//this function is what everything else is wrapping, and it is a sloppy mess
int WriteText() {
	int n = outw;
	int leftA = alignL;
	int rightA = alignR;
	int leftW = leftname.size();
	int rightW = rightname.size();
	char* out = (char*)malloc(sizeof(char) * (n + 1));
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
	
	loc = center - leftscore.size();
	j = (i = 0);
	while (leftscore[i] != '\0') out[loc + j++] = leftscore[i++];
	
	loc = center + 1 + MaxNum(leftscore.size(), rightscore.size()) - rightscore.size();
	j = (i = 0);
	while (rightscore[i] != '\0') out[loc + j++] = rightscore[i++];
	
	out[center] = '-';
	out[n] = '\0';
	
	FILE *fp = fopen(outfile.c_str(), "w");
	if (fp == NULL) return 1;
	else {
		fprintf(fp, "%s", out);
		fclose(fp);
	}
	free(out);
	return 0;
}

void WriteDisplay(HWND hWnd) {
	int t = VERSUSE_OPTION_LEFT_C;
	if (alignL == 1) t = VERSUSE_OPTION_LEFT_L;
	if (alignL == 3) t = VERSUSE_OPTION_LEFT_R;
	SendMessage(GetDlgItem(hWnd, t), BM_SETCHECK, BST_CHECKED, 0);
	t = VERSUSE_OPTION_RIGHT_C;
	if (alignR == 1) t = VERSUSE_OPTION_RIGHT_L;
	if (alignR == 3) t = VERSUSE_OPTION_RIGHT_R;
	SendMessage(GetDlgItem(hWnd, t), BM_SETCHECK, BST_CHECKED, 0);
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTNAME), leftname.c_str());
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTSCORE), leftscore.c_str());
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTNAME), rightname.c_str());
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTSCORE), rightscore.c_str());
	
	
	std::stringstream ss;
	ss << outw;
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_TEXTWIDTH), ss.str().c_str());
}

void ReadWndToStr(HWND ref, std::string* dest) {
	int buflen = GetWindowTextLength(ref) + 1;
	(*dest).resize(buflen);
	GetWindowText(ref, &((*dest)[0]), buflen);
	(*dest).resize(buflen - 1);
}

void ReadDisplay(HWND hWnd) {
	if (IsDlgButtonChecked(hWnd, VERSUSE_OPTION_LEFT_L)) alignL = 1;
	else if (IsDlgButtonChecked(hWnd, VERSUSE_OPTION_LEFT_R)) alignL = 3;
	else alignL = 2;
	if (IsDlgButtonChecked(hWnd, VERSUSE_OPTION_RIGHT_L)) alignR = 1;
	else if (IsDlgButtonChecked(hWnd, VERSUSE_OPTION_RIGHT_R)) alignR = 3;
	else alignR = 2;
		
	ReadWndToStr(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTNAME), &leftname);
	ReadWndToStr(GetDlgItem(hWnd, VERSUSE_EDIT_LEFTSCORE), &leftscore);
	ReadWndToStr(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTNAME), &rightname);
	ReadWndToStr(GetDlgItem(hWnd, VERSUSE_EDIT_RIGHTSCORE), &rightscore);
	
	std::string buf;
	ReadWndToStr(GetDlgItem(hWnd, VERSUSE_EDIT_TEXTWIDTH), &buf);
	std::stringstream buf2;
	buf2 << buf;
	buf2 >> outw;
}

int WriteSave() {
	std::fstream fs (VERSUSE_STRING_CONFIG, std::fstream::out);
	if (!fs) return -1;
	
	fs << outfile << "\n" 
	   << outw << " " << alignL << " " << alignR << "\n"
	   << leftname << "\n"
	   << leftscore << "\n"
	   << rightname << "\n"
	   << rightscore << std::endl;
	   
	fs.close();
	return 0;
}

int ReadSave() {
	std::fstream fs (VERSUSE_STRING_CONFIG, std::fstream::in);
	if (!fs) return -1;
	
	std::string line;
	
	if (!std::getline(fs, line)) return 1;
	outfile = line;
	if (!std::getline(fs, line)) return 3;
	int i;
	if ((i = sscanf(line.c_str(), "%d %d %d\n", &outw, &alignL, &alignR)) < 3) return 4;
	if (!std::getline(fs, line)) return 5;
	leftname = line;
	if (!std::getline(fs, line)) return 7;
	if (line.length() == 0) line = "0";
	leftscore = line;
	if (!std::getline(fs, line)) return 9;
	rightname = line;
	if (!std::getline(fs, line)) return 11;
	if (line.length() == 0) line = "0";
	rightscore = line;
	
	return 0;
}
