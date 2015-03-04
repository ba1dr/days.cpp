#ifndef work_cpp
#define work_cpp
#include "days.cpp"
#include "days.h"

void genpause()
{
  int was;
  char ws[10];
  globvars.getvar("eventcount", ws);
  was=atoi(ws);
  for(long ps=1; ps<=options.pause*was*100; ps++)
  {
    if (kbhit()) if (chup(getch())=='P')
                 {
                   wait();
                   getch();
                   break;
                 } else break;
    delay(10);
  }
}

void helper()
{
  cnout<<" ";
  textattr(14);
  cnout<<" Программа для напоминания важных дат.\n";
  textattr(12);
  cnout<<" Для работы Программы требуется текстовый файл (См. документацию.)\n";
  textattr(10);
  cnout<<"    Ключи Программы :\n";
  textattr(9);
  cnout<<"     /? ";
  textattr(11);
  cnout<<"      -  Эта справка.\n";
  textattr(9);
  cnout<<"     /F<n> ";
  textattr(11);
  cnout<<"   -  За сколько дней вперед предупреждать (умлч. 3).\n";
  textattr(9);
  cnout<<"     /B<n> ";
  textattr(11);
  cnout<<"   -  За сколько дней выводить прошедшие праздники (умлч. 2).\n";
  textattr(9);
  cnout<<"     /P<n> ";
  textattr(11);
  cnout<<"   -  Длительность паузы.(\"/PR\" - для ожидания)\n";
  textattr(9);
  cnout<<"     \\<Path> ";
  textattr(11);
  cnout<<" -  Путь к файлам с датами.\n";
  textattr(9);
  cnout<<"     /1 ";
  textattr(11);
  cnout<<"      -  Запускать Программу не более 1 раза в сутки.\n";
  textattr(7);
  cnout<<"\n";
  globvars.setvar("eventcount", "1");
  return;
}

void exiter()   //Если что-либо выводилось, выдаем копирайт
{
  int was;
  char ws[10];
  globvars.getvar("eventcount", ws);
  was=atoi(ws);
  if (was>0)
  {
    wait();
    textattr(7);
    gotoxy(1, wherey()-1);
    cnout<<"\n";
    gotoxy(1, wherey()-1);
    if (options.pause==-1)
    {
      cnout<<"  --Пауза-- (нажми любую клавишу...)\r";
      getch();
      cnout<<"                                       \r";
      wait();
    } else genpause();
    textattr(5);
    cnout<<cr;
    textattr(7);
    cnout<<"\n";
    wait();
  };
  return;
}

int ifmanagst(char *strk)
{
  for(int i=1; i<_maxdirective; i++)
    if (strstr(strk, directive[i])!=0)
      return i;
  return 0;
}

void extract(char *strk)//Удаляем комментарии и лишние пробелы с концов строки
{
  char *cPtr1;
  cPtr1=strpbrk(strk, ";");
  if (cPtr1) *cPtr1=0;
  cPtr1=strk+strlen(strk)-1;
  while (*cPtr1==' '||*cPtr1=='\n')
  {
    *cPtr1=0;
    cPtr1--;
  }
  for(int i=0; strk[i]==' '; i++);
  for(int j=0; j<=strlen(strk)-i; j++) strk[j]=strk[j+i];
}

int readfile(char *filename, int *asdf)
{
  struct ffblk ffb;
  if (findfirst(filename, &ffb, 0)!=0)
  {
    cnout<<"  Error: unable to find file \""<<filename<<"\"\n";
    return 0;
  }  //Сюда вставить выбор рабочего каталога!!!!!!!!!
  FILE *fp=fopen(filename, "r");
  strcpy(curfname, filename);
  int exiter=0;
  if feof(fp) exiter=1;
  char strk[300];
  strcpy(strk, "");
  while (!exiter&&!strpbrk(strk, "{"))
  {
    fgets(strk, 300, fp);
    if feof(fp) exiter=1;
  }
  if (!exiter) process(fp, 0, asdf);
  fclose(fp);
  return 1;
}

