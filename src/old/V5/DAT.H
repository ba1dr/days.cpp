#ifndef dat_h
#define dat_h

constream cnout;  //��ଠ��㥬� ��⮪ �����-�뢮��!

//=====================================================================//
//                       ����� ⥪�饩 ����                            //
//=====================================================================//

class Tcurdate
{
  public:
    Tcurdate();
    ~Tcurdate();
    int day();         //����祭�� ���
    void strday(char *);
    int month();       //����祭�� �����
    void strmonth(char *);
    int year();        //����祭�� ����
    void stryear(char *);
    int wday();        //����祭�� ��� ������
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
//                       ����� �訡�� �� "�������樨"                 //
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
//                   ����� ��������� ��६����� (define)              //
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
  int daysback;  //�� ᪮�쪮 ���� �।�०����!
  int daysfrw;
  long datef;
  int sort;
//  int music;     //������⢨� ��모
  int pause;     //��㧠 ��᫥ �뢮��
//  int was;       //�뫮 �� ��� ���� ᮮ�饭��?
  int onerun;    //����᪠�� ⮫쪮 ���� ࠧ � ����
  char begpath[130];  //����, �� ���ண� �� ����饭
  char olddir[130];
}options;

#endif