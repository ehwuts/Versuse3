#ifndef VERSUSE_H

#define VERSUSE_H

/*
* this blob of defines makes me sad but defining things as local
* constants broke more than I was up for fixing pre-functionality
*/
#define VERSUSE_STRING_VERSION "v1.1"
#define VERSUSE_STRING_VERSION2 "1.1"
#define VERSUSE_STRING_VERSION3 1,1,0,0
#define VERSUSE_STRING_PVERSION "1.0"
#define VERSUSE_STRING_PVERSION2 1,0,0,0

#define VERSUSE_ICON 151
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
#define VERSUSE_STRING_L "L"
#define VERSUSE_STRING_C "C"
#define VERSUSE_STRING_R "R"
#define VERSUSE_STRING_CONFIG "versuse-conf.txt"
#define VERSUSE_STRING_CONFIG_FORMAT "%s\n%d %d %d %d\n%s\n%s\n%s\n%s\nThings to avoid to prevent unexpected behaviour:\noutput filename length above 25 characters.\nname length above 40 characters\nscores more than 3 characters"
#define VERSUSE_LIST_CONFIG_VARS_SET outfile, outw, alignL, alignR, mono, leftname, leftscore, rightname, rightscore

#endif