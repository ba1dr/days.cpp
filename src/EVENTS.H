//include-файл с определениями классов событий и классов типов событий
#ifndef events_h
#define events_h

//=====================================================================//
//                       Класс ТИПА события                            //
//=====================================================================//
#define max_TEvarN 30

class TeventT
{
  friend TeventsT;
  friend Tevents;
  public:
    TeventT();       //Конструктор типа события
    ~TeventT();      //Деструктор типа события
    int isvar(const char *);
    int setvar(const char *, const char *);//Присвоение значения переменной
  private:
    char name[20];                     //Имя типа события
    int varN;                          //Число переменных
    char *varname[max_TEvarN];         //Имена переменных
    char *varvalue[max_TEvarN];        //Значения переменных
};

//=====================================================================//
//                       Класс ТИПОВ событий                           //
//=====================================================================//
#define max_TeventsN 30

class TeventsT
{
  friend Tevents;
  public:
    TeventsT();
    ~TeventsT();
    int iseventT(const char *);
    int isvar(const int, const char *);
    int addeventT(const char *);
    int setvar(const int, const char *, const char *);//Присвоение значения переменной
    void getvar(const int, const char *, char *);
  private:
    int eventTN;
    TeventT *eventTV[max_TeventsN];
};

//=====================================================================//
//                            Класс события                            //
//=====================================================================//
#define max_EvarN 30

class Tevent
{
//  friend Tevents;
  public:
    Tevent();     //Конструктор события
    ~Tevent();    //Деструктор события
    int isvar(const char *);
    int setvar(const char *, const char *);//Присвоение значения переменной
    char *getvar(const int, char *);  //Получение значения к-л переменной
    void dateset(int, int, int);
    int color();
    int bkcolor();
    int daysback();
    int daysfrw();
    char *outstr(char *);       //Строка для вывода
  private:
//    int typenum;               //Принадлежность к какому-либо типу события
    struct tm Time;             //Время события
    int varN;                   //Число переменных
    char *varname[max_EvarN];   //Имена переменных
    char *varvalue[max_EvarN];  //Значения переменных
};

//=====================================================================//
//                            Класс событий                            //
//=====================================================================//
#define max_eventsN 150

class Tevents
{
  friend TeventsT;
  public:
    Tevents();
    ~Tevents();
    int addevent(const TeventsT *, const int);
    int isvar(const int, const char *);
    int setvar(const int, const char *, const char *);//Присвоение значения переменной
    char *getvar(const int, const int, char *);  //Получение значения к-л переменной
    void dateset(const int, int, int, int);
    void sortevents();//
    int color(const int);
    int bkcolor(const int);
    int daysback(const int);
    int daysfrw(const int);
    char *outstr(const int, char *);       //Строка для вывода
    int eventC();
  private:
    int eventN;
    Tevent *eventV[max_eventsN];
};

#endif