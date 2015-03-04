#ifndef dat_h
#define dat_h

constream cnout;  //Форматируемый поток ввода-вывода!

//=====================================================================//
//                       Класс текущей даты                            //
//=====================================================================//

class Tcurdate
{
  public:
    Tcurdate();
    ~Tcurdate();
    int day();         //Получение дня
    void strday(char *);
    int month();       //Получение месяца
    void strmonth(char *);
    int year();        //Получение года
    void stryear(char *);
    int wday();        //Получение дня недели
    void strwday(char *);
    int chour();
    void strhour(char *);
    int cmin();
    void strmin(char *);
    int csec();
    void strsec(char *);
  private:
    struct tm Date;
};

//=====================================================================//
//                       Класс ошибок при "компиляции"                 //
//=====================================================================//
#define max_errN 20

class Terr
{
  public:
    Terr();
    ~Terr();
    void adderr(const char *);
    char *geterrstr(char *);
  private:
    int errN;
    char *errstr[max_errN];
};

//=====================================================================//
//                   Класс глобальных переменных (define)              //
//=====================================================================//
#define max_glvarsN 40

class Tglobvar
{
  public:
    Tglobvar();
    ~Tglobvar();
    int isvar(const char *);
    int setvar(const char *, const char *);
    char *getvar(const char *, char *);
  private:
    int varN;
    char *varname[max_glvarsN];
    char *varvalue[max_glvarsN];
};
//---------------------------------------------------------------------
struct Toptions
{
  int daysback;  //За сколько дней предупреждать!
  int daysfrw;
  long datef;
  int sort;
//  int music;     //Присутствие музыки
  int pause;     //Пауза после вывода
//  int was;       //Было ли хоть одно сообщение?
  int onerun;    //Запускать только один раз в день
  char begpath[130];  //Путь, из которого был запущен
  char olddir[130];
}options;

#endif