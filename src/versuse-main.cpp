#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "versuse.hpp"
#include "versuse-main.hpp"

int MaxNum(int a, int b) { return (a > b ? a : b); }
int MinNum(int a, int b) { return (a < b ? a : b); }

void WriteSaveOrComplain(HWND hWnd) { if (WriteSave()) MessageBox(hWnd, "Failed to save default config.", "Err", MB_ICONEXCLAMATION|MB_OK); }
void ReadSaveOrComplain(HWND hWnd) {
	if (int i = ReadSave()) MessageBox(hWnd, (i<0?"No brackets in file. Loaded defaults.":"Problem after partially reading config."), "Err", MB_ICONEXCLAMATION|MB_OK);
}
void WriteTextOrComplain(HWND hWnd) { if (WriteText()) MessageBox(hWnd, "Failed to save display text.", "Welp", MB_OK|MB_ICONERROR); }
void WriteBracketOrComplain(HWND hWnd) { if (WriteBracket()) MessageBox(hWnd, "Failed to save bracket text.", "Rip", MB_OK|MB_ICONERROR); }

// GLOBALS EVERYWHERE GLOBALS
std::string outfile, outfile2, leftname, leftscore, rightname, rightscore, brackconf, nameconf;
int outw, alignL, alignR, mono;

int bracksel, playersel, playersel2, scoresel, scoresel2;