void skipblock(FILE *fp, char *strk, int *j, int *exiter)
{
  int k=0;
  int deeper=1;
  while (k!=*j&&k!=_ENDIF&&!*exiter)
  {
    fgets(strk, 300, fp);
    if (feof(fp)) *exiter=1;
    extract(strk);
    if (strlen(strk)==0) continue;
    k=ifmanagst(strk);
    if (k==_IF||k==_IFDEF||k==_IFNDEF)
    {
      deeper++;
      k=0;
    }else
    if ((k==_ENDIF||k==*j)&&deeper!=1)
    {
      if (k==_ENDIF) deeper--;
      k=0;
    }
  }
  if (k==_ENDIF) *j=0;
  else *j=1;
}

void paramstrget(const char *st)
{
  char strk[100], *stlen, *cPtr1, *cPtr2;
  strcpy(strk, st);
  cPtr1=strk;
  for(int j=0; *(cPtr1+j); j++) *(cPtr1+j)=chup(*(cPtr1+j));
  cPtr1+=strlen(directive[_PARAMSTR]);
  while (*cPtr1&&*cPtr1!='=') cPtr1++;
  if (!*cPtr1) return;
  cPtr1++;
  cPtr2=cPtr1;
  stlen=cPtr1+strlen(cPtr1);
  for(; cPtr1<stlen; cPtr1++) if (*cPtr1==' '||*cPtr1=='/') *cPtr1=0;
  cPtr1=cPtr2;
  while(cPtr1<stlen)
  {
    if (strcmp(cPtr1, "PR")==0) options.pause=-1; else
    if (strcmp(strk, "SORT")==0) options.sort=1; else
    if (*cPtr1=='P') options.pause=atoi(cPtr1+1); else
    if (*cPtr1=='B') options.daysback=atoi(cPtr1+1); else
    if (*cPtr1=='F') options.daysfrw=atoi(cPtr1+1); else
    if (*cPtr1=='1') options.onerun=1;
    while (*cPtr1) cPtr1++;
    while (!(*cPtr1)) cPtr1++;
  }
  if (options.onerun)
  {
    int dyear=(options.datef&65024)>>9;
    int dmon=(options.datef&4064)>>9;
    int dday=options.datef&31;
    if ((curdate.year()==dyear)&&(curdate.month()==dmon)&&(curdate.day()==dday))
    {
      exiter();
      chdir(options.olddir);
      exit(0);
    }
  }
}

int eval(const char *from, char *to)
{
  char strk[200];
  //сначала - в скобках рекурсивно, затем - умножение/деление,
  //затем - сложение/вычитание - все в циклах
  char *cPtr1, *cPtr2;
  cPtr1=strcpy(strk, from);
  int stlen=strlen(strk);
  while ((cPtr1=strpbrk(cPtr1, "("))!=0)
  {
    *cPtr1=0;
    cPtr1++;
    char brckt[200];
    int deeper=1, i=0;
    while (cPtr1[i]&&deeper>0)//Определяем границы скобок
    {
      if (cPtr1[i]=='(') deeper++;else
        if (cPtr1[i]==')') deeper--;
      i++;
    }
    cPtr2=cPtr1+i-1;
    *cPtr2=0;
    if (cPtr2<stlen+strk) cPtr2++;
    if (!eval(cPtr1, brckt)) return 0;  //Рекурсивно считаем значение для скобок...
    char strk1[200];
    strcpy(strk1, strk);
    strcat(strk1, brckt);
    strcat(strk1, cPtr2);
    cPtr1=strcpy(strk, strk1);
  }
  cPtr1=strk;
  while ((cPtr1=strpbrk(cPtr1, "*/"))!=0) //Здесь уже скобок быть не должно!
  {
    if ((cPtr2=cPtr1+1)>=stlen+strk) return 0;
    char operation=*cPtr1, *cPtr3=cPtr2, spt[50], sp[2];
    *cPtr1=0;
    cPtr1--;
    sp[1]=0;
    sp[0]=*cPtr1;
    while ((strcspn("0123456789", sp)<10)&&cPtr1>strk)
    {
      cPtr1--;
      sp[0]=*cPtr1;
    }
    if (cPtr1>strk) cPtr1++;
    sp[0]=*cPtr3;
    while ((strcspn("0123456789", sp)<10)&&cPtr3<=stlen+strk)
    {
      cPtr3++;
      sp[0]=*cPtr3;
    }
    strncpy(spt, cPtr2, cPtr3-cPtr2+1);
    spt[cPtr3-cPtr2]=0;
    double mul, operand1, operand2;
    operand1=_atold(cPtr1);
    operand2=_atold(cPtr2);
    if (operand1&&operand2)
    {
      if (operation=='*') mul=operand1*operand2;else
        if (operation=='/') mul=operand1/operand2;
    } else mul=0;
    char str1[30];
    gcvt(mul, 30, str1);
    char strk1[200];
    *cPtr1=0;
    strcpy(strk1, strk);
    strcat(strk1, str1);
    strcat(strk1, cPtr3);
    cPtr1=strcpy(strk, strk1);
  }
  cPtr1=strk; //Здесь уже скобок и умножения с делением быть не должно!
  while ((cPtr1=strpbrk(cPtr1, "+-"))!=0)
  {
    if ((cPtr2=cPtr1+1)>=stlen+strk) return 0;
    char operation=*cPtr1, *cPtr3=cPtr2, spt[50], sp[2];
    *cPtr1=0;
    cPtr1--;
    sp[1]=0;
    sp[0]=*cPtr1;
    while ((strcspn("0123456789", sp)<10)&&cPtr1>strk)
    {
      cPtr1--;
      sp[0]=*cPtr1;
    }
    if (cPtr1>strk) cPtr1++;
    sp[0]=*cPtr3;
    while ((strcspn("0123456789", sp)<10)&&cPtr3<=stlen+strk)
    {
      cPtr3++;
      sp[0]=*cPtr3;
    }
    strncpy(spt, cPtr2, cPtr3-cPtr2+1);
    spt[cPtr3-cPtr2]=0;
    double sum, operand1, operand2;
    operand1=_atold(cPtr1);
    operand2=_atold(cPtr2);
    if (operation=='+') sum=operand1+operand2;else
      if (operation=='-') sum=operand1-operand2;
    char str1[30];
    gcvt(sum, 30, str1);
    char strk1[200];
    *cPtr1=0;
    strcpy(strk1, strk);
    strcat(strk1, str1);
    strcat(strk1, cPtr3);
    cPtr1=strcpy(strk, strk1);
  }
  strcpy(to, strk);
  return 1;
}

