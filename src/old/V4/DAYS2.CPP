// include-файл для программы Days.

FILE *tryfileopen(char *fpath, int *datef_)
{
//  FILE *opfile;
  struct ffblk ffblk;
  char *goodpath=fpath;
  int i=findfirst(fpath, &ffblk, 0);
  if (i)
  {
    findfirst(options.begpath, &ffblk, 0);
    if (i)
    {
      cputs(" Error: file not found...\r\n");
      helper;
      chdir(options.olddir);
      exit(1);
    }
    cputs(" Warning: file not found, opening file from start directory...");
    goodpath=options.begpath;
  }
  *datef_=ffblk.ff_fdate;
  return fopen(goodpath, "r");
}

void freest(int *stC_, char *st_[])
{
  for(int i=0; i<=*stC_; i++) delete st_[i];
  *stC_=-1;
}

void freeglvar(int *glvarsN_, tglvars *glvar_[])
{
  for(int i=0; i<=*glvarsN_; i++) delete glvar_[i];
  *glvarsN_=-1;
}

void freeeventT(int *eventTN_, TeventT *eventT_[])
{
  for(int i=0; i<=*eventTN_; i++)
  {
    for(int j=0; j<=eventT_[i]->varN;j++)
    {
      delete eventT_[i]->varname[j];
      delete eventT_[i]->varvalue[j];
    }
    delete eventT_[i];
  }
  *eventTN_=-1;
}

void freeevent(int *eventN_, Tevent *event_[])
{
  for(int i=0; i<=*eventN_; i++)
  {
    for(int j=0; j<=event_[i]->varN;j++)
    {
      delete event_[i]->varname[j];
      delete event_[i]->varvalue[j];
    }
    delete event_[i];
  }
  *eventN_=-1;
}

int fillst(FILE *dayf_, char *fname, char *dir_, char *st_[], int *stC_, int cnt)  //Заполняем массив st
{
  char strk[255]="";
  while ((!feof(dayf_))&&fgetc(dayf_)!='{');
  fpos_t *fpos;
  fseek(dayf_,-1,1);
  while (!feof(dayf_))
  {
    strk[0]=0;
    char symb=' ';
    int i=0;
    while (symb!='\n'&&symb!=EOF)
    {
      symb=fgetc(dayf_);
      if (symb!='\n'&&symb!=EOF) strk[i++]=symb;
    }
//    if (symb==EOF) break;
    strk[i]=0;
    int j=0;
    while (strk[j]!=';'&&strk[j])j++;
    i=j;
    strk[i--]=0;
    while ((strk[i]==' ')&&(i>0)) strk[i--]=0;
    j=0;
    while (strk[j]==' '&&strk[j]!=0)j++;
    if (j>0) for(int k=0;k<=i-j+1;k++) strk[k]=strk[k+j];
    i-=j-1;
    char *incl=0;
    if ((incl=strstr(strk,_INCLUDE))!=0)
    {
      char dir1_[128];
      strcpy(dir1_,dir_);
      fgetpos(dayf_,fpos);
      fclose(dayf_);
      while (*incl!='\"'&&*incl) incl++;
      if (*incl)
      {
        incl++;
        char newpath[128];
	for(i=0; incl[i]!='\"'&&incl[i]; i++) newpath[i]=incl[i];
	newpath[i]=0;
	struct ffblk ffblk;
        int temp;
        if (!findfirst(newpath,&ffblk,0))
          if ((dayf_=tryfileopen(newpath, &temp))!=NULL)
          { if (fillst(dayf_, newpath, dir_, st_, stC_, cnt+1)) return 1;}
	  else cputs(" Error: unable to open include file...\r\n");
	else cputs(" Error: include file not found...\r\n");
      }
      i=0;
      chdir(dir1_);
      dayf_=fopen(fname,"r");
      fsetpos(dayf_,fpos);
    }
    if (i>0) if ((st_[*stC_+1]=new char[i+1])!=NULL)
      strcpy(st_[++(*stC_)],strk); //Выделяем память под новую строку и сохраняем ее в списке.
    else
    {
      cputs("Error: not enough memory...\r\n");
      freest(stC_, st_);
      chdir(options.olddir);
      exit(1);
    }
  }
  return 0;
}

