#ifndef days_h
#define days_h
#include "events.h"
#include "events.cpp"

#define _maxdirective 13

char *empt="<empty>";
char curfname[128];

char *directive[_maxdirective]={" ",
                                "#if ",
                                "#ifdef ",
                                "#ifndef ",
                                "#else",
                                "#endif",
                                "#define ",
                                "#include ",
                                "#eval ",
                                "#defeventtype ",
                                "#event ",
                                "#language",
                                "#paramstr"};

#define _NONE 0
#define _IF 1
#define _IFDEF 2
#define _IFNDEF 3
#define _ELSE 4
#define _ENDIF 5
#define _DEFINE 6
#define _INCLUDE 7
#define _EVAL 8
#define _EVENTT 9
#define _EVENT 10
#define _LANGUAGE 11
#define _PARAMSTR 12

char *(monthstr[13])={"", "������", "�������", "�����", "������", "���", "����",
              "����", "�������", "��������", "�������", "������", "�������"};

#ifndef _when
#define _when
char *(when[5])={"���������", "�����", "�������", "������", "�����������"};
#endif

char *(weekday[8])={"� �����������", "� �����������", "�� �������", "� �����", "� �������", "� �������", "� �������", "� �����������"};

TeventsT eventT;

Tevents event;

Tglobvar globvars;

#endif