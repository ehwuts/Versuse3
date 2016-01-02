#ifndef VERSUSE_MAIN_H
#define VERSUSE_MAIN_H

/*
* in the land of defines everyone is sad
*/
#define VERSUSE_STRING_VERSION "v2.0"
#define VERSUSE_RES_VERSION 2,0,0,0
#define VERSUSE_RES_VERSION2 "2.0"

#define VERSUSE_MAIN 150
#define VERSUSE_ICON 151
//#define VERSUSE_ICO2 152

#define VERSUSE_EDIT_LEFTNAME 111
#define VERSUSE_EDIT_LEFTSCORE 113
#define VERSUSE_EDIT_RIGHTNAME 112
#define VERSUSE_EDIT_RIGHTSCORE 114
#define VERSUSE_BUTTON_WRITE 101

#define VERSUSE_OPTION_LEFT_L 121
#define VERSUSE_OPTION_LEFT_C 122
#define VERSUSE_OPTION_LEFT_R 123

#define VERSUSE_OPTION_RIGHT_L 124
#define VERSUSE_OPTION_RIGHT_C 125
#define VERSUSE_OPTION_RIGHT_R 126

#define VERSUSE_STRING_CONFIG "versuse-conf.txt"
#define VERSUSE_STRING_CONFIG_FORMAT "%s\n%d %d %d %d\n%s\n%s\n%s\n%s\nThings to avoid to prevent unexpected behaviour:\noutput filename length above 25 characters.\nname length above 40 characters\nscores more than 3 characters"
#define VERSUSE_LIST_CONFIG_VARS_SET outfile, outw, alignL, alignR, mono, leftname, leftscore, rightname, rightscore

#endif