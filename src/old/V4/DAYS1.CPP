// include-䠩� ��� �ணࠬ�� Days.

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

//����
struct Toption
{
  int before;    //�� ᪮�쪮 ���� �।�०����!
  int music;     //������⢨� ��모
  int pasthol;   //�뢮���� �� ��襤訥 �ࠧ�����?
  int pause;     //��㧠 ��᫥ �뢮��
  int was;       //�뫮 �� ��� ���� ᮮ�饭��?
  int nohello;   //�뢮���� �� �ਢ���⢨�?
  int onerun;    //����᪠�� ⮫쪮 ���� ࠧ � ����
  char begpath[130];  //����, �� ���ண� �� ����饭
  char olddir[130];
}options;

typedef struct TeventT        //��� ᮡ���
{
  char name[20];              //��� ⨯� ᮡ���
  int color;                  //���� �� 㬮�砭��
  int bkcolor;                //���� 䮭� �� 㬮�砭��
  int before;                 //�� ᪮�쪮 ���� �।�०���� �� 㬮�砭��
  char format[100];           //��ଠ� �뤠�
  int varN;                   //��᫮ ��६�����
  char *varname[20];          //����� ��६�����
  char *varvalue[20];         //���祭�� ��६�����
};

typedef struct Tevent
{
  int typenum;                //� ������ ⨯� �⭮���� (?-��祬-?)
  int color;                  //    -//-      :)
  int bkcolor;
  int before;
  struct tm Time;
//  int year;
  char format[100];
  char evntdate[14];          //��� �ய���� ��� �뤠�
  char evntwhen[20];          //<��१ ᪮�쪮 ����> ��� �뤠�
  char evntstr[200];          //����� ��� �뤠� (�����?)
  int varN;
  char *varname[20];          //����� ��६�����
  char *varvalue[20];         //���祭�� ��६�����
};

typedef struct tglvars
{
  char name[40];
  char value[80];
};

char *(month[12])={"������", "�������", "�����", "������", "���", "����","����",
                   "�������", "��������", "�������", "������", "�������"};

char *(when[5])={"���������", "�����", "�������", "������", "�����������"};

char *(dow[7])={"�����������", "�����������", "�������", "�����", "�������", "�������", "�������"};