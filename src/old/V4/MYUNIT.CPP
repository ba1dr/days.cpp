// Описания
const true=1;
const false=0;

// Прототипы
void wait();                           //clear keyboard buffer
char chup(char ch);  //uppercase symbol

//  Функции
void wait() //clear keyboard buffer
{
  while (kbhit()) getch();
}

char chup(char CH)  //uppercase symbol
{
  unsigned char ch=CH;
  if ((ch<97)||((ch>122)&&(ch<160))||((ch>175)&&(ch<224))||(ch>239)) ;
    else if (((ch>96)&&(ch<123))||((ch>159)&&(ch<176))) ch-=32;
      else if (ch>223) ch-=80;
  CH=(char) ch;
  return CH;
}