int evaluate(const char *from, char *to)
{
  char *cPtr1, *cPtr2, strk[200];
  cPtr1=strcpy(strk, from);
  if (strspn(cPtr1, "0123456789()+-/*")<strlen(cPtr1)) return 0;
  if (!eval(strk, strk)) return 0;
  strcpy(to, strk);
  return 1;
}

int ifeq(char *strk1, int evif)
{
  char strk[300];
  char *cPtr1=strcpy(strk, strk1);
  char *cPtr2=strpbrk(cPtr1, "<>!=\x0");
  char operation[3];
  *operation=*cPtr2;
  operation[1]=0;
  if (*(cPtr2+1)=='=')
  {
    strcat(operation, "=");
    *cPtr2=0;
    cPtr2++;
  }
  *cPtr2=0;
  cPtr2++;
  int exist1=globvars.isvar(cPtr1), //Если evif>-1, то - и из событий тоже!!!
      exist2=globvars.isvar(cPtr2); //Добавить!!
  char stt1[100], stt2[100];
  if (exist1!=-1) { globvars.getvar(cPtr1, stt1);cPtr1=stt1; }
  if (exist2!=-1) { globvars.getvar(cPtr2, stt2);cPtr2=stt2; }
  int equal;
  if (operation[0]=='=') equal=(strcmp(cPtr1, cPtr2)==0); else
    if (operation[0]=='!') equal=(strcmp(cPtr1, cPtr2)!=0); else
      if (strcmp(operation, ">")==0) equal=(strcmp(cPtr1, cPtr2)>0); else
        if (strcmp(operation, ">=")==0) equal=(strcmp(cPtr1, cPtr2)>=0); else
          if (strcmp(operation, "<")==0) equal=(strcmp(cPtr1, cPtr2)<0); else
            if (strcmp(operation, "<=")==0) equal=(strcmp(cPtr1, cPtr2)<=0);
  return equal;
}