void helper()
{
  textattr(14);
  cputs("  Программа для напоминания важных дат.\r\n");
  textattr(12);
  cputs(" Для работы Программы требуется текстовый файл (См. документацию.)\r\n");
  textattr(10);
  cputs("    Ключи Программы :\r\n");
  textattr(9);
  cputs("     /? ");
  textattr(11);
  cputs("      -  Эта справка.\r\n");
  textattr(9);
  cputs("     /NM ");
  textattr(11);
  cputs("     -  Отключение музыки.\r\n");
  textattr(9);
  cputs("     /D<n> ");
  textattr(11);
  cputs("   -  За сколько дней предупреждать (умлч. 3).\r\n");
  textattr(9);
  cputs("     /P<n> ");
  textattr(11);
  cputs("   -  Длительность паузы.\r\n");
  textattr(9);
  cputs("     \\<Path> ");
  textattr(11);
  cputs(" -  Путь к файлам с датами.\r\n");
  textattr(9);
  cputs("     /S ");
  textattr(11);
  cputs("      -  Запускать Программу не более 1 раза в сутки.\r\n");
//  textattr(9);
//  cputs("     /Z ");
//  textattr(11);
//  cputs("    -  Выводить знак зодиака для дней рождения.\r\n");
  textattr(9);
  cputs("     /NV ");
  textattr(11);
  cputs("     -  Не выводить праздники, прошедшие вчера.\r\n");
  cputs("\r\n");
  options.was=true;
  return;
}

void exiter()   //Если что-либо выводилось, выдаем копирайт
{
  if (options.was)
   {
     wait();
     if (!options.pause) getch(); else genpause();
     textattr(5);
     cputs(vio);
     textattr(7);
     cputs("\r\n");
     wait();
   };
  return;
}

void sortSt(int *stC_, char *st_[])
{
  int i, j=0;
  for(i=0; i<=*stC_-j; i++)
  {
    while (!(*st_[i+j])) j++;
    st_[i]=st_[i+j];
  }
  for(;i<=*stC_;i++) st_[i]=0;
  *stC_-=j;
}

void readGLVars(const int beg, const int end, int *stC_, char *st_[], int *glvarsN_, const char optSt[], tglvars *glvar_[]) //Выбор глобальных переменных
{
  if (*stC_>=0)
  {
    for(int i=beg; i<=end; i++)
      {
        char *cPtr1;
        char *strk=st_[i];
        if ((cPtr1=strstr(strk, optSt))!=0)
        {
          cPtr1+=8;
          while (*cPtr1==' ') cPtr1++;
          char *cPtr2=strpbrk(cPtr1, " =\0\n");
          if (cPtr2!=0)
          {
            (*glvarsN_)++;
            if ((glvar_[(*glvarsN_)]=new tglvars)==0)
            {
              cputs("  Error: not enough memory...");
              freest(stC_, st_);
              freeglvar(glvarsN_, glvar_);
              chdir(options.olddir);
              exit(1);
            }
            strncpy(glvar_[(*glvarsN_)]->name, cPtr1, (cPtr2)-(cPtr1));
            glvar_[(*glvarsN_)]->name[(cPtr2)-(cPtr1)]=0;
            cPtr1=++cPtr2;
            cPtr2=st_[i]+strlen(st_[i]);
            strcpy(glvar_[(*glvarsN_)]->value, cPtr1);
            glvar_[(*glvarsN_)]->value[(cPtr2)-(cPtr1)]=0;
            delete st_[i];
            st_[i]=0;
          }
        }
      }
    sortSt(stC_ , st_);
  }
}

int endblock(int pos, const int stC_, const char *st_[])
{
  char *symb;
  do
  {
    symb=strstr(st_[pos], "{");
    if (!(*symb)) pos++;
  }
  while ((!(*symb))&&(pos<=stC_));
  if (!(*symb)) return 0;
  int k=1;
  for(int pos2=pos+1; pos2<=stC_; pos2++)
  {
    char *op, *cl;
    op=strstr(st_[pos2], "{");
    cl=strstr(st_[pos2], "}");
    if ((cl!=0)||(op!=0)) {(cl>op)? k-- : k++;}
    if (k==0) break;
  };
  return pos2;
}