std::vector< std::string > brackets, players, scores;

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
			
			{
				std::fstream fs (VERSUSE_STRING_CONFIG, std::ifstream::in);
				if (fs.good()) ReadSaveOrComplain(hWnd);
				else WriteSaveOrComplain(hWnd);
				fs.close();
			}
			{
				std::fstream fs (brackconf.c_str(), std::ifstream::in);
				if (fs.good()) {
					std::string line;
					while (std::getline(fs, line)) brackets.push_back(line);					
				}
				if (brackets.size() == 0) {
					DefaultBrackets();
					WriteBracketFile();
				}
				fs.close();	

				if (bracksel < 0) bracksel = 0;
				if ((unsigned)bracksel >= brackets.size()) bracksel = brackets.size() - 1;
			}
			{
				std::fstream fs (nameconf.c_str(), std::ifstream::in);
				if (fs.good()) {
					std::string line;
					while (std::getline(fs, line)) players.push_back(line);					
				}
				if (players.size() < 2) WritePlayerFile();
				fs.close();			
				
				if (playersel < 0) playersel = 0;
				if (playersel2 < 0) playersel2 = 0;
				if ((unsigned)playersel >= players.size()) playersel = players.size() - 1;
				if ((unsigned)playersel2 >= players.size()) playersel2 = players.size() - 1;
			}
			
			WriteDisplay(hWnd);
			DisplayBrackets(GetDlgItem(hWnd, VERSUSE_CBOX_BRACKET));
			DisplayNames(GetDlgItem(hWnd, VERSUSE_CBOX_LEFTNAME), GetDlgItem(hWnd, VERSUSE_CBOX_RIGHTNAME));
			DisplayScores(GetDlgItem(hWnd, VERSUSE_CBOX_LEFTSCORE), GetDlgItem(hWnd, VERSUSE_CBOX_RIGHTSCORE));
		}
		break;
		case WM_COMMAND:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				switch(LOWORD(wParam)) {
					case VERSUSE_CBOX_LEFTNAME:
						playersel = SendMessage(GetDlgItem(hWnd, VERSUSE_CBOX_LEFTNAME), CB_GETCURSEL, 0, 0);
						leftname = players[playersel];
					break;
					case VERSUSE_CBOX_LEFTSCORE:
						scoresel = SendMessage(GetDlgItem(hWnd, VERSUSE_CBOX_LEFTSCORE), CB_GETCURSEL, 0, 0);
						leftscore = scores[scoresel];
					break;
					case VERSUSE_CBOX_RIGHTNAME:
						playersel2 = SendMessage(GetDlgItem(hWnd, VERSUSE_CBOX_RIGHTNAME), CB_GETCURSEL, 0, 0);
						rightname = players[playersel2];
					break;
					case VERSUSE_CBOX_RIGHTSCORE:
						scoresel2 = SendMessage(GetDlgItem(hWnd, VERSUSE_CBOX_RIGHTSCORE), CB_GETCURSEL, 0, 0);
						rightscore = scores[scoresel2];
					break;
					default:
						acted = false;
				}
				WriteDisplay(hWnd);
			} else switch(LOWORD(wParam)) {
				case VERSUSE_BUTTON_WRITE:
					ReadDisplay(hWnd);
					WriteSaveOrComplain(hWnd);
					WriteTextOrComplain(hWnd);
					WriteBracketOrComplain(hWnd);
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

void DefaultNumbers() {
	scores.erase(brackets.end(), brackets.begin());
	scores.push_back("0");
	scores.push_back("1");
	scores.push_back("2");
	scores.push_back("3");
	scores.push_back("4");
	scores.push_back("5");
	scoresel = 0;
	scoresel2 = 0;
}

void DefaultNames() {
	players.erase(brackets.end(), brackets.begin());
	players.push_back("");
	playersel = 0;
	playersel2 = 0;
}

void LoadDefaults() {
	outfile = "versuse-out.txt";
	outw = 69;
	alignL = 2;
	alignR = 2;
	mono = 0;
	leftname = "";
	leftscore = "0";
	rightname = "";
	rightscore = "0";
	outfile2 = "versuse-brackout.txt";
	brackconf = "versuse-brackconf.txt";
	nameconf = "versuse-names.txt";
	DefaultNames();
	DefaultNumbers();
}

void DefaultBrackets() {
	brackets.erase(brackets.end(), brackets.begin());
	brackets.push_back("Winners Bracket");
	brackets.push_back("Losers Bracket");
	brackets.push_back("Winners Quarters");
	brackets.push_back("Losers Quarters");
	brackets.push_back("Winners Semis");
	brackets.push_back("Losers Semis");
	brackets.push_back("Winners Semis");
	brackets.push_back("Losers Semis");
	brackets.push_back("Semifinals");
	brackets.push_back("Grand Finals");
	bracksel = 0;
}

int WriteBracket() {
	std::fstream fs (outfile2.c_str(), std::fstream::out);
	if (!fs) return -1;
	
	fs << brackets[bracksel] << " ";
	fs.flush();
	
	fs.close();
	return 0;
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
	
	loc = center - (mono?0:1)  - leftscore.size();
	j = (i = 0);
	while (leftscore[i] != '\0') out[loc + j++] = leftscore[i++];
	
	loc = center + (mono?1:2) + MaxNum(leftscore.size(), rightscore.size()) - rightscore.size();
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
		/*
	std::stringstream ss;
	ss << outw;
	SetWindowText(GetDlgItem(hWnd, VERSUSE_EDIT_TEXTWIDTH), ss.str().c_str());
	*/
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
	/*
	std::string buf;
	ReadWndToStr(GetDlgItem(hWnd, VERSUSE_EDIT_TEXTWIDTH), &buf);
	std::stringstream buf2;
	buf2 << buf;
	buf2 >> outw;
	*/
	bracksel = SendMessage(GetDlgItem(hWnd, VERSUSE_CBOX_BRACKET), CB_GETCURSEL, 0, 0);
	/*
	int buflen = SendMessage(GetDlgItem(hWnd, VERSUSE_CBOX_BRACKET), CB_GETLBTEXTLEN, dropsel, 0) + 1;
	buf.resize(buflen);
	SendMessage(GetDlgItem(hWnd, VERSUSE_CBOX_BRACKET), CB_GETLBTEXT, dropsel, (LPARAM)&(buf[0]));
	buf.resize(buflen - 1);
	brackets[dropsel] = buf;
	*/
}

int WriteBracketFile() {
	std::fstream fs (brackconf.c_str(), std::fstream::out);
	if (!fs) return -1;
	
	std::vector< std::string >::iterator it = brackets.begin();
	while (it != brackets.end()) {
		fs << (*it) << "\n";
		++it;
	}
	
	fs.flush();	
	
	fs.close();
	return 0;
}
int WritePlayerFile() {
	std::fstream fs (nameconf.c_str(), std::fstream::out);
	if (!fs) return -1;
	
	fs << std::endl;
	
	fs.close();
	return 0;
}

int WriteSave() {
	std::fstream fs (VERSUSE_STRING_CONFIG, std::fstream::out);
	if (!fs) return -1;
	
	fs << outw << " " << alignL << " " << alignR << " " << bracksel << " " << playersel << " " << scoresel << " " << playersel2 << " " << scoresel2 << " " << mono << "\n"
	   << leftname << "\n"
	   << leftscore << "\n"
	   << rightname << "\n"
	   << rightscore << "\n"
	   << outfile << "\n" 
	   << outfile2 << "\n"
	   << brackconf << "\n"
	   << nameconf << std::endl;
	   
	return 0;
}

void DisplayBrackets(HWND ref) {
       std::vector< std::string >::iterator it = brackets.begin();
       while (it != brackets.end()) {
               SendMessage(ref, (UINT)CB_ADDSTRING, (WPARAM)0,(LPARAM)((*it).c_str()));
               ++it;
       }
       SendMessage(ref, CB_SETCURSEL, bracksel, 0);// != dropsel)MessageBox(NULL, "BRACKET A.", "Rip", MB_OK|MB_ICONERROR);
}
void DisplayNames(HWND ref, HWND ref2) {
       std::vector< std::string >::iterator it = players.begin();
       while (it != players.end()) {
               SendMessage(ref, (UINT)CB_ADDSTRING, (WPARAM)0,(LPARAM)((*it).c_str()));
               SendMessage(ref2, (UINT)CB_ADDSTRING, (WPARAM)0,(LPARAM)((*it).c_str()));
               ++it;
       }
       SendMessage(ref, CB_SETCURSEL, playersel, 0);
       SendMessage(ref2, CB_SETCURSEL, playersel2, 0);
}
void DisplayScores(HWND ref, HWND ref2) {
       std::vector< std::string >::iterator it = scores.begin();
       while (it != scores.end()) {
               SendMessage(ref, (UINT)CB_ADDSTRING, (WPARAM)0,(LPARAM)((*it).c_str()));
               SendMessage(ref2, (UINT)CB_ADDSTRING, (WPARAM)0,(LPARAM)((*it).c_str()));
               ++it;
       }
       SendMessage(ref, CB_SETCURSEL, scoresel, 0);
       SendMessage(ref2, CB_SETCURSEL, scoresel2, 0);
}

int ReadSave() {
	std::fstream fs (VERSUSE_STRING_CONFIG, std::fstream::in);
	if (!fs) return -1;
	
	std::string line;
	
	if (!std::getline(fs, line)) return 3;
	int i;
	if ((i = sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d\n", &outw, &alignL, &alignR, &bracksel, &playersel, &scoresel, &playersel2, &scoresel2, &mono)) < 3) return 4;
	switch(i) {
		case 3: bracksel = 0;
		case 4:	playersel = 0;
		case 5:	scoresel = 0;
		case 6:	playersel2 = 0;
		case 7:	scoresel2 = 0;
		case 8: mono = 0;
	}
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
	if (!std::getline(fs, line)) return 1;
	outfile = line;
	if (!std::getline(fs, line)) return 13;
	outfile2 = line;
	if (!std::getline(fs, line)) return 15;
	brackconf = line;
	if (!std::getline(fs, line)) return 17;
	nameconf = line;
	
	return 0;
}