int eqEx(char *strk1, char *strk2, int evif)
{
  char strk[300], *cPtr1, *cPtr2;
  cPtr1=strcpy(strk, strk1);
  int stlen=strlen(strk);
  while ((cPtr1=strpbrk(cPtr1, "("))!=0)
  {
    *cPtr1=0;
    cPtr1++;
    char brckt[200];
    int deeper=1, i=0;
    while (cPtr1[i]&&deeper>0)//Определяем границы скобок
    {
      if (cPtr1[i]=='(') deeper++;else
        if (cPtr1[i]==')') deeper--;
      i++;
    }
    cPtr2=cPtr1+i-1;
    if (*cPtr2==')') *cPtr2=0; else *(cPtr2+1)=0;
    if (cPtr2<stlen+strk) cPtr2++;
    char st[300], steq[2]="\x0\x0", stt[300];
    if (eqEx(cPtr1, st, evif)) steq[0]='1';
    else steq[0]='0';
    strcpy(stt, strk);
    strcat(stt, st);
    strcat(stt, cPtr2);
    cPtr1=strcpy(strk, stt);
  }
  if (strpbrk(strk, "!=<>")!=0)
  {
    int i;
    if ((i=ifeq(strk, evif))==1) strcpy(strk2, "1"); else strcpy(strk2, "0");
    return i;
  }
  cPtr1=strk;
  while ((cPtr1=strstr(cPtr1, "and"))!=0)
  {
    cPtr2=cPtr1-1;
    while (*cPtr2!='0'&&*cPtr2!='1'&&cPtr2>strk) cPtr2--;
    char operand1, operand2, st[2]="0\x0", stt[300];
    operand1=*cPtr2;
    *cPtr2=0;
    cPtr2=cPtr1+3;
    while (*cPtr2!='0'&&*cPtr2!='1'&&*cPtr2) cPtr2++;
    operand2=*cPtr2;
    *cPtr2=0;
    cPtr2++;
    if (operand1=='1'&&operand2=='1') st[0]='1';
    else st[0]='0';
    strcpy(stt, strk);
    strcat(stt, st);
    strcat(stt, cPtr2);
    cPtr1=strcpy(strk, stt);
  }
  cPtr1=strk;
  while ((cPtr1=strstr(cPtr1, "or"))!=0)
  {
    cPtr2=cPtr1-1;
    while (*cPtr2!='0'&&*cPtr2!='1'&&cPtr2>strk) cPtr2--;
    char operand1, operand2, st[2]="0\x0", stt[300];
    operand1=*cPtr2;
    *cPtr2=0;
    cPtr2=cPtr1+2;
    while (*cPtr2!='0'&&*cPtr2!='1'&&*cPtr2) cPtr2++;
    operand2=*cPtr2;
    *cPtr2=0;
    cPtr2++;
    if (operand1=='1'||operand2=='1') st[0]='1';
    else st[0]='0';
    strcpy(stt, strk);
    strcat(stt, st);
    strcat(stt, cPtr2);
    cPtr1=strcpy(strk, stt);
  }
  extract(strk);
  strcpy(strk2, strk);
  return strk[0]=='1';
}