void eventtyperead(int *pos, char *symb, int *stC_, char *st_[], int *eventTN_, TeventT *eventT_[], int *glvarsN_, tglvars *glvar_[])
{
  symb+=13;
  while (*symb==' ') symb++;
  char *cPtr1=strpbrk(symb, " \x0\n{");
  *cPtr1=0;
  (*eventTN_)++;
  eventT_[*eventTN_]=new TeventT;
  strcpy(eventT_[*eventTN_]->name, symb);
  eventT_[*eventTN_]->color=7;
  eventT_[*eventTN_]->bkcolor=0;
  eventT_[*eventTN_]->before=options.before;
  strcpy(eventT_[*eventTN_]->format, "@evntdate@\", \"@evntwhen@\", \"@evntstr@");
  eventT_[*eventTN_]->varN=-1;
  int pos2=endblock(*pos, *stC_, st_);
  //Читаем переменные в eventT.
  int i;
  for(i=*pos; i<=pos2; i++)
  {
    char strk[300];
    strcpy(strk, st_[i]);
    char *pt=strstr(strk, "$");// define - переменная - подставляем!
    if (pt)
    {
      *pt=0;
      pt++;
      char *pt1=strpbrk(pt, "$\x0");
      if (pt1)
      {
        *pt1=0;
        char strk1[300];
        strcpy(strk1, pt1+1);
        int j;
        for(j=0; j<=*glvarsN_+1; j++)
          if (j==*glvarsN_+1) break;
          else if (!(strcmp(pt, glvar_[j]->name))) break;
        if (j<=*glvarsN_) strcat(strk, glvar_[j]->value);
        strcat(strk, strk1);
      }
    }
    char *cptr1=strk;
    if (i==*pos)
    {
      cptr1=strstr(strk, "{");
      if ((cptr1)||(*cptr1=='{')) cptr1++;
      else cptr1=strk;
    }
    //А если используется "+" - перенос строки?? - сделать!!
    cptr1=strstr(cptr1, "&");
    char *cptr2;
    if (cptr1)
    {
      cptr2=strpbrk(cptr1, " =\x0");
      *cptr2=0;
      cptr2++;
      if (strcmp(cptr1, _COLOR)==0) eventT_[*eventTN_]->color=atoi(cptr2); else
      if (strcmp(cptr1, _BKCOLOR)==0) eventT_[*eventTN_]->bkcolor=atoi(cptr2); else
      if (strcmp(cptr1, _BEFORE)==0)
      {
        int bef=atoi(cptr2);
        if (bef>options.before) eventT_[*eventTN_]->before=bef;
        else eventT_[*eventTN_]->before=options.before;
      } else
      if (strcmp(cptr1, _FORMAT)==0) strcpy(eventT_[*eventTN_]->format, cptr2); else
      if (*eventTN_<_EVTN)
      {  //Создаем новую переменную!
        eventT_[*eventTN_]->varN++;
	int a1=((eventT_[*eventTN_]->varname[eventT_[*eventTN_]->varN]=new char[20])==0);
	int a2=((eventT_[*eventTN_]->varvalue[eventT_[*eventTN_]->varN]=new char[20])==0);
	if (a1||a2)
	{
          cputs("  Error: not enough memory...");
          freest(stC_, st_);
          freeglvar(glvarsN_, glvar_);
          freeeventT(eventTN_, eventT_);
          chdir(options.olddir);
          exit(1);
	}
	strcpy(eventT_[*eventTN_]->varname[eventT_[*eventTN_]->varN], cptr1);
	strcpy(eventT_[*eventTN_]->varvalue[eventT_[*eventTN_]->varN], cptr2);
      }
    }
  }
  *pos=pos2;
}

