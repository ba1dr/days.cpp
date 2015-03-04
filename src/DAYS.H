#ifndef days_h
#define days_h
#include "events.h"
#include "events.cpp"

#define _maxdirective 14

char *empt="<empty>";
int up=-1;
fpos_t *fpos[30];
char curfname[128][30];

char *directive[_maxdirective]={" ",
                                "#defeventtype ",
                                "#event ",
                                "#language",
                                "#paramstr",
                                "#if ",
                                "#ifdef ",
                                "#ifndef ",
                                "#else",
                                "#endif",
                                "#define ",
                                "#include ",
                                "#eval ",
                                "#ID"};

#define _NONE 0
#define _EVENTT 1
#define _EVENT 2
#define _LANGUAGE 3
#define _PARAMSTR 4
#define _IF 5
#define _IFDEF 6
#define _IFNDEF 7
#define _ELSE 8
#define _ENDIF 9
#define _DEFINE 10
#define _INCLUDE 11
#define _EVAL 12
#define _ID 13

char *(monthstr[13])={"", "Ÿ‚€Ÿ", "”…‚€‹Ÿ", "Œ€’€", "€…‹Ÿ", "Œ€Ÿ", "ˆŸ",
              "ˆ‹Ÿ", "€‚ƒ“‘’€", "‘…’ŸŸ", "Š’ŸŸ", "ŸŸ", "„…Š€Ÿ"};

#ifndef _when
#define _when
char *(when[5])={"‡€‚—…€", "‚—…€", "‘…ƒ„Ÿ", "‡€‚’€", "‘‹…‡€‚’€"};
#endif

char *(weekday[8])={"‚ ‚‘Š…‘…œ…", "‚ …„…‹œˆŠ", "‚ ‚’ˆŠ", "‚ ‘…„“", "‚ —…’‚…ƒ", "‚ Ÿ’ˆ–“", "‚ ‘“’“", "‚ ‚‘Š…‘…œ…"};

char passw[]={15, 24, 51, 42, 99, 61, 3, 5, 88, 17, 205, 254, 26,
              76, 19, 127, 13, 13, 13, 45, 31, 59, 7, 88, 41, 90,
              27, 34, 177, 200, 15, 59, 33, 2, 24, 199, 46, 14, 115,
              16, 78, 52, 79, 3, 43, 12, 178, 220, 250, 219, 7, 190,
              34, 238, 64, 129, 31, 174, 46, 12, 26, 79, 9, 10, 82};

TeventsT eventT;

Tevents event;

Tglobvar globvars;

#endif