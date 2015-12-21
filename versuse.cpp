#include <windows.h>
#include <commctrl.h>
#include <cstdio>

/*
* this blob of defines makes me sad but defining things as local
* constants broke more than I was up for fixing pre-functionality
*/
#define VERSUSE_BUTTON_WRITE 101
#define VERSUSE_EDIT_LEFTNAME 111
#define VERSUSE_EDIT_LEFTSCORE 113
#define VERSUSE_EDIT_RIGHTNAME 112
#define VERSUSE_EDIT_RIGHTSCORE 114
#define VERSUSE_OPTION_LEFT_L 121
#define VERSUSE_OPTION_LEFT_C 122
#define VERSUSE_OPTION_LEFT_R 123
#define VERSUSE_OPTION_RIGHT_L 124
#define VERSUSE_OPTION_RIGHT_C 125
#define VERSUSE_OPTION_RIGHT_R 126
#define VERSUSE_STATIC_VERSION 131
#define VERSUSE_STATIC_L 131
#define VERSUSE_STATIC_C 132
#define VERSUSE_STATIC_R 133
#define VERSUSE_STATIC_SLIDER 134
#define VERSUSE_TRACKBAR_WIDTH 141
#define VERSUSE_STRING_VERSION "v0.6"
#define VERSUSE_STRING_L "L"
#define VERSUSE_STRING_C "C"
#define VERSUSE_STRING_R "R"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void UpdateWidthDisplay(HWND hWnd);
int SaveSettings();

enum Align { LEFT = 1, CENTER = 2, RIGHT = 3 };

// GLOBALS EVERYWHERE GLOBALS
Align alignL = CENTER;
Align alignR = CENTER;
short outw = 68;
char leftname[41] = "";
char rightname[41] = "";
char outfile[26] = "versuse-conf.txt";
short leftscore = 0;
short rightscore = 0;

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
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm       = NULL;
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
			FILE *conf = fopen("versuse.conf", "r");
			if (conf == NULL) {
				//try write defaults
				if (SaveSettings()) MessageBox(hWnd, "Failed to save default config.", "Err", MB_ICONEXCLAMATION | MB_OK);
			} else {
				//read in saved values
				
				fclose(conf);
			}
			
			CreateWindowEx(WS_EX_CLIENTEDGE, 
							"EDIT", "", 
							WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
							4, 4, 168, 24,
							hWnd, (HMENU)VERSUSE_EDIT_LEFTNAME,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(WS_EX_CLIENTEDGE, 
							"EDIT", "", 
							WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
							184, 4, 36, 24,
							hWnd, (HMENU)VERSUSE_EDIT_LEFTSCORE,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON|WS_GROUP,
							232, 4, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_LEFT_L,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
							254, 4, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_LEFT_C,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
							276, 4, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_LEFT_R,
							GetModuleHandle(NULL), NULL);
								
			CreateWindowEx(WS_EX_CLIENTEDGE, 
							"EDIT", "", 
							WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
							4, 32, 168, 24,
							hWnd, (HMENU)VERSUSE_EDIT_RIGHTNAME,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(WS_EX_CLIENTEDGE, 
							"EDIT", "", 
							WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
							184, 32, 36, 24,
							hWnd, (HMENU)VERSUSE_EDIT_RIGHTSCORE,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON|WS_GROUP,
							232, 32, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_RIGHT_L,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
							254, 32, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_RIGHT_C,
							GetModuleHandle(NULL), NULL);
			CreateWindowEx(NULL, 
							"BUTTON", "", 
							WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
							276, 32, 24, 24,
							hWnd, (HMENU)VERSUSE_OPTION_RIGHT_R,
							GetModuleHandle(NULL), NULL);
								
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
			HWND trackRef = CreateWindowEx(NULL, 
							"msctls_trackbar32", "Output Width", 
							WS_VISIBLE|WS_CHILD|TBS_AUTOTICKS|TBS_ENABLESELRANGE,
							2, 88, 292, 12,
							hWnd, (HMENU)VERSUSE_TRACKBAR_WIDTH,
							GetModuleHandle(NULL), NULL);
			SendMessage(trackRef, TBM_SETRANGE, true, MAKELONG(51,161));
			SendMessage(trackRef, TBM_SETSEL, true, MAKELONG(68,100));
			SendMessage(trackRef, TBM_SETPAGESIZE, 0,  2); 
			SendMessage(trackRef, TBM_SETTICFREQ, 10, 0); 
			SendMessage(trackRef, TBM_SETPOS, true, outw); 
			UpdateWidthDisplay(hWnd);
		}
		break;
		case WM_HSCROLL:
			UpdateWidthDisplay(hWnd);
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case VERSUSE_BUTTON_WRITE:
					MessageBox(hWnd, "This isn't implemented yet :(", "Hi", MB_OK);
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

/*
Align alignL = CENTER;
Align alignR = CENTER;
short outW = 68;
std::string leftname = "";
std::string rightname = "";
short rightscore = 0;
*/
int SaveSettings() {
	FILE *fd = fopen("versuse.conf", "w");
	if (fd == NULL) return -1;
	fprintf(fd, "%s\n%d %d %d\n%s\n%d\n%s\n%d\nMax supported output filename length is 25 characters.\nMax supported name length is 40 characters.", outfile, outw, alignL, alignR, leftname, leftscore, rightname, rightscore);
	fclose(fd);
	return 0;
}