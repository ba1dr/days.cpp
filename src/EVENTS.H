//include-䠩� � ��।�����ﬨ ����ᮢ ᮡ�⨩ � ����ᮢ ⨯�� ᮡ�⨩
#ifndef events_h
#define events_h

//=====================================================================//
//                       ����� ���� ᮡ���                            //
//=====================================================================//
#define max_TEvarN 30

class TeventT
{
  friend TeventsT;
  friend Tevents;
  public:
    TeventT();       //��������� ⨯� ᮡ���
    ~TeventT();      //�������� ⨯� ᮡ���
    int isvar(const char *);
    int setvar(const char *, const char *);//��᢮���� ���祭�� ��६�����
  private:
    char name[20];                     //��� ⨯� ᮡ���
    int varN;                          //��᫮ ��६�����
    char *varname[max_TEvarN];         //����� ��६�����
    char *varvalue[max_TEvarN];        //���祭�� ��६�����
};

//=====================================================================//
//                       ����� ����� ᮡ�⨩                           //
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
    int setvar(const int, const char *, const char *);//��᢮���� ���祭�� ��६�����
    void getvar(const int, const char *, char *);
  private:
    int eventTN;
    TeventT *eventTV[max_TeventsN];
};

//=====================================================================//
//                            ����� ᮡ���                            //
//=====================================================================//
#define max_EvarN 30

class Tevent
{
//  friend Tevents;
  public:
    Tevent();     //��������� ᮡ���
    ~Tevent();    //�������� ᮡ���
    int isvar(const char *);
    int setvar(const char *, const char *);//��᢮���� ���祭�� ��६�����
    char *getvar(const int, char *);  //����祭�� ���祭�� �-� ��६�����
    void dateset(int, int, int);
    int color();
    int bkcolor();
    int daysback();
    int daysfrw();
    char *outstr(char *);       //��ப� ��� �뢮��
  private:
//    int typenum;               //�ਭ���������� � ������-���� ⨯� ᮡ���
    struct tm Time;             //�६� ᮡ���
    int varN;                   //��᫮ ��६�����
    char *varname[max_EvarN];   //����� ��६�����
    char *varvalue[max_EvarN];  //���祭�� ��६�����
};

//=====================================================================//
//                            ����� ᮡ�⨩                            //
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
    int setvar(const int, const char *, const char *);//��᢮���� ���祭�� ��६�����
    char *getvar(const int, const int, char *);  //����祭�� ���祭�� �-� ��६�����
    void dateset(const int, int, int, int);
    void sortevents();//
    int color(const int);
    int bkcolor(const int);
    int daysback(const int);
    int daysfrw(const int);
    char *outstr(const int, char *);       //��ப� ��� �뢮��
    int eventC();
  private:
    int eventN;
    Tevent *eventV[max_eventsN];
};

#endif