int eventgood(Tevent *eventW)
{
  time_t timet, tm1;
  struct tm *tmmt, tmt;
  timet=time(0);
  tmmt=localtime(&timet);
  tmt=*tmmt;
  if (eventW->Time.tm_mday==0) eventW->Time.tm_mday=tmt.tm_mday;
  if (eventW->Time.tm_mon==0) eventW->Time.tm_mon=tmt.tm_mon+1;
  eventW->Time.tm_mon--;
  eventW->Time.tm_year=tmt.tm_year;
  tmt.tm_sec=eventW->Time.tm_sec=0;
  tmt.tm_min=eventW->Time.tm_min=0;
  tmt.tm_hour=eventW->Time.tm_hour=0;
//  tmt->tm_yday=eventW->Time.tm_yday=0;
  tm1=mktime(&(eventW->Time));
  timet=mktime(&tmt);
  int abdiff=(tm1-timet)/86400;
  int pasth=(options.pasthol==1)||(options.pasthol==0&&abdiff>=0);
  abdiff+=eventW->before;
  if ((abdiff>=0)&&(abdiff<=(eventW->before*2))&&pasth) return 1;
  else return 0;
}

void eventcpy(Tevent *event1, const Tevent *event2)
{
  event1->typenum=event2->typenum;
  event1->color=event2->color;
  event1->bkcolor=event2->bkcolor;
  event1->before=event2->before;
  event1->Time=event2->Time;
//  int year;
  strcpy(event1->format, event2->format);
  strcpy(event1->evntdate, event2->evntdate);
  strcpy(event1->evntwhen, event2->evntwhen);
  strcpy(event1->evntstr, event2->evntstr);
  event1->varN=event2->varN;
  for(int a=0; a<=event1->varN; a++)
  {
    event1->varname[a]=new char[20];
    strcpy(event1->varname[a], event2->varname[a]);
    event1->varvalue[a]=new char[20];
    strcpy(event1->varvalue[a], event2->varvalue[a]);
  }
}

