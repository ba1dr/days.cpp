//include-файл с описаниями классов событий и их типов
#ifndef events_cpp
#define events_cpp
#include "days.cpp"
#include "events.h"

#ifndef _when
#define _when
char *(when[5])={"ПОЗАВЧЕРА", "ВЧЕРА", "СЕГОДНЯ", "ЗАВТРА", "ПОСЛЕЗАВТРА"};
#endif

//--------------------------------------------------------------------------
//Функции-элементы для класса ТИПА события.

TeventT::TeventT()
{
  strcpy(name,"");
  int i=0;
  for(i=0; i<max_TEvarN; i++) varname[i]=0;
  for(i=0; i<max_TEvarN; i++) varvalue[i]=0;
  varN=-1;
  setvar("color", "7");
  setvar("bkcolor", "0");
  setvar("daysback", "2");
  setvar("daysfrw", "3");
  setvar("freq", "0");
  setvar("delay", "0");
  setvar("format", "@eventstr@");
}

int TeventT::isvar(const char *name)
{
  char name_[51];
  strcpy(name_, name);
  if (strlen(name_)>50) name_[49]=0;
  for(int i=0; i<=varN; i++) if (strcmp(name_, varname[i])==0) return i;
  return -1;
}

int TeventT::setvar(const char *name, const char *value)
{
  int nm=-1;
  char name_[51], value_[301];
  strcpy(name_, name);
  if (strlen(name_)>50) name_[49]=0;
  strcpy(value_, value);
  if (strlen(value_)>300) value_[299]=0;
  int nl=strlen(name_),
      vl=strlen(value_);
  if ((nm=isvar(name_))==-1)
  {
    if (varN>=max_TEvarN) return 0;
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
    cnout<<" Error: not enough memory (00x"<<varN<<")\n";
    exit(1);
  }
  strcpy(varname[nm], name_);
  strcpy(varvalue[nm], value_);
  return 1;
}

TeventT::~TeventT()
{
  for(int i=0; i<=varN; i++)
  {
    delete varname[i];
    delete varvalue[i];
  }
  varN=-1;
}

//--------------------------------------------------------------------------
//Функции-элементы для класса ТИПОВ событий.

TeventsT::TeventsT()
{
  for(int i=0; i<max_TeventsN; i++) eventTV[i]=0;
  eventTN=-1;
}

int TeventsT::iseventT(const char *name)
{
  for(int i=0; i<=eventTN; i++) if (strcmp(name, eventTV[i]->name)==0) return i;
  return -1;
}

int TeventsT::isvar(const int eTname, const char *name)
{
  if (eTname<0||eTname>eventTN) return -1;
  return eventTV[eTname]->isvar(name);
}

int TeventsT::addeventT(const char *name)
{
  //if (eventTN==-1||iseventT(name)!=-1) return 0;
  char name_[51];
  strcpy(name_, name);
  if (eventTN>=max_TeventsN) return 0;
  if (iseventT(name)!=-1) return 0;
  eventTN++;
  int err=(eventTV[eventTN]=new TeventT)==0;
  if (err)
  {
    cnout<<" Error: not enough memory (01x"<<eventTN<<")\n";
    exit(1);
  }
  if (strlen(name_)>50) name_[49]=0;
  strcpy(eventTV[eventTN]->name, name_);
  return 1;
}

int TeventsT::setvar(const int eTname, const char *name, const char *value)
{
  if (eTname<0||eTname>eventTN) return -1;
  return eventTV[eTname]->setvar(name, value);
}

void TeventsT::getvar(const eTname, const char *name, char *value)
{
  if (eTname<0||eTname>eventTN) return;
  int num;
  if ((num=isvar(eTname, name))==-1) return;
  strcpy(value, eventTV[eTname]->varvalue[num]);
}

TeventsT::~TeventsT()
{
  for(int i=0; i<=eventTN; i++) delete eventTV[i];
  eventTN=-1;
}

//--------------------------------------------------------------------------
//Функции-элементы для класса события.

