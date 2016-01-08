#ifndef VERSUSE_MAIN_H
#define VERSUSE_MAIN_H

BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void LoadDefaults();
void DefaultBrackets();
int WriteSave();
int WriteBracketFile();
int WritePlayerFile();
int ReadSave();
int WriteText();
int WriteBracket();
void WriteDisplay(HWND);
void ReadDisplay(HWND);
void DisplayBrackets(HWND);
void DisplayNames(HWND, HWND);
void DisplayScores(HWND, HWND);

int MaxNum(int, int);
int MinNum(int, int);

#endif