void process(FILE *fp, int blocknum, int *evORtev)
{
  int exiter=0;
  if feof(fp) exiter=1;
  char strk[300];
  while (!exiter)
  {
    fgets(strk, 300, fp);
    if (feof(fp)) exiter=1;
    extract(strk);
    if (strlen(strk)==0) continue;
    char *cPtr1=strchr(strk, '$');
    //Вставляем, если необходимо, define-переменные.
    while (cPtr1!=0)
    {
      char *stlen=strk+strlen(strk);
      *cPtr1=0;
      cPtr1++;
      char *cPtr2=strpbrk(cPtr1, "$\x0");
      *cPtr2=0;
      if (cPtr2<stlen) cPtr2++;
      char st1[50], st2[150];
      strcpy(st2, cPtr2);
      if (globvars.isvar(cPtr1)) strcat(strk, globvars.getvar(cPtr1, st1));
      strcat(strk, st2);
      cPtr1=strchr(strk, '$');
    }
    cPtr1=strk;
    int i=ifmanagst(cPtr1);
    if (i==_PARAMSTR)
    {
      paramstrget(strk);
      continue;
    }
    if (i<9&&i>0) //#-директива!
    {
      cPtr1=strpbrk(strk, "#");
      cPtr1+=strlen(directive[i]);
      while (*cPtr1==' ') cPtr1++;
      if (i==_IF)  //Встречается незаблокированное условие...
      {
        int equal=0, j=0;
        equal=eqEx(cPtr1, strk, -1);
        if (equal) //Если переменная существует и эквивалентна операцией значению...
        {
          process(fp, 1, evORtev);//
          continue;
        }else if (!equal) j=_ELSE;//Существует и не равна...
              else j=_ENDIF; //Не существует...
        skipblock(fp, strk, &j, &exiter);
        if (j==_ELSE) process(fp, 2, evORtev);
      }else
      if (i==_IFDEF)
      {
        int exist=globvars.isvar(cPtr1);
        if (exist!=-1)
        {
          process(fp, 1, evORtev);
          continue;
        }
        int j=_ELSE;
        skipblock(fp, strk, &j, &exiter);
        if (j) process(fp, 2, evORtev);
      }else
      if (i==_IFNDEF)
      {
        int exist=globvars.isvar(cPtr1);
        if (exist==-1)
        {
          process(fp, 1, evORtev);
          continue;
        }
        int j=_ELSE;
        skipblock(fp, strk, &j, &exiter);
        if (j) process(fp, 2, evORtev);
      }else
      if (i==_ELSE)
      {
        if (blocknum==1)
        {
          int j=_ENDIF;
          skipblock(fp, strk, &j, &exiter);
          return;
        }
      }else
      if (i==_ENDIF)
      {
        if (blocknum>0) return;
      }else
      if (i==_DEFINE)
      {
        char *cPtr2=strpbrk(cPtr1, " =\x0");
        if (cPtr2)
        {
          *cPtr2=0;
          cPtr2++;
        }else cPtr2=empt;
        strcpy(strk, cPtr1);
        globvars.setvar(strk, cPtr2);
      }else
      if (i==_INCLUDE)
      {
        if (*cPtr1!='\"') continue;
        cPtr1++;
        char *cPtr2=strpbrk(cPtr1, "\"\x0");
        *cPtr2=0;
        fpos_t *fpos;
        fgetpos(fp,fpos);
        fclose(fp);
        char fname[128];
        strcpy(fname, curfname);
        readfile(cPtr1, evORtev);
        fp=fopen(fname, "r");
        fsetpos(fp,fpos);
      }else
      if (i==_EVAL)
      {
        int j, k=0;
        while (*cPtr1==' ') cPtr1++;
        char *cPtr2=strpbrk(cPtr1, " =");
        if (cPtr2==0) continue;
        *cPtr2=0;
        char dname[50];
        strcpy(dname, cPtr1);//читаем имя переменной для подстановки
        cPtr1=cPtr2+1;
        for(j=0; cPtr1[j+k]; j++) //удаляем ВСЕ пробелы из остатка строки...
        {
          while (cPtr1[j+k]==' ') k++;
          cPtr1[j]=cPtr1[j+k];
        }
        char value[100];
        if (!evaluate(cPtr1, value)) strcpy(value, empt);//continue;
        globvars.setvar(dname, value);
      }
      continue;
    }
    if (strstr(strk, "{")!=0) process(fp, blocknum, evORtev);
    if (*evORtev==1) { if (readTevent(strk, *evORtev)==0)
    {
      *evORtev=0;
      continue;
    } continue; }
    if (*evORtev==2) { if (readevent(strk, *evORtev)==0)
    {
      *evORtev=0;
      continue;
    } continue; }
    if (strstr(strk, "}")!=0) return;
    if (strstr(strk, directive[_EVENTT])!=0) {if (readTevent(strk, *evORtev)) *evORtev=1;}else
    if (strstr(strk, directive[_EVENT])!=0) {if (readevent(strk, *evORtev)) *evORtev=2;}
  }
}

int readTevent(char *sty, int isbeg)
{
  if (strstr(sty, "}")) return 0;
  static int eventTnum;
  char *cPtr1, strk[150];
  strcpy(strk, sty);
  if (isbeg==0)
  {
    cPtr1=strk;
    cPtr1+=strlen(directive[_EVENTT]);
    while (*cPtr1==' ') cPtr1++;
    if (!eventT.addeventT(cPtr1)) return 1;
    eventTnum=eventT.iseventT(cPtr1);
    return 1;
  }
  if ((cPtr1=strchr(strk, '&'))!=0)
  {
    cPtr1++;
    char *cPtr2=strpbrk(cPtr1, " =\x0");
    if (cPtr2!=0)
    {
      *cPtr2=0;
      cPtr2++;
      if (!eventT.setvar(eventTnum, cPtr1, cPtr2)) return 1;
    }
  }
  return 1;
}

