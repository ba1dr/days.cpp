// include-файл для программы Days.

#define _INCLUDE "#include"
#define _LANGUAGE "#language"
#define _DEFGLOB "#define"
#define _DEFEVNTTYPE "#defeventtype"
#define _EVENT "#event"
#define _IFDEF "#ifdef"
#define _ELSE "#else"
#define _ENDIF "#endif"
#define _COLOR "&color"
#define _BKCOLOR "&bkcolor"
#define _BEFORE "&before"
#define _FORMAT "&format"

//Типы
struct Toption
{
  int before;    //За сколько дней предупреждать!
  int music;     //Присутствие музыки
  int pasthol;   //Выводить ли прошедшие праздники?
  int pause;     //Пауза после вывода
  int was;       //Было ли хоть одно сообщение?
  int nohello;   //Выводить ли приветствия?
  int onerun;    //Запускать только один раз в день
  char begpath[130];  //Путь, из которого был запущен
  char olddir[130];
}options;

typedef struct TeventT        //Тип события
{
  char name[20];              //Имя типа события
  int color;                  //Цвет по умолчанию
  int bkcolor;                //Цвет фона по умолчанию
  int before;                 //За сколько дней предупреждать по умолчанию
  char format[100];           //Формат выдачи
  int varN;                   //Число переменных
  char *varname[20];          //Имена переменных
  char *varvalue[20];         //Значения переменных
};

typedef struct Tevent
{
  int typenum;                //К какому типу относится (?-зачем-?)
  int color;                  //    -//-      :)
  int bkcolor;
  int before;
  struct tm Time;
//  int year;
  char format[100];
  char evntdate[14];          //Дата прописью для выдачи
  char evntwhen[20];          //<Через сколько дней> для выдачи
  char evntstr[200];          //Текст для выдачи (полный?)
  int varN;
  char *varname[20];          //Имена переменных
  char *varvalue[20];         //Значения переменных
};

typedef struct tglvars
{
  char name[40];
  char value[80];
};

char *(month[12])={"ЯНВАРЯ", "ФЕВРАЛЯ", "МАРТА", "АПРЕЛЯ", "МАЯ", "ИЮНЯ","ИЮЛЯ",
                   "АВГУСТА", "СЕНТЯБРЯ", "ОКТЯБРЯ", "НОЯБРЯ", "ДЕКАБРЯ"};

char *(when[5])={"ПОЗАВЧЕРА", "ВЧЕРА", "СЕГОДНЯ", "ЗАВТРА", "ПОСЛЕЗАВТРА"};

char *(dow[7])={"ВОСКРЕСЕНЬЕ", "ПОНЕДЕЛЬНИК", "ВТОРНИК", "СРЕДА", "ЧЕТВЕРГ", "ПЯТНИЦА", "СУББОТА"};