void eventread(int *pos, char *symb, int *stC_, char *st_[], int *eventTN_, TeventT *eventT_[], int *glvarsN_, tglvars *glvar_[], int *eventN_, Tevent *event_[])
{
  symb+=6;
  while (*symb==' ') symb++;
  char *cPtr1=strpbrk(symb, " \x0\n{");
  *cPtr1=0;
  cPtr1++;
  char *cPtr2=strpbrk(cPtr1, " \x0\n{");
  *cPtr2=0;
  int i;
  for(i=0; i<=*eventTN_+1; i++)
    if (i<=*eventTN_)
      if (!strcmp(eventT_[i]->name, symb)) break;
  if (!(i<=*eventTN_)) i=-1;
  int pos2=endblock(*pos, *stC_, st_);
  if ((i>=0)&&cPtr1)
  {
    Tevent eventL, eventR;          //Текущее событие.
    eventL.typenum=i;
    int j;
    eventL.varN=-1;
    eventL.color=eventT_[i]->color;
    eventL.bkcolor=eventT_[i]->bkcolor;
    eventL.before=eventT_[i]->before;
    eventL.Time.tm_mday=0;
    eventL.Time.tm_mon=0;
    eventL.Time.tm_year=0;
    *(eventL.evntdate)=0;
    *(eventL.evntwhen)=0;
    strcpy(eventL.format, eventT_[i]->format);
    eventcpy(&eventR, &eventL);
    for(j=*pos; j<=pos2; j++)
    {
      char strk[300];
      strcpy(strk, st_[j]);
      char *pt=strstr(strk, "$");// define-переменная - подставляем
      if (pt)
      {
        *pt=0;
        pt++;
        char *pt1=strpbrk(pt, "$\x0");
        if (pt1)
        {
          *pt1=0;
          char strk1[300];
          strcpy(strk1, pt1+1);
          int j;
          for(j=0; j<=*glvarsN_+1; j++)
            if (j==*glvarsN_+1) break;
            else if (!(strcmp(pt, glvar_[j]->name))) break;
          if (j<=*glvarsN_) strcat(strk, glvar_[j]->value);
          strcat(strk, strk1);
        }
      }
      cPtr1=strk;
      if (j==*pos)
      {
        cPtr1=strpbrk(strk, "{}");
        if ((cPtr1)||(*cPtr1=='{')) cPtr1++;
        else cPtr1=strk;
        if (*cPtr1==0) continue;
      }
      //А если используется "+" - перенос строки?? - сделать!!
      cPtr1=strpbrk(cPtr1, "&");
      if (cPtr1)  //Объявляется локальная переменная
      {
        cPtr2=strpbrk(cPtr1, " =\x0");
        *cPtr2=0;
        cPtr2++;
        {  //Создаем новую переменную!
          eventL.varN++;
      	  int a1=((eventL.varname[eventL.varN]=new char[20])==0);
	  int a2=((eventL.varvalue[eventL.varN]=new char[20])==0);
	  if (a1||a2)
  	  {
  	    cputs(" Error: not enough memory...");
            for(int r=0; r<eventL.varN; r++)
            {
              delete(eventL.varname[r]);
              delete(eventL.varvalue[r]);
            }
            freest(stC_, st_);
            freeglvar(glvarsN_, glvar_);
            freeeventT(eventTN_, eventT_);
            freeevent(eventN_, event_);
            chdir(options.olddir);
            exit(1);
	  }
	  strcpy(eventL.varname[eventL.varN], cPtr1);
	  strcpy(eventL.varvalue[eventL.varN], cPtr2);
        }
        continue;
      }
      cPtr1=strpbrk(strk, "(");
      cPtr2=strpbrk(cPtr1, ")");
      eventcpy(&eventR, &eventL);
      if ((cPtr1||(*cPtr1=='(')))  //  Найдено новое событие
      {
        cPtr1++;
        char *cPtr3=strpbrk(cPtr1, " ,)");
        if ((cPtr3>0)&&(cPtr3<cPtr2))
        {        //ДОБАВИТЬ подставление локальных переменных!!!
          *cPtr3=0;
          cPtr3++;
          eventR.Time.tm_mday=atoi(cPtr1);
          cPtr1=strpbrk(cPtr3, " ,)");
          if ((cPtr1>0)&&(cPtr3<cPtr2))
          {
            *cPtr1=0;
            if (cPtr1==cPtr2) cPtr2++;
            cPtr1++;
            eventR.Time.tm_mon=atoi(cPtr3);
            cPtr3=cPtr1;
//            cPtr3=strpbrk(cPtr1, " ,)");
//            eventR.year=atoi(cPtr1)-1900;
//            if ((cPtr3>0)&&(cPtr1<cPtr2))
//            {
//              *cPtr3=0;
//              if (cPtr3==cPtr2) cPtr2++;
//              cPtr3++;
              cPtr1=strpbrk(cPtr3, " ,)");
              if ((cPtr1>0)&&cPtr3<cPtr2)
              {
                *cPtr1=0;
                if (cPtr1==cPtr2) cPtr2++;
                cPtr1++;
                eventR.before=atoi(cPtr3);
                cPtr3=strpbrk(cPtr1, " ,)");
                if ((cPtr3>0)&&cPtr1<cPtr2)
                {
                  *cPtr3=0;
                  if (cPtr3==cPtr2) cPtr2++;
                  cPtr3++;
                  strcpy(eventR.format, cPtr1);
                }
              }
            //}
          }
        }
      }
      cPtr1=strpbrk(cPtr2, "\"");
      if (cPtr1)
      {
        *cPtr1=0;
        cPtr1++;
        cPtr2=strpbrk(cPtr1, "\"\x0");
        *cPtr2=0; //Вообще-то конкатенация строк не помешала бы!
        strcpy(eventR.evntstr, cPtr1);
        if (eventgood(&eventR))
        {
          (*eventN_)++;
          event_[*eventN_]=new Tevent;
          eventcpy(event_[*eventN_], &eventR);
        }
      }
    }
    int r;
    for(r=0; r<eventL.varN; r++)
    {
      delete(eventL.varname[r]);
      delete(eventL.varvalue[r]);
    }
    for(r=0; r<eventR.varN; r++)
    {
      delete(eventR.varname[r]);
      delete(eventR.varvalue[r]);
    }
  }
  *pos=pos2;
}

