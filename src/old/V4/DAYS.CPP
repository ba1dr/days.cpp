// Days v4.0 (-0.1)
#include <conio.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include <time.h>
#include "myunit.cpp"
// Константы
#define _curversion "v4.00"
#define vio " Company \"Do\"   Колянов Алексей   v4.0  26.05.2001 г.           "
//#define _daysbefore 3
//#define _beforedef "3"
#define _colordef "7"
#define _bkcolordef "0"
#define _EVTN 50
#include "days1.cpp"

// Прототипы
FILE *tryfileopen(char *, int *);
void freeallmem(int *, char *[], int *, tglvars *[], int *, TeventT *[], int *, Tevent *[]);
int fillst(FILE *, char *, char *, char *[], int *, int );
void helper();           //help screen
void exiter();       //Если что-либо выводилось, выдаем копирайт
void sortSt(int *, char *[]);
void readGLVars(const int, const int, int *, char *[], int *, const char [], tglvars *[]); //Выбор глобальных переменных
int endblock(int , const int , const char *[]);
void eventtyperead(int *, char *, int *, char *[], int *, TeventT *[], int *, tglvars *[]);
int eventgood(Tevent);
void eventread(int *, char *, int *, char *[], int *, TeventT *[], int *, tglvars *[], int *, Tevent *[]);
void analyzeBlock(int *, int *, char *[], int *, TeventT *[], int *, tglvars *[], int *, Tevent *[]); //Анализ списка из файла
void setGLvars(int *, tglvars *[]);
void datestr(char *, struct tm *);
void whenstr(char *, struct tm *, const struct tm);
void outhello(struct tm);
void addeventvar(Tevent *, char [], char *);
void outevents(int, Tevent *[]);
void freest(int *, char *[]);
void freeglvar(int *, tglvars *[]);
void freeeventT(int *, TeventT *[]);
void freeevent(int *, Tevent *[]);
void genpause();

// Начало!
void main(int argc, char *argv[])
{
  options.before=3;
  options.music=1;
  options.pasthol=1;
  options.pause=0;
  options.was=false;
  options.nohello=0;
  options.onerun=0;
  char path[130];
  strcpy(path, argv[0]);
  int i=strlen(path);
  for(; path[i]!='\\'; i--) path[i]=0;
  strcpy(options.begpath, path);
  char strk[500]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  if (argc<1)
  {
    helper();
    exiter();
    return;
  } else
    for(int i=1; i<=argc; i++)
    {
      strcpy(strk, argv[i]);
      for(int j=0; strk[j]; j++) strk[j]=chup(strk[j]);
      if (strk[0]=='/') for(int j=0; strk[j]; j++) strk[j]=strk[j+1];
      if (strk[0]=='?') {
                          helper();
                          exiter();
                          return;
                         } else
      if (strk[0]=='\\') {
                           for(int j=0; strk[j]; j++) strk[j]=strk[j+1];
                           strcpy(path,strk);
                         }else
      if (strk[0]=='N'&&strk[1]=='M') options.music=false; else
      if (strk[0]=='P') options.pause=atoi(strk+1); else
      if (strk[0]=='N'&&strk[1]=='V') options.pasthol=0; else
      if (strk[0]=='D') options.before=atoi(strk+1); else
      if (strk[0]=='S') options.onerun=1;
    }
  i=strlen(path);
  if (!i) strcpy(path,"DAYS.TXD\x0");
  if (path[i-1]=='\\') strcat(path,"DAYS.TXD\x0");
/*  struct ffblk ffblk;
  i=findfirst(path,&ffblk,0);
  if (i)
  {
    cputs(" Error: file not found...\r\n");
    return;
  }*/
  //char olddir[130]="\x0\x0\x0";
  options.olddir[0]=getdisk()+'a';
  getcurdir(0,strk);
  strcat(options.olddir,":\\");
  strcat(options.olddir,strk);
  char drive[4], dir[128], fulldir[128], name[9], ext[5];
  fnsplit(path, drive, dir, name, ext);
  strcpy(fulldir,drive);
  strcat(fulldir,dir);
  chdir(fulldir);
  int datef;
  FILE *dayf=tryfileopen(path, &datef);
  time_t timet;
  time(&timet);
  struct tm *tmt=localtime(&timet);
  if (options.onerun)
  {
    int dyear=(datef&65024)>>9;
    int dmon=(datef&4064)>>9;
    int dday=datef&31;
    if ((tmt->tm_year==dyear)&&(tmt->tm_mon+1==dmon)&&(tmt->tm_mday==dday))
    {
      fclose(dayf);
      exiter();
      chdir(options.olddir);
      exit(0);
    }
  }
  if (!dayf)
  {
    cputs(" Error: unable to open file...\r\n");
    chdir(options.olddir);
    return;
  }
  char *st[1000];
  int stC=-1;
  i=0;
  if (fillst(dayf, path, fulldir, st, &stC, i)==1) return; //чтение файла...
  struct ftime timefil;
  timefil.ft_year=tmt->tm_year;
  timefil.ft_month=tmt->tm_mon+1;
  timefil.ft_day=tmt->tm_mday;
  timefil.ft_tsec=tmt->tm_sec/2;
  timefil.ft_min=tmt->tm_min;
  timefil.ft_hour=tmt->tm_hour;
  setftime(fileno(dayf), &timefil);
  fclose(dayf);
  int eventTN=-1, eventN=-1, globvarsN=-1;
  TeventT *eventT[_EVTN];              //Тип события
  Tevent *event[50];               //События, которые НАДО выводить
  tglvars *globvar[50];           //Глобальные переменные
  setGLvars(&globvarsN, globvar);
  readGLVars(0, stC, &stC, st, &globvarsN, _DEFGLOB, globvar); //Ищем все глобальные переменные
  i=0;
  analyzeBlock(&i, &stC, st, &eventTN, eventT, &globvarsN, globvar, &eventN, event);
  //Сформированы события - можно и выводить!
  outevents(eventN, event);
//  options.was=eventN;
  //Выход
  freest(&stC, st);
  freeglvar(&globvarsN, globvar);
  freeeventT(&eventTN, eventT);
  freeevent(&eventN, event);
  exiter();
  chdir(options.olddir);
}

#include "days2.cpp"