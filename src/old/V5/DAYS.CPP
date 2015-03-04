#ifndef days_cpp
#define days_cpp
#define cr "  Company \"Do\"  Колянов Алексей  Days v5.2 from 14.06.01"
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include <constrea.h>
#include <dos.h>
#include <dir.h>
#include <stdio.h>
#include <conio.h>
#include "myunit.cpp"
#include "dat.h"
#include "dat.cpp"
#include "events.h"
#include "events.cpp"
#include "days.h"

void genpause();
void helper();
void exiter();
int ifmanagst(char *);
void extract(char *);
void skipblock(FILE *, char *, int *, int *);
void paramstrget(const char *);
int eval(const char *, char *);
int evaluate(const char *, char *);
void process(FILE *, int, int *);
int readfile(char *, int *);
int readTevent(char *, int);
char *getone(char *, int *);
int dateok(int, int, int, int, int *);
void datestrmake(char *, int, int);
void whenstrmake(char *, int);
int readevent(char *, int);
void outevents();

void main(int argc, char *argv[])
{
  cnout<<" ";
  gotoxy(wherex()-1, wherey());
  options.daysback=0;
  options.daysfrw=0;
  options.pause=3;
  options.sort=0;
  options.onerun=0;
  globvars.setvar("daysversion", "5.17");
  globvars.setvar("eventcount", "0");
  char path[130];
  strcpy(path, argv[0]);
  int i=strlen(path);
  for(; path[i]!='\\'; i--) path[i]=0;
  strcpy(options.begpath, path);
  char strk[200];
  curdate.strday(strk);
  globvars.setvar("day", strk);
  curdate.strmonth(strk);
  globvars.setvar("month", strk);
  curdate.stryear(strk);
  globvars.setvar("year", strk);
  curdate.strwday(strk);
  globvars.setvar("wday", strk);
  curdate.strhour(strk);
  globvars.setvar("hour", strk);
  curdate.strmin(strk);
  globvars.setvar("min", strk);
  curdate.strsec(strk);
  globvars.setvar("sec", strk);
  for(i=1; i<argc; i++)
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
    if (strcmp(strk, "PR")==0) options.pause=-1; else
    if (strcmp(strk, "SORT")==0) options.sort=1; else
    if (strk[0]=='P') options.pause=atoi(strk+1); else
    if (strk[0]=='B') options.daysback=atoi(strk+1); else
    if (strk[0]=='F') options.daysfrw=atoi(strk+1); else
    if (strk[0]=='1') options.onerun=1;
  }
  i=strlen(path);
  if (!i) strcpy(path,"DAYS.TXD\x0");
  if (path[i-1]=='\\') strcat(path,"DAYS.TXD\x0");
  //Здесь - проверка на ключ "/1"
  if (options.onerun)
  {
    struct ffblk ffb;
    findfirst(path, &ffb, 0);
    options.datef=ffb.ff_fdate;
    int dyear=((options.datef&65024)>>9)+1980;
    int dmon=((options.datef&4064)>>9)+1;
    int dday=options.datef&31;
    if ((curdate.year()==dyear)&&(curdate.month()==dmon)&&(curdate.day()==dday))
    {
      exiter();
      chdir(options.olddir);
      exit(0);
    }
  }
  options.olddir[0]=getdisk()+'a';
  getcurdir(0,strk);
  strcat(options.olddir,":\\");
  strcat(options.olddir,strk);
  char drive[4], dir[128], fulldir[128], name[9], ext[5];
  fnsplit(path, drive, dir, name, ext);
  strcpy(fulldir,drive);
  strcat(fulldir,dir);
  chdir(fulldir);
  i=0;
  if (!readfile(path, &i))
  {
    cnout<<"  Global error... Exit...\n";
    chdir(options.olddir);
    exit(1);
  }
  outevents();
//Выход...
  FILE *fp=fopen(path, "r");
  struct ftime timefil;
  timefil.ft_year=curdate.year()-1980;
  timefil.ft_month=curdate.month();
  timefil.ft_day=curdate.day();
  timefil.ft_tsec=curdate.csec()/2;
  timefil.ft_min=curdate.cmin();
  timefil.ft_hour=curdate.chour();
  setftime(fileno(fp), &timefil);
  fclose(fp);
  exiter();
  chdir(options.olddir);
}

#include "work.cpp"
#endif