char *getone(char *ptr1, int *endpl)
{
  while (*ptr1==' ') ptr1++;
  char *ptr2=strpbrk(ptr1, " ,)\x0");
  *ptr2=0;
  ptr2++;
  *endpl=atoi(ptr1);
  return ptr2;
}

int dateok(int day, int mon, int dfw, int dbk, int *dt)
{
  if (day==0) day=curdate.day();
  if (mon==0) mon=curdate.month();
  struct tm curtmt, tmt;
  tmt.tm_sec=0;
  tmt.tm_min=0;
  tmt.tm_hour=0;
  tmt.tm_wday=0;
  tmt.tm_isdst=0;
  curtmt=tmt;
  tmt.tm_year=curdate.year()-1900;
  tmt.tm_mday=day;
  tmt.tm_mon=mon-1;
  curtmt.tm_year=curdate.year()-1900;
  curtmt.tm_mday=curdate.day();
  curtmt.tm_mon=curdate.month()-1;
  time_t curt, t;
  curt=mktime(&curtmt);
  t=mktime(&tmt);
  long tdiff=difftime(t, curt)/86400;
  if (tdiff>183) tdiff-=365;
  if (tdiff<-183) tdiff+=365;
  dbk+=options.daysback;
  dfw+=options.daysfrw;
  if (dbk>40) dbk=40;
  if (dfw>40) dfw=40;
  dbk=-dbk;
  *dt=tdiff;
  if (tdiff>=dbk&&tdiff<=dfw) return 1;
  return 0;
}

void datestrmake(char *datestr, int day, int month)
{
  strcpy(datestr, "");
  char strk[20];
  itoa(day, strk, 10);
  strcat(datestr, strk);
  strcat(datestr, " ");
  strcat(datestr, monthstr[month]);
}

void whenstrmake(char *whenstr, int dt)
{
  if (((dt+2)>=0)&&(dt+2<=4))
  {
    strcpy(whenstr, when[dt+2]);
    return;
  }
  char strk1[5];
  if (dt>0)
  {
    itoa(dt, strk1, 10);
    strcpy(whenstr, "ЧЕРЕЗ ");
    strcat(whenstr, strk1);
  } else
  {
    itoa(-dt, strk1, 10);
    strcpy(whenstr, strk1);
  }
  char sttrkk1[3]="\x0\x0\x0";
  sttrkk1[1]=strk1[strlen(strk1)-1];
  sttrkk1[0]=strk1[strlen(strk1)-2];
  if (strcmp(sttrkk1, "10")>0&&strcmp(sttrkk1, "15")<0) strcat(whenstr, " ДНЕЙ"); else
    switch (strk1[strlen(strk1)-1])
    {
      case '1': strcat(whenstr, " ДЕНЬ");break;
      case '2': case '3': case '4': strcat(whenstr, " ДНЯ");break;
      case '0': case '5': case '6': case '7': case '8': case '9': strcat(whenstr, " ДНЕЙ");break;
    }
  if (dt<0)
  {
    strcat(whenstr, " НАЗАД");
  }
}