Tevent::Tevent()
{
//  typenum=-1;
  int i=0;
  for(i=0; i<max_EvarN; i++) varname[i]=0;
  for(i=0; i<max_EvarN; i++) varvalue[i]=0;
  varN=-1;
  setvar("color", "7");
  setvar("bkcolor", "0");
  setvar("daysback", "2");
  setvar("daysfrw", "3");
  setvar("eventstr", "");
  setvar("eventwhen", "");
  setvar("eventdate", "");
  setvar("weekdate", "");
  setvar("was", "");
  setvar("format", "@eventstr@");
  Time.tm_sec=0;
  Time.tm_min=0;
  Time.tm_hour=0;
  Time.tm_mday=1;
  Time.tm_mon=0;
  Time.tm_year=0;
  Time.tm_wday=0;
  Time.tm_yday=0;
  Time.tm_isdst=0;
}

void Tevent::dateset(int day, int month, int year)
{
  if (day>0&&day<31) Time.tm_mday=day;
  else Time.tm_mday=curdate.day();
  if (month>0&&month<=12) Time.tm_mon=month;
  else Time.tm_mon=curdate.month();
  if (year>0) Time.tm_year=year;
  else Time.tm_year=curdate.year()-1900;
}

int Tevent::isvar(const char *name)
{
  char name_[51];
  strcpy(name_, name);
  if (strlen(name_)>50) name_[49]=0;
  for(int i=0; i<=varN; i++) if (strcmp(name_, varname[i])==0) return i;
  return -1;
}

