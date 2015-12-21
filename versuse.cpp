#include <windows.h>

/*
* this blob of defines makes me sad but defining things as local
* constants broke more than I was up for fixing pre-function
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
#define VERSUSE_STRING_VERSION "v0.3"
#define VERSUSE_STRING_L "L"
#define VERSUSE_STRING_C "C"
#define VERSUSE_STRING_R "R"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
			CW_USEDEFAULT, 200, 312, 124,
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
			HWND init;
			init = CreateWindowEx(WS_EX_CLIENTEDGE, 
								"EDIT", "", 
								WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
								4, 4, 168, 24,
								hWnd, (HMENU)VERSUSE_EDIT_LEFTNAME,
								GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(WS_EX_CLIENTEDGE, 
								"EDIT", "", 
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
			init = CreateWindowEx(NULL, 
								"BUTTON", "", 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
								254, 4, 24, 24,
								hWnd, (HMENU)VERSUSE_OPTION_LEFT_C,
								GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
								"BUTTON", "", 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
								276, 4, 24, 24,
								hWnd, (HMENU)VERSUSE_OPTION_LEFT_R,
								GetModuleHandle(NULL), NULL);
								
			init = CreateWindowEx(WS_EX_CLIENTEDGE, 
								"EDIT", "", 
								WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
								4, 32, 168, 24,
								hWnd, (HMENU)VERSUSE_EDIT_RIGHTNAME,
								GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(WS_EX_CLIENTEDGE, 
								"EDIT", "", 
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
			init = CreateWindowEx(NULL, 
								"BUTTON", "", 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
								254, 32, 24, 24,
								hWnd, (HMENU)VERSUSE_OPTION_RIGHT_C,
								GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
								"BUTTON", "", 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
								276, 32, 24, 24,
								hWnd, (HMENU)VERSUSE_OPTION_RIGHT_R,
								GetModuleHandle(NULL), NULL);
								
			init = CreateWindowEx(NULL, 
								"BUTTON", "Write", 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
								4, 60, 100, 24,
								hWnd, (HMENU)VERSUSE_BUTTON_WRITE,
								GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
								"STATIC", VERSUSE_STRING_VERSION, 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
								184, 64, 36, 24,
								hWnd, (HMENU)VERSUSE_STATIC_VERSION,
								GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
								"STATIC", VERSUSE_STRING_L, 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
								226, 64, 24, 24,
								hWnd, (HMENU)VERSUSE_STATIC_L,
								GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
								"STATIC", VERSUSE_STRING_C, 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
								248, 64, 24, 24,
								hWnd, (HMENU)VERSUSE_STATIC_C,
								GetModuleHandle(NULL), NULL);
			init = CreateWindowEx(NULL, 
								"STATIC", VERSUSE_STRING_R, 
								WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
								270, 64, 24, 24,
								hWnd, (HMENU)VERSUSE_STATIC_R,
								GetModuleHandle(NULL), NULL);
		}
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case VERSUSE_BUTTON_WRITE:
					
				break;
				default:
					acted = false;				
			}
		break;
		/*case WM_RBUTTONDOWN:
			MessageBox(hWnd, "v0.1", "Hi", MB_OK);
		break;*/
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