int readevent(char *sty, int isbeg)
{
  if (strstr(sty, "}")!=0) return 0;
  static int eventTnum;
  char *cPtr1, *cPtr2, strk[150];
  strcpy(strk, sty);
  cPtr1=strk;
  int dt;
  char txt[150];
  if (isbeg==0)
  {
    cPtr1+=strlen(directive[_EVENT]);
    while (*cPtr1==' ') cPtr1++;
    cPtr2=strpbrk(cPtr1, " \x0");
    *cPtr2=0;
    eventTnum=eventT.iseventT(cPtr1);
    return 2;
  }
    char *stlen;
    stlen=strk+strlen(strk);
    cPtr1=strpbrk(cPtr1, "(");
    if (!cPtr1) return 2;
    *cPtr1=0;
    cPtr1++;
    cPtr2=strpbrk(cPtr1, ")\x0");
    int eday=-1, emon=-1, ebd=-1, efd=-1;
    strcpy(txt, "");
    cPtr1=getone(cPtr1, &eday);
    if (cPtr1<cPtr2)
    {
      cPtr1=getone(cPtr1, &emon);
      if (cPtr1<cPtr2)
      {
        cPtr1=getone(cPtr1, &efd);
        if (cPtr1<cPtr2) cPtr1=getone(cPtr1, &ebd);
      }
    }
    cPtr1=++cPtr2;
    while (*cPtr1==' ') cPtr1++;
    cPtr1=strchr(cPtr1, '\"');
    while (cPtr1)  //Дополнительная конкатенация!!
    {
      cPtr1++;
      cPtr2=strpbrk(cPtr1, "\"\x0");
      *cPtr2=0;
      if (cPtr2<stlen) cPtr2++;
      strcat(txt, cPtr1);
      cPtr1=strchr(cPtr2, '\"');
    }
    //Добавляем только тогда, когда сходится дата!
    if (eday<=0) eday=curdate.day();
    if (emon<=0) emon=curdate.month();
    int aa;
    char bb[10];
    eventT.getvar(eventTnum, "daysfrw", bb);
    aa=atoi(bb);
    if (efd<=0) efd=aa;
    eventT.getvar(eventTnum, "daysback", bb);
    aa=atoi(bb);
    if (ebd<=0) ebd=aa;
    if (dateok(eday, emon, efd, ebd, &dt))
    {
      if (!event.addevent(&eventT, eventTnum)) return 2;
      int C=event.eventC();
      itoa(efd, bb, 10);
      event.setvar(C, "daysfrw", bb);
      itoa(ebd, bb, 10);
      event.setvar(C, "daysback", bb);
      datestrmake(strk, eday, emon);
      event.setvar(C, "eventdate", strk);
      whenstrmake(strk, dt);
      event.setvar(C, "eventwhen", strk);
      event.setvar(C, "eventstr", txt);
      int da=0;
      if (dt>0) da=1;
      else if (dt<0) da=-1;
      for (aa=curdate.wday(); dt!=0; dt=dt-da, aa=aa+da) if (aa<=0) aa=7-aa;
                                                      else if (aa>=7) aa=aa-7;
      event.setvar(C, "weekdate", weekday[aa]);
      if (da==-1) event.setvar(C, "was", "БЫЛ");else
        if (da==1) event.setvar(C, "was", "БУДЕТ");
      int was, j;
      char ws[10];
      globvars.getvar("eventcount", ws);
      was=atoi(ws);
      was++;
      itoa(was, ws, 10);
      globvars.setvar("eventcount", ws);
    }
  return 2;
}

void makepause(int *strks)
{
  *strks=0;
  if (options.pause==-1)
  {
    textattr(7);
    cnout<<"\n";
    gotoxy(1, wherey()-1);
    char *spc="                                            ";
    cnout<<"  --Пауза-- (нажми любую клавишу...)"<<spc;
    gotoxy(1, wherey()-1);
    getch();
    cnout<<"  ----------------------------------"<<spc;
    wait();
  } else
  {
    textattr(7);
    cnout<<"\n";
    cnout<<"  ----------------------------------\n";
    genpause();
  }
}

void outevents()
{
//  if (options.sort) event.sortevents();
  int C=event.eventC();
  int strks=0;
  for(int i=0; i<=C; i++)
  {
    char os[500];
    textcolor(event.color(i));
    textbackground(event.bkcolor(i));
    event.outstr(i, os);
    if (strlen(os)==0) continue;
    char *cPtr1, *cPtr2;
    cPtr2=os;
    while ((cPtr1=strstr(cPtr2, "\\\\"))!=0)
    {
      *cPtr1=0;
      cnout<<cPtr2;
      int j=strlen(cPtr2);
      while (j<80)
      {
        j++;
        cnout<<" ";
      }
      strks++;
      cPtr2=cPtr1+2;
    }
    textcolor(event.color(i));
    textbackground(event.bkcolor(i));
    cnout<<cPtr2;
    strks++;
    int j=strlen(cPtr2);
    while (j<80)
    {
      j++;
      cnout<<" ";
    }
    char strk[100];
    j=event.isvar(i, "freq");
    int freq, dilay;
    if (j>=0)
    {
      event.getvar(i, j, strk);
      freq=atoi(strk);
    }
    j=event.isvar(i, "delay");
    if (j>=0)
    {
      event.getvar(i, j, strk);
      dilay=atoi(strk);
    }
    if (freq)
    {
      delay(50);
      sound(freq);
    }
    if (dilay) delay(dilay);
    nosound();
    if (strks>=22) makepause(&strks);
  }
  if (C>=0)
  {
    gotoxy(80, wherey()-1);
    textattr(7);
    cnout<<"\n";
    for(int j=1; j<=20; j++) cnout<<"    ";
  }
}

#endif