int Tevent::setvar(const char *name, const char *value)
{
  char name_[51], value_[301];
  strcpy(name_, name);
  if (strlen(name_)>50) name_[49]=0;
  strcpy(value_, value);
  if (strlen(value_)>300) value_[299]=0;
  int nl=strlen(name_),
      vl=strlen(value_);
  int nm=-1;
  if ((nm=isvar(name_))==-1)
  {
    if (varN>max_EvarN) return 0;
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

char *Tevent::getvar(const int name, char *value)
{
  if (name>=0&&name>7)//Нельзя получить напрямую стандартные переменные
  {
    strcpy(value, varvalue[name]);
    return value;
  }
  return 0;
}

int Tevent::color()
{
  int i=-1;
  if ((i=isvar("color"))!=-1) return atoi(varvalue[i]);
  return 7;
}

int Tevent::bkcolor()
{
  int i=-1;
  if ((i=isvar("bkcolor"))!=-1) return atoi(varvalue[i]);
  return 0;
}

int Tevent::daysback()
{
  int i=-1;
  if ((i=isvar("daysback"))!=-1) return atoi(varvalue[i]);
  return 2;
}

int Tevent::daysfrw()
{
  int i=-1;
  if ((i=isvar("daysfrw"))!=-1) return atoi(varvalue[i]);
  return 3;
}

char *Tevent::outstr(char *endstr)
{     //К этому времени никаких define-"констант" быть не должно!!
  char strk[500], st[500];
  strcpy(strk, "\x0");
  strcpy(st, varvalue[isvar("format")]);
  char *cPtr1=st, *cPtr2;
  if ((cPtr2=strpbrk(cPtr1, "@"))!=0||*cPtr1=='@')
  {
    while ((cPtr2=strpbrk(cPtr1, "@"))!=0||*cPtr1=='@')
    {
      if (*cPtr1=='@') cPtr2=cPtr1;
      cPtr2=strpbrk(cPtr1, "@\x0");
      *cPtr2=0;
      cPtr2++;
      strcat(strk, cPtr1);
      cPtr1=strpbrk(cPtr2, "@\x0");
      *cPtr1=0;
      cPtr1++;
      int nm=isvar(cPtr2);
      if (nm>=0)
      {
        strcat(strk, "\"");
        strcat(strk, varvalue[nm]);
        strcat(strk, "\"");
      }
    }
    strcat(strk, cPtr1);
    cPtr2=strk;
  } else cPtr2=st;
  cPtr1=endstr;
  int cp=0;
  for(; *cPtr2; cPtr2++)
  {
    if (*cPtr2=='\"')
    {
      cp=!cp;
      continue;
    }
    if (cp)
    {
      *cPtr1=*cPtr2;
      cPtr1++;
    }
  }
  *cPtr1=0;
  return endstr;
}

Tevent::~Tevent()
{
  for(int i=0; i<=varN; i++)
  {
    delete varname[i];
    delete varvalue[i];
  }
  varN=-1;
}

//--------------------------------------------------------------------------
//Функции-элементы для класса событий.

Tevents::Tevents()
{
  for(int i=0; i<max_eventsN; i++) eventV[i]=0;
  eventN=-1;
}

int Tevents::isvar(const int ename, const char *name)
{
  if (ename<0||ename>eventN) return 0;
  return eventV[ename]->isvar(name);
}

int Tevents::setvar(const int ename, const char *name, const char *value)
{
  if (ename<0||ename>eventN) return 0;
  return eventV[ename]->setvar(name, value);
}

char *Tevents::getvar(const int ename, const int name, char *value)
{
  if (ename<0||ename>eventN) return 0;
  eventV[ename]->getvar(name, value);
  return value;
}

void Tevents::dateset(const int ename, int day, int month, int year)
{
  if (ename<0||ename>eventN) return;
  eventV[ename]->dateset(day, month, year);
}

int Tevents::addevent(const TeventsT *eT, const int NeventT)
{
  if (NeventT<0||NeventT>eT->eventTN) return 0;
  if (eventN>=max_eventsN) return 0;
  eventN++;
  int err=(eventV[eventN]=new Tevent)==0;
  if (err)
  {
    cnout<<" Error: not enough memory (02x"<<eventN<<")\n";
    exit(1);
  }
  for(int i=0; i<=eT->eventTV[NeventT]->varN; i++) setvar(eventN, eT->eventTV[NeventT]->varname[i], eT->eventTV[NeventT]->varvalue[i]);
  return 1;
}

int Tevents::color(const int ename)
{
  if (ename<0||ename>eventN) return 0;
  return eventV[ename]->color();
}

int Tevents::bkcolor(const int ename)
{
  if (ename<0||ename>eventN) return 0;
  return eventV[ename]->bkcolor();
}

int Tevents::daysback(const int ename)
{
  if (ename<0||ename>eventN) return 0;
  return eventV[ename]->daysback();
}

int Tevents::daysfrw(const int ename)
{
  if (ename<0||ename>eventN) return 0;
  return eventV[ename]->daysfrw();
}

char *Tevents::outstr(const int ename, char *endstr)
{
  if (ename<0||ename>eventN) return 0;
  eventV[ename]->outstr(endstr);
  return endstr;
}

int Tevents::eventC() { return eventN; }

/*void Tevents::sortevents()
{
  int evdat[max_eventsN]={0};
  for(int i=0; i<=eventN; i++)
  { //Здесь мы только пересчитываем обратно eventwhen!!!
    char strk1[100];
    int j=isvar(i, "was"), was=0;
    getvar(i, j, strk1);
    if (strcmp(strk1, "БЫЛ")==0) was=-1; else
      if (strcmp(strk1, "БУДЕТ")==0) was=1;
    if (was==-1)
    {
      j=isvar(i, "eventwhen");
      getvar(i, j, strk1);
      if (strcmp(strk1, when[0])==0) { evdat[i]=-2; continue; }
      if (strcmp(strk1, when[1])==0) { evdat[i]=-1; continue; }
      j=0;
      char st[2]="\x0\x0";
      st[0]=strk1[j];
      while (strcspn("0123456789", st)<10) {j++; st[0]=strk1[j]; }
      char strks[5];
      strncpy(strks, strk1, j);
      evdat[i]=atoi(strks);
    }else
    if (was==1)
    {
      j=isvar(i, "eventwhen");
      getvar(i, j, strk1);
      if (strcmp(strk1, when[3])==0) { evdat[i]=1; continue; }
      if (strcmp(strk1, when[4])==0) { evdat[i]=2; continue; }
      j=6;
      char st[2]="\x0\x0";
      st[0]=strk1[j];
      while (strcspn("0123456789", st)<10) {j++; st[0]=strk1[j]; }
      char strks[5];
      strncpy(strks, strk1+6, j-6);
      evdat[i]=atoi(strks);
    } else evdat[i]=0;
  }
  int t, j;
  Tevent *Te;
  for(i=1; i<=eventN; i++)
  {
    t=evdat[i];
    Te=eventV[i];
    for(j=i-1; j>=0&&evdat[j]>t; j--)
    {
      evdat[j+1]=evdat[j];
      eventV[j+1]=eventV[j];
    }
    evdat[j+1]=t;
    eventV[j+1]=Te;
  }
} */

Tevents::~Tevents()
{
  for(int i=0; i<=eventN; i++) delete eventV[i];
  eventN=-1;
}

#endif