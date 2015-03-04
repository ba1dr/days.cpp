#ifndef dat_cpp
#define dat_cpp
#include "dat.h"
//--------------------------------------------------------------------------
//Функции-элементы для класса текущей даты.

Tcurdate::Tcurdate()
{
  time_t timet;
  time(&timet);
  Date=*localtime(&timet);
}

int Tcurdate::day() { return Date.tm_mday; }

void Tcurdate::strday(char *invar) { itoa(Date.tm_mday, invar, 10); }

void Tcurdate::strmonth(char *invar) { itoa(Date.tm_mon+1, invar, 10); }

void Tcurdate::stryear(char *invar) { itoa(Date.tm_year+1900, invar, 10); }

void Tcurdate::strwday(char *invar)
{
  int i=Date.tm_wday;
  if (i==0) i=7;
  itoa(i, invar, 10);
}

int Tcurdate::month() { return Date.tm_mon+1; }

int Tcurdate::year() { return Date.tm_year+1900; }

int Tcurdate::wday() { int i=Date.tm_wday; if (i==0) i=7; return i; }

int Tcurdate::chour() { return Date.tm_hour; }

void Tcurdate::strhour(char *invar) { itoa(Date.tm_hour, invar, 10); }

int Tcurdate::cmin() { return Date.tm_min; }

void Tcurdate::strmin(char *invar) { itoa(Date.tm_min, invar, 10); }

int Tcurdate::csec() { return Date.tm_sec; }

void Tcurdate::strsec(char *invar) { itoa(Date.tm_sec, invar, 10); }


Tcurdate::~Tcurdate() { /*Пустой деструктор!*/ }

//--------------------------------------------------------------------------
//Функции-элементы для класса глобальных переменных.

Tglobvar::Tglobvar()
{
  int i=0;
  for(i=0; i<max_glvarsN; i++) varname[i]=0;
  for(i=0; i<max_glvarsN; i++) varvalue[i]=0;
  varN=-1;
}

int Tglobvar::isvar(const char *name)
{
  for(int i=0; i<=varN; i++) if (strcmp(name, varname[i])==0) return i;
  return -1;
}

int Tglobvar::setvar(const char *name, const char *value)
{
  int nm=-1;
  char name_[51], value_[101];
  strcpy(name_, name);
  strcpy(value_, value);
  if (strlen(name_)>50) name_[49]=0;
  if (strlen(value_)>100) value_[99]=0;
  int nl=strlen(name_),
      vl=strlen(value_);
  if ((nm=isvar(name_))==-1)
  {
    if (varN>max_glvarsN) return 0;
    varN++;
    nm=varN;
  } else
  {
    delete varname[nm];
    delete varvalue[nm];
  }
  int err1=(varname[nm]=new char[nl+1])==0;
  int err2=(varvalue[nm]=new char[vl+1])==0;
  if (err1||err2)
  {
    cnout<<" Error: not enough memory (02x"<<varN<<")\n";
    exit(1);
  }
  strcpy(varname[nm], name_);
  strcpy(varvalue[nm], value_);
  return 1;
}

char *Tglobvar::getvar(const char *name, char *value)
{
  int i=-1;
  if ((i=isvar(name))!=-1)
  {
    strcpy(value, varvalue[i]);
    return value;
  }
  return 0;
}

Tglobvar::~Tglobvar()
{
  for(int i=0; i<=varN; i++)
  {
    delete varname[i];
    delete varvalue[i];
  }
  varN=-1;
}

Tcurdate curdate;

#endif