void analyzeBlock(int *pos, int *stC_, char *st_[], int *eventTN_, TeventT *eventT_[], int *globvarsN_, tglvars *globvar_[], int *eventN_, Tevent *event_[]) //Анализ списка из файла
{
  int pos2;
  char *symb;
  pos2=endblock(*pos, *stC_, st_);
  for(int i=*pos+1; i<pos2; i++)
  {
    if ((symb=strstr(st_[i], "{"))!=0) analyzeBlock(&i, stC_, st_, eventTN_, eventT_, globvarsN_, globvar_, eventN_, event_);
    if ((symb=strstr(st_[i], _DEFEVNTTYPE))!=0)
      eventtyperead(&i, symb, stC_, st_, eventTN_, eventT_, globvarsN_, globvar_);
    if ((symb=strstr(st_[i], _EVENT))!=0)
      eventread(&i, symb, stC_, st_, eventTN_, eventT_, globvarsN_, globvar_, eventN_, event_);

  }
  *pos=i;
}

void setGLvars(int *glvarsN_, tglvars *glvar_[])  //Устанавливаем парочку стандартных переменных
{
  (*glvarsN_)++;
  glvar_[*glvarsN_]=new tglvars;
  strcpy(glvar_[*glvarsN_]->name, "BDEFAULT");
  char bef[10];
  itoa(options.before, bef, 10);
  strcpy(glvar_[*glvarsN_]->value, bef);
  (*glvarsN_)++;
  glvar_[*glvarsN_]=new tglvars;
  strcpy(glvar_[*glvarsN_]->name, "CDEFAULT");
  strcpy(glvar_[*glvarsN_]->value, _colordef);
  (*glvarsN_)++;
  glvar_[*glvarsN_]=new tglvars;
  strcpy(glvar_[*glvarsN_]->name, "BKCDEFAULT");
  strcpy(glvar_[*glvarsN_]->value, _bkcolordef);
  (*glvarsN_)++;
  glvar_[*glvarsN_]=new tglvars;
  strcpy(glvar_[*glvarsN_]->name, "DAYSVERSION");
  strcpy(glvar_[*glvarsN_]->value, _curversion);
}

void datestr(char *strk, struct tm *tmt)
{
  *strk=0;
  itoa(tmt->tm_mday, strk, 10);
  strcat(strk, " ");
  strcat(strk, month[tmt->tm_mon]);
}

void whenstr(char *strk, struct tm *tmt, const struct tm Rtime)
{
  struct tm Ltime=Rtime;
  Ltime.tm_sec=0;
  Ltime.tm_min=0;
  Ltime.tm_hour=0;
  time_t tim1, tim2;
  tim1=mktime(tmt);
  tim2=mktime(&Ltime);
  int daysleft=(tim1-tim2)/86400;
  if (((daysleft+2)>=0)&&(daysleft+2<=4))
  {
    strcpy(strk, when[daysleft+2]);
    return;
  }
  char strk1[5];
  if (daysleft>0)
  {
    itoa(daysleft, strk1, 10);
    strcpy(strk, "ЧЕРЕЗ ");
    strcat(strk, strk1);
  } else
  {
    itoa(-daysleft, strk1, 10);
    strcpy(strk, strk1);
  }
//  int i=strlen(strk1);
  switch (strk1[strlen(strk1)-1])
  {
    case '1': strcat(strk, " ДЕНЬ");break;
    case '2': case '3': case '4': strcat(strk, " ДНЯ");break;
    case '0': case '5': case '6': case '7': case '8': case '9': strcat(strk, " ДНЕЙ");break;
  }
  if (daysleft<0)
  {
    strcat(strk, " НАЗАД");
  }
}

