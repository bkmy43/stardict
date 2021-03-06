#include <time.h>

extern time_t Ftime;


char *CordInts2String(int g, int m, char *s, char rezhim);
   /*
   �८�ࠧ�� ���������� ���न���� � ��ப� ⨯� 25N03 ��� 123W56
		   rezhim:  T - ���; N - ������
   */
char *CordInts3String(int g, int m, int sg, char *s, char rezhim);
					   /* T - ���; N - ������*/
   /* ���஥�� ⠪��, �� �����ਥ �������� �� ������ g ��� m,
      � ᮮ⢥�����饩 �㪢�� N/S W/E (sg) */

char *CordDouble2String(double k, char *s, char rezhim);
   /*
   �८�ࠧ�� ���������� ���न���� � ��ப� ⨯� 25N03 ��� 123W56
		   rezhim:  T - ���; N - ������
   */

void CordString2Ints(char *str, int *g, int *m);
void CordString2Double(char *str, double *k);
void CordString3Ints(char *str, int *g, int *m, int *sg);

time_t GetCurrentTime(void);
   /*
   �����頥� ⥪�饥 �६� � ᥪ㭤��
   ��⠭�������� Ftime
   */

long GetPassedTime(void);
     /*
     �����頥� �६� (� ᥪ㭤�� !), ��饤襥 � ��᫥����� �맮�� �⮩
     �㭪樨 ��� GetCurrentTime
     ��⠭�������� Ftime
     ४��������� �� ��ࢮ�� �맮�� �ᯮ�짮���� �㭪�� GetCurrentTime
     */

void Seconds2String(long seconds, char *str, char *format);
     /* �८�ࠧ�� ������⢮ ᥪ㭤 � ��ப� �� �ଠ⠬
     ⨯� HH:MM:SS ��� MM:SS � �.�. */

char *Kord2String(double kord, char *str);
     /* �८�ࠧ�� ���न���� ������� � ��ப� ⨯� �����
	� ����⢥ � - ����� ����� � inner */

char *Date2String(int d, int m, int y, char *str, char *format);
		  /* DD:MM:YY  MM:DD:YYYY */
char *Date2StringE(int d, int m, int y, char epoch, char *str, char *format);
					   /* B/A */
void String2Date(char *str, int *d, int *m, int *y, char *format);
void String2DateE(char *str, int *d, int *m, int *y, char *epoch, char *format);
					   /* B/A */

void CordString2Int(char *str, int *cord);
char *CordInt2String(int cord, char *str, char rezhim);
char *Time2String(int h, int m, char *str, char *format);
void String2Time(char *str, int *h, int *m, char *format);
