#ifndef VERSUSE_MAIN_H
#define VERSUSE_MAIN_H

BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void LoadDefaults();
int WriteSave();
int ReadSave();
int WriteText();
void WriteDisplay(HWND);
void ReadDisplay(HWND);

int MaxNum(int, int);
int MinNum(int, int);

#endif