void outhello(struct tm tmt)
{
  char hellostr[20], datesay[60], dayofweek[13];
  if ((tmt.tm_hour>=0)&&(tmt.tm_hour<5)) strcpy(hellostr, "  Доброй ночи,");else
    if ((tmt.tm_hour>=5)&&(tmt.tm_hour<12)) strcpy(hellostr, "  Доброе утро,");else
      if ((tmt.tm_hour>=12)&&(tmt.tm_hour<18)) strcpy(hellostr, "  Добрый день,");else
        if ((tmt.tm_hour>=18)&&(tmt.tm_hour<=23)) strcpy(hellostr, "  Добрый вечер,");
  char strk[10];
  strcpy(datesay, "  Сегодня ");
  itoa(tmt.tm_mday, strk, 10);
  strcat(datesay, strk);
  strcat(datesay, " ");
  strcat(datesay, month[tmt.tm_mon]);
  itoa(tmt.tm_year+1900, strk, 10);
  strcat(datesay, " ");
  strcat(datesay, strk);
  strcat(datesay, " года, ");
  strcpy(dayofweek, dow[tmt.tm_wday]);
  strcat(dayofweek, ".");
  textattr(13);
  cputs(hellostr);
  cputs("\r\n");
  textattr(10);
  cputs(datesay);
  if (tmt.tm_wday==0||tmt.tm_wday==6) textattr(12);
  else textattr(2);
  cputs(dayofweek);
  cputs("\r\n");
}

void addeventvar(Tevent *event_, char varname[], char *varval)
{
  event_->varN++; //создаем новую переменную!!
  int a1=((event_->varname[event_->varN]=new char[20])==0);
  int a2=((event_->varvalue[event_->varN]=new char[20])==0);
  if (a1||a2)
  {
    cputs(" Error: not enough memory...");
    //Освободить кучу памяти!!!!!!!!!(как?!!)
    chdir(options.olddir);
    exit(1);
  }
  strcpy(event_->varname[event_->varN], varname);
  strcpy(event_->varvalue[event_->varN], varval);
}

void outevents(int eventN_, Tevent *event_[])
{
  time_t timet=time(0);
  tm tmt=*localtime(&timet);
  outhello(tmt);
  for(int i=0; i<=eventN_; i++)
  {
    //Подставляем и вычисляем переменные, выводим всю эту фигню на экран...
    datestr(event_[i]->evntdate, &(event_[i]->Time));
    addeventvar(event_[i], "evntdate", event_[i]->evntdate);
    whenstr(event_[i]->evntwhen, &(event_[i]->Time), tmt);
    addeventvar(event_[i], "evntwhen", event_[i]->evntwhen);
    addeventvar(event_[i], "evntstr", event_[i]->evntstr);
    char formatstr[200];
    strcpy(formatstr, event_[i]->format);
    char *cPtr1=0;
    cPtr1=strpbrk(formatstr, "@");
    while (cPtr1)
    {
      *cPtr1=0;
      cPtr1++;
      char *cPtr2=strpbrk(cPtr1, "@\x0");
      if (cPtr2)
      {
        *cPtr2=0;
        char strk1[300];
        strcpy(strk1, cPtr2+1);
        int j;
        for(j=0; j<=event_[i]->varN+1; j++)
          if (j==event_[i]->varN+1) break;
          else if (!(strcmp(cPtr1, event_[i]->varname[j]))) break;
        if (j<=event_[i]->varN)
        {
          strcat(formatstr, "\"");
          strcat(formatstr, event_[i]->varvalue[j]);
          strcat(formatstr, "\"");
        }
        strcat(formatstr, strk1);
      }
      cPtr1=strpbrk(formatstr, "@");
    }
    char outstr[200];
    int l=strlen(formatstr), osi=0;
    int wr=0;
    for(int j=0; j<l; j++)
    {
      if (formatstr[j]=='\"')
      {
        wr=!wr;
        continue;
      }
      if (wr) outstr[osi++]=formatstr[j];
    }
    outstr[osi]=0;
    textcolor(event_[i]->color);
    textbackground(event_[i]->bkcolor);
    cputs(outstr);
    textattr(7);
    cputs("\r\n");
    options.was++;
    if ((i+1)%24==0&&i<eventN_)
    {
      cputs("  --Пауза-- (нажми любую клавишу...)\r\n");
      wait();
      if (!options.pause) getch(); else genpause();
      options.was=1;
    }
  }
}

void genpause()
{
  for(long ps=1; ps<=options.pause*options.was*15; ps++)
  {
    ps=ps;
    if (kbhit()) if (chup(getch())=='P')
                 {
                   wait();
                   getch();
                   break;
                 } else break;
    delay(10);
  }
}